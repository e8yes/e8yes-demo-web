from typing import List
import json

class NodeConfig:
  def __init__(self, name: str, user: str, location: str, ssh_password: str):
    self.name = name
    self.user = user
    self.location = location
    self.ssh_password = ssh_password

  def __repr__(self):
    return "NodeConfig={name=" + self.name + \
                      ",user=" + self.user + \
                      ",location=" + self.location + \
                      ",ssh_password=" + self.ssh_password + \
                      "}"

def ReadNodeConfig(config_file_path: str) -> List[NodeConfig]:
  with open(config_file_path, "r") as config_file:
    content = config_file.read()

  json_obj = json.loads(content)
  json_nodes_config = json_obj["nodes"]

  node_configs: List[NodeConfig] = list()
  for json_node_config in json_nodes_config:
    node_configs.append(NodeConfig(name=json_node_config["name"],
                                   user=json_node_config["user"],
                                   location=json_node_config["location"],
                                   ssh_password=json_node_config["ssh_password"]))
  return node_configs

