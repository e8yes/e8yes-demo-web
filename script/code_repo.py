from typing import List
from script.config import ClusterConfig
from script.config import NodeConfig
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
  for node in node_configs:
    SyncCodeRepo(git_repo=cluster_config.git_repo, node=node)
