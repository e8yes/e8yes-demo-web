from typing import List
from os.path import dirname
from script.config import BuildTarget
from script.config import NodeConfig
from script.run_bash_script import RunSingleCommandInNode
from script.template_instantiator import TemplateInstantiator
from script.template_instantiator import BuildTargetImageName
  

def BuildAndPushTargetImages(targets: List[BuildTarget], 
                             instantiator: TemplateInstantiator,
                             code_repo_base_path: str,
                             deployment_node: NodeConfig,
                             docker_registry_port: int):
  for target in targets:
    template_file_path = "{0}/{1}".format(code_repo_base_path, 
                                          target.docker_template)
    docker_file_path = instantiator.Instantiate(
      template_file_path=template_file_path,
      target_node=deployment_node)

    target_image_name = None
    if not target.pushable:
      target_image_name = target.name
    else:
      target_image_name = BuildTargetImageName(
        deployment_node=deployment_node,
        docker_registry_port=docker_registry_port,
        target=target)

    build_cmd ="sudo docker build -t=\"{0}\" {1}"\
        .format(target_image_name, dirname(docker_file_path))
    RunSingleCommandInNode(node=deployment_node, command=build_cmd)
    
    if target.pushable:
      RunSingleCommandInNode(
        node=deployment_node, 
        command="sudo docker push {0}".format(target_image_name))
