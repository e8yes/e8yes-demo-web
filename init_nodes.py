import subprocess

from script.config import NodeConfig
from script.config import ClusterConfig
from script.config import LoadSourceOfTruths
from script.run_bash_script import RunScriptInNode
from script.run_bash_script import RunSingleCommandInNode
from script.run_bash_script import ToSingleLineString
from script.refresh_host_keys import RefreshHostKeys

def InstallPackagesForNode(node_config: NodeConfig, script_file_path: str):
  RunScriptInNode(node=node_config, script_file_path=script_file_path)

def ClearDockerImages(node: NodeConfig):
  RunSingleCommandInNode(
    node=node, command="sudo docker stop `sudo docker ps -a -q`")
  RunSingleCommandInNode(
    node=node, command="sudo docker rm --force `sudo docker ps -a -q`")
  RunSingleCommandInNode(
    node=node, command="sudo docker rmi --force `sudo docker images -q`")                         

def SetUpKubernetesMasterNode(master_node_config: NodeConfig):
  # Generate token and certificate.
  token = ToSingleLineString(
        RunSingleCommandInNode(node=master_node_config, 
                               command="kubeadm token generate",
                               retrieve_output=True))
  certificate = ToSingleLineString(
        RunSingleCommandInNode(node=master_node_config, 
                               command="kubeadm alpha certs certificate-key",
                               retrieve_output=True))

  # Reset node and initialize it to a master node.
  RunSingleCommandInNode(node=master_node_config, 
                         command="echo y | sudo kubeadm reset")
  init_command="sudo kubeadm init --token={0} --certificate-key={1}"\
    .format(token, certificate)
  RunSingleCommandInNode(node=master_node_config, command=init_command)
  RunSingleCommandInNode(node=master_node_config, 
                         command="mkdir -p $HOME/.kube")
  RunSingleCommandInNode(node=master_node_config, 
                         command="sudo cp -f /etc/kubernetes/admin.conf $HOME/.kube/config")
  RunSingleCommandInNode(node=master_node_config, 
                         command="sudo chown $(id -u):$(id -g) $HOME/.kube/config")

  return token, certificate

def SetUpKubernetesWorkerNode(worker_node_config: NodeConfig, 
                              master_node_config: NodeConfig,
                              token: str,
                              certificate: str):
  # Reset node and join it to the master node.
  RunSingleCommandInNode(node=worker_node_config, 
                         command="echo y | sudo kubeadm reset")
  join_command="sudo kubeadm join {0}:6443 --token={1} --certificate-key={2} --discovery-token-unsafe-skip-ca-verification"\
    .format(master_node_config.location, token, certificate)
  RunSingleCommandInNode(node=worker_node_config, command=join_command)

def SetUpKubernetesMasterNodeCni(master_node_config: NodeConfig):
  RunSingleCommandInNode(node=master_node_config, 
                         command="kubectl apply -f \"https://cloud.weave.works/k8s/net?k8s-version=$(kubectl version | base64 | tr -d '\\n')\"")

def SetUpDockerRegistry(deployment_node_config: NodeConfig,
                        docker_registry_port: str):
  RunSingleCommandInNode(
    node=deployment_node_config, 
    command="sudo docker run -d -p {0}:{0} --restart=always --name=registry registry:latest"
      .format(docker_registry_port))

def PrepareNodeForDockerRegistry(node: NodeConfig, 
                                 deployment_node: NodeConfig,
                                 docker_registry_port: str):
  update_daemon_config_cmd = \
    "echo '{{ \"insecure-registries\":[\"{0}:{1}\"] }}' | sudo tee /etc/docker/daemon.json"\
      .format(deployment_node.location, docker_registry_port)
  RunSingleCommandInNode(node=node, command=update_daemon_config_cmd)
  RunSingleCommandInNode(node=node, command="sudo systemctl restart docker")

if __name__ == "__main__":
  node_configs, cluster_config, _ = LoadSourceOfTruths(
    config_file_path="source_of_truths.json")

  print("Refreshing host keys...")
  RefreshHostKeys(node_configs.values())

  for node_name, node_config in node_configs.items():
    print("Installing packages for node", node_name, node_config)
    InstallPackagesForNode(node_config=node_config, 
                           script_file_path="script/install_pkgs_for_node.sh")
  
  postgres_node = node_configs[cluster_config.postgres_citus_master]
  InstallPackagesForNode(node_config=postgres_node, 
                         script_file_path="script/install_pkgs_for_db_node.sh")
  
  for node_name, node_config in node_configs.items():
    print("Clearing docker images for node", node_name, node_config)
    ClearDockerImages(node=node_config)

  kube_master_node = node_configs[cluster_config.kubernetes_master]
  print("Setting up a kubernetes master node at", kube_master_node)
  kube_token, kube_certificate = SetUpKubernetesMasterNode(
    master_node_config=kube_master_node)

  print("Setting up kubernetes worker nodes")
  for node_name, node_config in node_configs.items():
    if node_name == cluster_config.kubernetes_master:
      continue

    print("Setting up kubernetes worker node", node_config)
    SetUpKubernetesWorkerNode(worker_node_config=node_config,
                              master_node_config=kube_master_node,
                              token=kube_token,
                              certificate=kube_certificate)

  print("Setting up kubernetes container network interface")
  SetUpKubernetesMasterNodeCni(master_node_config=kube_master_node)

  deployment_node = node_configs[cluster_config.deployment_master]
  print("Setting up docker image registry on ", deployment_node)
  SetUpDockerRegistry(
    deployment_node_config=deployment_node,
    docker_registry_port=cluster_config.deployment_image_registry_port)

  print("Preparing nodes for docker registry")
  for node_config in node_configs.values():
    print("Preparing", node_config, "for docker registry")
    PrepareNodeForDockerRegistry(
      node=node_config,
      deployment_node=deployment_node,
      docker_registry_port=cluster_config.deployment_image_registry_port)
