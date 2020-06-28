from typing import List
from script.config import BuildTarget
from script.config import NodeConfig
from script.run_bash_script import RunSingleCommandInNode
from script.template_instantiator import TemplateInstantiator
from script.template_instantiator import BuildTargetImageName
  

def BuildAndPushTargetImages(targets: List[BuildTarget], 
                             instantiator: TemplateInstantiator,
                             code_repo_base_path: str,
                             deployment_node: NodeConfig,
                             docker_registry_port: str):
  for target in targets:
    template_file_path = "{0}/{1}".format(code_repo_base_path, 
                                          target.docker_template)
    docker_file_path = instantiator.Instantiate(
      template_file_path=template_file_path,
      target_node=deployment_node)

    target_image_name = BuildTargetImageName(
      deployment_node=deployment_node,
      docker_registry_port=docker_registry_port,
      target=target)
    RunSingleCommandInNode(
      node=deployment_node,
      command="cd {0} sudo docker build -t={1}"
        .format(docker_file_path, target_image_name))
    
    if target.pushable:
      RunSingleCommandInNode(
        node=deployment_node, 
        command="sudo docker push {0}".format(target_image_name))
