import subprocess
from os.path import basename

from script.parse_node_config import NodeConfig

def RunScriptInNode(node: NodeConfig, script_file_path: str):
  script_file_name = basename(script_file_path)
  subprocess.call(["sshpass", "-p", node.ssh_password, 
                   "scp", script_file_path, 
                   node.user + "@" + node.location + ":~/" + script_file_name])
  subprocess.call(["sshpass", "-p", node.ssh_password, 
                   "ssh", node.user + "@" + node.location, 
                   "~/" + script_file_name])
  subprocess.call(["sshpass", "-p", node.ssh_password, 
                   "ssh", node.user + "@" + node.location, 
                   "rm ~/" + script_file_name])
