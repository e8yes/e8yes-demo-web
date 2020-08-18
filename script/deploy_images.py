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
                 home_mount_point: str,
                 root_mount_point: str,
                 targets: List[BuildTarget], 
                 nodes: List[NodeConfig]) -> None:
  for node in nodes:
    print("Killing running containers...")
    RunSingleCommandInNode(
      node=node,
      command="sudo docker kill `sudo docker container ls -q -a`")

    print("Removing running containers...")
    RunSingleCommandInNode(
      node=node,
      command="sudo docker rm `sudo docker container ls -q -a`")
    
    # Starting the registry back. TODO: Avoid this hack.
    if node.name == deployment_node.name:
      RunSingleCommandInNode(
        node=node, 
        command="sudo docker run -d -p {0}:{0} --restart=always --name=registry registry:latest"
          .format(docker_registry_port))

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
        command="sudo docker run --restart always {0} -v {1}:/host/home -v {2}:/host/root -d -t {2}:latest"
          .format(PortPublishingCommandArgs(target.open_ports),
                  home_mount_point,
                  root_mount_point,
                  image_name))
