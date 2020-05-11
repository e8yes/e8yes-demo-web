from script.parse_node_config import NodeConfig
from script.parse_node_config import ClusterConfig
from script.parse_node_config import ReadNodeConfig
from script.refresh_host_keys import RefreshHostKeys
from script.run_bash_script import RunSingleCommandInNode
from script.host_ip import GetHostIp

def BuildImage(git_repo: str, deployment_node: NodeConfig):
  RunSingleCommandInNode(node=deployment_node, 
                         command="git clone {0} demoweb_src"\
                           .format(git_repo))
  RunSingleCommandInNode(node=deployment_node, 
                         command="cd demoweb_src && git pull {0} master"\
                           .format(git_repo))
  RunSingleCommandInNode(node=deployment_node, 
                         command="cd demoweb_src/script && ./build.sh {0}"\
                           .format(GetHostIp(deployment_node.location)))

def PrepareNodeForDockerRegistry(node: NodeConfig, deployment_node: NodeConfig):
  update_daemon_config_cmd = "echo '{{ \"insecure-registries\":[\"{0}:5000\"] }}' | sudo tee /etc/docker/daemon.json"\
                           .format(GetHostIp(deployment_node.location))
  RunSingleCommandInNode(node=node, command=update_daemon_config_cmd)
  RunSingleCommandInNode(node=node, command="sudo systemctl restart docker")


def PushToDockerRegistry(deployment_node: NodeConfig):
  RunSingleCommandInNode(node=deployment_node, 
                         command="sudo docker push {0}:5000/demowebservice"\
                           .format(GetHostIp(deployment_node.location)))

def DeployImage(kube_master_node: NodeConfig, deployment_node: NodeConfig):
  # TODO: Pull in yaml templates and complete the placeholders then deploy through kubernetes.
  pass

if __name__ == "__main__":
  node_configs, cluster_config = ReadNodeConfig(
    config_file_path="nodes_configuration.json")

  print("Refreshing host keys...")
  RefreshHostKeys(node_configs.values())

  deployment_node = node_configs[cluster_config.deployment_master]
  git_repo = cluster_config.git_repo

  print("Building from source " + git_repo + " in " + str(deployment_node))
  BuildImage(git_repo=git_repo, deployment_node=deployment_node)

  print("Prepare nodes for docker registry")
  for node_config in node_configs.values():
    print("Prepare", node_config, "for docker registry")
    PrepareNodeForDockerRegistry(node=node_config, deployment_node=deployment_node)

  print("Push built image to registry")
  PushToDockerRegistry(deployment_node=deployment_node)

  print("Deploy image")
  kube_master_node = node_configs[cluster_config.kubernetes_master]
  DeployImage(kube_master_node=kube_master_node, deployment_node=deployment_node)
