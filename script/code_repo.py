from typing import List
from script.parse_node_config import ClusterConfig
from script.parse_node_config import NodeConfig
from script.run_bash_script import RunSingleCommandInNode

CODE_REPO_LOCATION = "demoweb_src"

def SyncCodeRepo(git_repo: str, node: NodeConfig):
  print("Synchronizing code repository at " + str(node))
  RunSingleCommandInNode(node=node, 
                         command="git clone {0} {1}"\
                          .format(git_repo, CODE_REPO_LOCATION))
  RunSingleCommandInNode(node=node,
                         command="cd {0} && git reset --hard"
                          .format(CODE_REPO_LOCATION))
  RunSingleCommandInNode(node=node, 
                         command="cd {0} && git pull {1} master"\
                          .format(CODE_REPO_LOCATION, git_repo))

def SyncCodeRepoInOperationalNodes(cluster_config: ClusterConfig,
                                   node_configs: List[NodeConfig]):
  SyncCodeRepo(git_repo=cluster_config.git_repo,
               node=node_configs[cluster_config.kubernetes_master])
  SyncCodeRepo(git_repo=cluster_config.git_repo,
               node=node_configs[cluster_config.deployment_master])
  SyncCodeRepo(git_repo=cluster_config.git_repo,
               node=node_configs[cluster_config.postgres_citus_master])
