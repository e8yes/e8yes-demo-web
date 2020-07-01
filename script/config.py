from typing import Dict
from typing import List
from typing import Tuple
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

class ClusterConfig:
  def __init__(self, 
               load_balancer: str, 
               postgres_citus_master: str,
               postgres_citus_master_port: int,
               postgres_citus_master_user: str,
               postgres_citus_master_password: str,
               deployment_master: str,
               deployment_image_registry_port: str,
               git_repo: str):
    self.load_balancer = load_balancer
    self.postgres_citus_master = postgres_citus_master
    self.postgres_citus_master_port = postgres_citus_master_port
    self.postgres_citus_master_user = postgres_citus_master_user
    self.postgres_citus_master_password = postgres_citus_master_password
    self.deployment_master = deployment_master
    self.deployment_image_registry_port = deployment_image_registry_port
    self.git_repo = git_repo

  def __repr__(self):
    return "ClusterConfig={load_balancer=" + self.load_balancer + \
                         ",postgres_citus_master=" + \
                            self.postgres_citus_master + \
                         ",postgres_citus_master_port" + \
                            str(self.postgres_citus_master_port) + \
                         ",postgres_citus_master_user" + \
                            self.postgres_citus_master_user + \
                         ",postgres_citus_master_password=" + \
                            self.postgres_citus_master_password + \
                         ",deployment_master=" + self.deployment_master + \
                         ",deployment_image_registry_port=" + \
                            self.deployment_image_registry_port + \
                         ",git_repo=" + self.git_repo + \
                         "}"

class BuildTarget:
  def __init__(self, 
               name: str, 
               docker_template: str,
               pushable: bool,
               open_ports: List[int]):
    self.name = name
    self.docker_template = docker_template
    self.pushable = pushable
    self.open_ports = open_ports
  
  def __repr__(self):
    return "BuildTarget={name=" + self.name + \
                       ",docker_template=" + self.docker_template + \
                       ",pushable=" + str(self.pushable) + \
                       "}"

def LoadSourceOfTruths(config_file_path: str) -> Tuple[Dict[str, NodeConfig],
                                                       ClusterConfig,
                                                       List[BuildTarget]]:
  with open(config_file_path, "r") as config_file:
    content = config_file.read()

  json_obj = json.loads(content)

  json_nodes_config = json_obj["nodes"]
  node_configs: Dict[str, NodeConfig] = dict()
  for json_node_config in json_nodes_config:
    node_configs[json_node_config["name"]] = \
      NodeConfig(name=json_node_config["name"],
                 user=json_node_config["user"],
                 location=json_node_config["location"],
                 ssh_password=json_node_config["ssh_password"])

  json_cluster_config = json_obj["cluster"]
  cluster_config = ClusterConfig(
    load_balancer=json_cluster_config["load_balancer"],
    postgres_citus_master=json_cluster_config["postgres_citus_master"],
    postgres_citus_master_port=\
      json_cluster_config["postgres_citus_master_port"],
    postgres_citus_master_user=\
      json_cluster_config["postgres_citus_master_user"],
    postgres_citus_master_password=\
      json_cluster_config["postgres_citus_master_password"],
    deployment_master=json_cluster_config["deployment_master"],
    deployment_image_registry_port=\
      json_cluster_config["deployment_image_registry_port"],
    git_repo=json_cluster_config["git_repo"])

  json_build_targets = json_obj["build_targets"]
  build_targets: List[BuildTarget] = list()
  for json_build_target in json_build_targets:
    build_targets.append(
      BuildTarget(name=json_build_target["name"],
                  docker_template=json_build_target["docker_template"],
                  pushable=bool(json_build_target["pushable"]),
                  open_ports=json_build_target["open_ports"]))

  return node_configs, cluster_config, build_targets

