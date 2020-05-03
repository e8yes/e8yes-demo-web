import subprocess

from script.parse_node_config import NodeConfig
from script.parse_node_config import ReadNodeConfig
from script.run_bash_script import RunScriptInNode

def InitNode(node_config: NodeConfig, init_script_file_path: str):
  RunScriptInNode(node=node_config, script_file_path=init_script_file_path)

if __name__ == "__main__":
  node_configs = ReadNodeConfig(config_file_path="nodes_configuration.json")
  for node_config in node_configs:
    print("Initializing node", node_config)
    InitNode(node_config=node_config, 
             init_script_file_path="script/init_node.sh")

