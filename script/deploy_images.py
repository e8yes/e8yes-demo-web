from typing import List
from script.config import NodeConfig
from script.config import BuildTarget
from script.run_bash_script import RunSingleCommandInNode
from script.template_instantiator import BuildTargetImageName

def PortPublishingCommandArgs(ports: List[int]) -> str:
  args = ""
  for port in ports:
    args += "-p {0}:{0} ".format(port)
  return args

def DeployImages(deployment_node: NodeConfig,
                 docker_registry_port: int,
                 targets: List[BuildTarget], 
                 nodes: List[NodeConfig]) -> None:
  for node in nodes:
    for target in targets:
      if not target.pushable:
        continue
      image_name = BuildTargetImageName(
        deployment_node=deployment_node,
        docker_registry_port=docker_registry_port,
        target=target)
      print("Pulling image", image_name, "in node", node)
      RunSingleCommandInNode(
        node=node,
        command="sudo docker pull {0}:latest".format(image_name))
      print("Running image", image_name, "in node", node)
      RunSingleCommandInNode(
        node=node,
        command="sudo docker run {0} -d -t {1}:latest"
          .format(PortPublishingCommandArgs(target.open_ports),
                  image_name))
