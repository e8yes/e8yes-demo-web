import subprocess
from typing import List
from script.config import NodeConfig
from script.config import ClusterConfig
from script.config import LoadSourceOfTruths
from script.run_bash_script import RunScriptInNode
from script.run_bash_script import RunSingleCommandInNode
from script.run_bash_script import UploadScriptToNode
from script.run_bash_script import ToSingleLineString
from script.refresh_host_keys import RefreshHostKeys
from script.template_instantiator import TemplateInstantiator

def InstallPackagesForNode(node_config: NodeConfig, script_file_path: str):
  RunScriptInNode(node=node_config, script_file_path=script_file_path)

def ClearDockerImages(node: NodeConfig):
  RunSingleCommandInNode(
    node=node, command="sudo docker stop `sudo docker ps -a -q`")
  RunSingleCommandInNode(
    node=node, command="sudo docker rm --force `sudo docker ps -a -q`")
  RunSingleCommandInNode(
    node=node, command="sudo docker rmi --force `sudo docker images -q`")                         

def SetUpDockerRegistry(deployment_node_config: NodeConfig,
                        docker_registry_port: str):
  RunSingleCommandInNode(
    node=deployment_node_config, 
    command="sudo docker run -d -p {0}:{0} --restart=always --name=registry registry:latest"
      .format(docker_registry_port))

def PrepareNodeForDockerRegistry(node: NodeConfig, 
                                 deployment_node: NodeConfig,
                                 docker_registry_port: str):
  update_daemon_config_cmd = \
    "echo '{{ \"insecure-registries\":[\"{0}:{1}\"] }}' | sudo tee /etc/docker/daemon.json"\
      .format(deployment_node.location, docker_registry_port)
  RunSingleCommandInNode(node=node, command=update_daemon_config_cmd)
  RunSingleCommandInNode(node=node, command="sudo systemctl restart docker")

def SetUpPostgresMasterNode(postgres_master: NodeConfig):
  InstallPackagesForNode(node_config=postgres_master, 
                         script_file_path="script/install_pkgs_for_db_node.sh")
  
  UploadScriptToNode(node=postgres_master,
                     script_file_path="script/pg_hba.conf")
  UploadScriptToNode(node=postgres_master,
                     script_file_path="script/postgresql.conf")
  RunSingleCommandInNode(node=postgres_master,
                         command="sudo mv pg_hba.conf /etc/postgresql/11/main/pg_hba.conf")
  RunSingleCommandInNode(node=postgres_master,
                         command="sudo mv postgresql.conf /etc/postgresql/11/main/postgresql.conf")
  RunSingleCommandInNode(node=postgres_master,
                         command="sudo chown root /etc/postgresql/11/main/pg_hba.conf")
  RunSingleCommandInNode(node=postgres_master,
                         command="sudo chown root /etc/postgresql/11/main/postgresql.conf")

  RunSingleCommandInNode(node=postgres_master,
                         command="sudo systemctl restart postgresql")

def DefineNginxCluster(cluster_nodes: List[NodeConfig], port: int):
  defn = ""
  for node in cluster_nodes:
    defn += "server {0}:{1};\n\t\t".format(node.location, port)
  return defn[:-3]


def SetUpLoadBalancerNode(load_balancer: NodeConfig,
                          cluster_nodes: List[NodeConfig],
                          instantiator: TemplateInstantiator):
  InstallPackagesForNode(node_config=load_balancer, 
                         script_file_path="script/install_pkgs_for_load_balancer_node.sh")

  web_frontend_cluster = DefineNginxCluster(cluster_nodes=cluster_nodes, port=8080)
  grpcweb_cluster = DefineNginxCluster(cluster_nodes=cluster_nodes, port=8000)
  instantiator.AddVar("{{demoweb_web_frontend_cluster}}", web_frontend_cluster)
  instantiator.AddVar("{{demoweb_grpcweb_service_cluster}}", grpcweb_cluster)

  instantiator.Instantiate(template_file_path="script/nginx.conf.tmpl", 
                           target_node=None)
  
  UploadScriptToNode(node=load_balancer,
                     script_file_path="script/nginx.conf")
  RunSingleCommandInNode(node=load_balancer,
                         command="sudo mv nginx.conf /etc/nginx/nginx.conf")     
  subprocess.call(["rm", "script/nginx.conf"])       

  UploadScriptToNode(node=load_balancer,
                     script_file_path="script/nginx_default.conf")
  RunSingleCommandInNode(node=load_balancer,
                         command="sudo mv nginx_default.conf /etc/nginx/sites-available/default")

  RunSingleCommandInNode(node=load_balancer,
                         command="sudo systemctl restart nginx")

def SetUpMountpoint(node: NodeConfig, mount_point: str):
  RunSingleCommandInNode(node=node,
                         command="mkdir -p {0}".format(mount_point))

if __name__ == "__main__":
  node_configs, cluster_config, build_targets = LoadSourceOfTruths(
    config_file_path="source_of_truths.json")
  
  instantiator = TemplateInstantiator(cluster_config, 
                                      node_configs,
                                      build_targets)

  print("Refreshing host keys...")
  RefreshHostKeys(node_configs.values())

  for node_name, node_config in node_configs.items():
    print("Installing packages for node", node_name, node_config)
    InstallPackagesForNode(node_config=node_config, 
                           script_file_path="script/install_pkgs_for_node.sh")

  for node_name, node_config in node_configs.items():
    print("Set up mount point for node", node_name, node_config)
    SetUpMountpoint(node=node_config, 
                    mount_point=cluster_config.mount_point)
  
  print("Setting up the load balancer...")
  load_balancer_node = node_configs[cluster_config.load_balancer]
  SetUpLoadBalancerNode(load_balancer=load_balancer_node,
                        cluster_nodes=node_configs.values(),
                        instantiator=instantiator)

  print("Setting up the postgres master...")
  postgres_node = node_configs[cluster_config.postgres_citus_master]
  SetUpPostgresMasterNode(postgres_master=postgres_node)
  
  for node_name, node_config in node_configs.items():
    print("Clearing docker images for node", node_name, node_config)
    ClearDockerImages(node=node_config)

  deployment_node = node_configs[cluster_config.deployment_master]
  print("Setting up docker image registry on ", deployment_node)
  SetUpDockerRegistry(
    deployment_node_config=deployment_node,
    docker_registry_port=cluster_config.deployment_image_registry_port)

  print("Preparing nodes for docker registry")
  for node_config in node_configs.values():
    print("Preparing", node_config, "for docker registry")
    PrepareNodeForDockerRegistry(
      node=node_config,
      deployment_node=deployment_node,
      docker_registry_port=cluster_config.deployment_image_registry_port)
