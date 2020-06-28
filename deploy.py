from script.config import NodeConfig
from script.config import ClusterConfig
from script.config import LoadSourceOfTruths
from script.refresh_host_keys import RefreshHostKeys
from script.run_bash_script import UploadScriptToNode
from script.run_bash_script import RunScriptInNode
from script.run_bash_script import RunSingleCommandInNode
from script.code_repo import SyncCodeRepoInOperationalNodes
from script.code_repo import CODE_REPO_LOCATION
from script.template_instantiator import TemplateInstantiator
from script.build_images import BuildAndPushTargetImages



def PushPostgresSchema(postgres_node: NodeConfig):
  RunSingleCommandInNode(node=postgres_node, 
                         command="cd {0}/postgres && ./push_schema.sh"
                          .format(CODE_REPO_LOCATION))

def DeployImages(kube_master_node: NodeConfig, 
                 instantiator: TemplateInstantiator):
  deployment_template_file_path = \
    "{0}/script/demoweb_service_deployment.yaml.tmpl"\
    .format(CODE_REPO_LOCATION)
  deployment_config_file_path = instantiator.Instantiate(
    template_file_path=deployment_template_file_path,
    target_node=kube_master_node)

  RunSingleCommandInNode(
    node=kube_master_node, 
    command="kubectl apply -f {0}".format(deployment_config_file_path))

if __name__ == "__main__":
  node_configs, cluster_config, build_targets = LoadSourceOfTruths(
    config_file_path="source_of_truths.json")

  instantiator = TemplateInstantiator(cluster_config, 
                                      node_configs,
                                      build_targets)

  print("Refreshing host keys...")
  RefreshHostKeys(node_configs.values())

  print("Synchronizing code repository...")
  SyncCodeRepoInOperationalNodes(cluster_config, node_configs)

  print("Pushing postgres schemas...")
  postgres_node = node_configs[cluster_config.postgres_citus_master]
  PushPostgresSchema(postgres_node)

  deployment_node = node_configs[cluster_config.deployment_master]
  print("Building targets in " + str(deployment_node))
  BuildAndPushTargetImages(
    targets=build_targets,
    instantiator=instantiator,
    code_repo_base_path=CODE_REPO_LOCATION,
    deployment_node=deployment_node,
    docker_registry_port=cluster_config.deployment_image_registry_port)

  print("Deploy image")
  kube_master_node = node_configs[cluster_config.kubernetes_master]
  DeployImages(kube_master_node=kube_master_node,
               instantiator=instantiator)
