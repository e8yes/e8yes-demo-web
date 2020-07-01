import subprocess

from script.config import NodeConfig
from script.config import ClusterConfig
from script.config import LoadSourceOfTruths
from script.run_bash_script import RunScriptInNode
from script.run_bash_script import RunSingleCommandInNode
from script.run_bash_script import UploadScriptToNode
from script.run_bash_script import ToSingleLineString
from script.refresh_host_keys import RefreshHostKeys

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
  InstallPackagesForNode(node_config=postgres_node, 
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

def SetUpLoadBalancerNode(load_balancer: NodeConfig):
  InstallPackagesForNode(node_config=load_balancer, 
                         script_file_path="script/install_pkgs_for_load_balancer_node.sh")

if __name__ == "__main__":
  node_configs, cluster_config, _ = LoadSourceOfTruths(
    config_file_path="source_of_truths.json")

  print("Refreshing host keys...")
  RefreshHostKeys(node_configs.values())

  for node_name, node_config in node_configs.items():
    print("Installing packages for node", node_name, node_config)
    InstallPackagesForNode(node_config=node_config, 
                           script_file_path="script/install_pkgs_for_node.sh")
  
  print("Setting up the load balancer...")
  load_balancer_node = node_configs[cluster_config.load_balancer]
  SetUpLoadBalancerNode(load_balancer=load_balancer_node)

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
