from script.parse_node_config import NodeConfig
from script.parse_node_config import ClusterConfig
from script.parse_node_config import ReadNodeConfig
from script.refresh_host_keys import RefreshHostKeys
from script.run_bash_script import UploadScriptToNode
from script.run_bash_script import RunScriptInNode
from script.run_bash_script import RunSingleCommandInNode
from script.code_repo import SyncCodeRepoInOperationalNodes
from script.code_repo import CODE_REPO_LOCATION
from script.template_instantiator import TemplateInstantiator


def PushPostgresSchema(postgres_node: NodeConfig):
  RunSingleCommandInNode(node=postgres_node, 
                         command="cd {0}/postgres && ./push_schema.sh"
                          .format(CODE_REPO_LOCATION))

def BuildImages(git_repo: str, deployment_node: NodeConfig):
  RunSingleCommandInNode(node=deployment_node, 
                         command="cd {0}/script && ./build.sh {1}"\
                          .format(CODE_REPO_LOCATION, deployment_node.location))

def PrepareNodeForDockerRegistry(node: NodeConfig, deployment_node: NodeConfig):
  update_daemon_config_cmd = "echo '{{ \"insecure-registries\":[\"{0}:5000\"] }}' | sudo tee /etc/docker/daemon.json"\
                           .format(deployment_node.location)
  RunSingleCommandInNode(node=node, command=update_daemon_config_cmd)
  RunSingleCommandInNode(node=node, command="sudo systemctl restart docker")


def PushToDockerRegistry(deployment_node: NodeConfig):
  RunSingleCommandInNode(node=deployment_node, 
                         command="sudo docker push {0}:5000/demowebservice"\
                           .format(deployment_node.location))

def DeployImages(kube_master_node: NodeConfig, 
                 deployment_node: NodeConfig,
                 instantiator: TemplateInstantiator):
  with open("./script/template-demoweb_service_deployment.yaml", "r") as demoweb_service_deployment_template:
    template = demoweb_service_deployment_template.read()
    # TODO: Refactor this to a more re-usable construct.
    deployment_config = template.replace(\
      "DEMOWEB_SERVICE_IMAGE", 
      "{0}:5000/demowebservice".format(deployment_node.location))
    write_config_file_cmd = "echo -e \"{0}\" > {1}/script/demoweb_service_deployment.yaml"\
      .format(deployment_config, CODE_REPO_LOCATION)
    RunSingleCommandInNode(node=kube_master_node, command=write_config_file_cmd)
    RunSingleCommandInNode(node=kube_master_node, 
                           command="kubectl apply -f demoweb_src/script/demoweb_service_deployment.yaml")

if __name__ == "__main__":
  node_configs, cluster_config = ReadNodeConfig(
    config_file_path="source_of_truths.json")

  instantiator = TemplateInstantiator(cluster_config, node_configs)

  print("Refreshing host keys...")
  RefreshHostKeys(node_configs.values())

  print("Synchronizing code repository...")
  SyncCodeRepoInOperationalNodes(cluster_config, node_configs)

  print("Pushing postgres schemas...")
  postgres_node = node_configs[cluster_config.postgres_citus_master]
  PushPostgresSchema(postgres_node)

  deployment_node = node_configs[cluster_config.deployment_master]
  git_repo = cluster_config.git_repo

  print("Building from source " + git_repo + " in " + str(deployment_node))
  BuildImages(git_repo=git_repo, deployment_node=deployment_node)

  print("Prepare nodes for docker registry")
  for node_config in node_configs.values():
    print("Prepare", node_config, "for docker registry")
    PrepareNodeForDockerRegistry(node=node_config, deployment_node=deployment_node)

  print("Push built image to registry")
  PushToDockerRegistry(deployment_node=deployment_node)

  print("Deploy image")
  kube_master_node = node_configs[cluster_config.kubernetes_master]
  DeployImages(kube_master_node=kube_master_node, 
               deployment_node=deployment_node,
               instantiator=instantiator)
