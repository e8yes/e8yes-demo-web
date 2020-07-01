from typing import List
from script.config import NodeConfig
from script.config import BuildTarget
from script.run_bash_script import RunSingleCommandInNode
from script.run_bash_script import ToSingleLineString
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

      print("Checking running image", image_name, "in node", node)
      container_id = ToSingleLineString(
        RunSingleCommandInNode(
          node=node,
          command="sudo docker ps -a -q --filter ancestor={0} --format=\"{{.ID}}\""
            .format(image_name),
          retrieve_output=True))
      if container_id:
        print("Killing the container", container_id, "that runs the image", image_name)
        RunSingleCommandInNode(
          node=node,
          command="sudo docker kill {0}".format(container_id))
        
        print("Removing the container", container_id)
        RunSingleCommandInNode(
          node=node,
          command="sudo docker rm {0}".format(container_id))
      else:
        print("The image", image_name, " is not running.")

      print("Running image", image_name, "in node", node)
      RunSingleCommandInNode(
        node=node,
        command="sudo docker run {0} -d -t {1}:latest"
          .format(PortPublishingCommandArgs(target.open_ports),
                  image_name))
