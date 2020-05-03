import subprocess
from os.path import basename

from script.parse_node_config import NodeConfig

def RunSingleCommandInNode(node: NodeConfig,
                           command: str,
                           retrieve_output=False):
  if retrieve_output:
    output = subprocess.check_output([
      "sshpass", "-p", node.ssh_password, 
      "ssh", node.user + "@" + node.location, command])
    return output
  else:
    output = subprocess.call([
      "sshpass", "-p", node.ssh_password, 
      "ssh", node.user + "@" + node.location, command])
    return None

def RunScriptInNode(node: NodeConfig, script_file_path: str):
  script_file_name = basename(script_file_path)
  subprocess.call(["sshpass", "-p", node.ssh_password, 
                   "scp", script_file_path, 
                   node.user + "@" + node.location + ":~/" + script_file_name])
  RunSingleCommandInNode(node=node, command="~/" + script_file_name)
  RunSingleCommandInNode(node=node, command="rm ~/" + script_file_name)

def ToSingleLineString(bytes_output: bytes):
  return bytes_output[:-1].decode("utf-8")

