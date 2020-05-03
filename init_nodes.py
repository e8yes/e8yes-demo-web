import subprocess

from script.parse_node_config import NodeConfig
from script.parse_node_config import ClusterConfig
from script.parse_node_config import ReadNodeConfig
from script.run_bash_script import RunScriptInNode

def InstallPackagesForNode(node_config: NodeConfig, script_file_path: str):
  RunScriptInNode(node=node_config, script_file_path=script_file_path)

if __name__ == "__main__":
  node_configs, cluster_config = ReadNodeConfig(
    config_file_path="nodes_configuration.json")
  for node_name, node_config in node_configs.items():
    print("Installing packages for node", node_name, node_config)
    InstallPackagesForNode(node_config=node_config, 
                           script_file_path="script/install_pkgs_for_node.sh")

