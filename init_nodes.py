import subprocess

from script.parse_node_config import NodeConfig
from script.parse_node_config import ClusterConfig
from script.parse_node_config import ReadNodeConfig
from script.run_bash_script import RunScriptInNode
from script.run_bash_script import RunSingleCommandInNode
from script.run_bash_script import ToSingleLineString

def InstallPackagesForNode(node_config: NodeConfig, script_file_path: str):
  RunScriptInNode(node=node_config, script_file_path=script_file_path)

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
  init_command="sudo kubeadm init --pod-network-cidr=\"192.168.0.0/16\" --token={0} --certificate-key={1}"\
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
  join_command="sudo kubeadm join `getent hosts {0} | awk '{{ print $1 }}'`:6443 --token={1} --certificate-key={2} --discovery-token-unsafe-skip-ca-verification"\
    .format(master_node_config.location, token, certificate)
  RunSingleCommandInNode(node=worker_node_config, command=join_command)

def SetUpKubernetesMasterNodeCni(master_node_config: NodeConfig):
  RunSingleCommandInNode(node=master_node_config, 
                         command="kubectl apply -f \"https://cloud.weave.works/k8s/net?k8s-version=$(kubectl version | base64 | tr -d '\\n')\"")
  

if __name__ == "__main__":
  node_configs, cluster_config = ReadNodeConfig(
    config_file_path="nodes_configuration.json")
  for node_name, node_config in node_configs.items():
    print("Installing packages for node", node_name, node_config)
    InstallPackagesForNode(node_config=node_config, 
                           script_file_path="script/install_pkgs_for_node.sh")

  kube_master_node = node_configs[cluster_config.kubernetes_master]
  print("Setting up a kubernetes master node at", kube_master_node)
  kube_token, kube_certificate = SetUpKubernetesMasterNode(
    master_node_config=kube_master_node)

  print("Setting up kubernetes worker nodes");
  for node_name, node_config in node_configs.items():
    if node_name == cluster_config.kubernetes_master:
      continue

    print("Setting up kubernetes worker node", node_config);
    SetUpKubernetesWorkerNode(worker_node_config=node_config,
                              master_node_config=kube_master_node,
                              token=kube_token,
                              certificate=kube_certificate)

  print("Setting up kubernetes container network interface")
  SetUpKubernetesMasterNodeCni(master_node_config=kube_master_node)

