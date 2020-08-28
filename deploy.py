from script.config import NodeConfig
from script.config import ClusterConfig
from script.config import TopologyConfig
from script.config import LoadSourceOfTruths
from script.refresh_host_keys import RefreshHostKeys
from script.run_bash_script import UploadScriptToNode
from script.run_bash_script import RunScriptInNode
from script.run_bash_script import RunSingleCommandInNode
from script.code_repo import SyncCodeRepoInOperationalNodes
from script.code_repo import CODE_REPO_LOCATION
from script.template_instantiator import TemplateInstantiator
from script.build_images import BuildAndPushTargetImages
from script.deploy_images import DeployImages


def PushPostgresSchema(postgres_node: NodeConfig):
  RunSingleCommandInNode(node=postgres_node, 
                         command="cd {0}/postgres && ./push_schema.sh"
                          .format(CODE_REPO_LOCATION))

if __name__ == "__main__":
  node_configs, cluster_config, topology_configs, build_targets = LoadSourceOfTruths(
    config_file_path="source_of_truths.json")

  instantiator = TemplateInstantiator(cluster_config, 
                                      node_configs,
                                      build_targets)

  print("Refreshing host keys...")
  RefreshHostKeys(node_configs.values())

  print("Synchronizing code repository...")
  SyncCodeRepoInOperationalNodes(cluster_config, node_configs.values())

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

  print("Deploy images")
  DeployImages(deployment_node=deployment_node,
               docker_registry_port=cluster_config.deployment_image_registry_port,
               home_mount_point=cluster_config.home_mount_point,
               root_mount_point=cluster_config.root_mount_point,
               targets=build_targets,
               nodes=node_configs.values())
