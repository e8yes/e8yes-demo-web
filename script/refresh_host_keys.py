import os
import subprocess
from pathlib import Path
from typing import List
from script.config import NodeConfig

def RefreshHostKeys(nodes: List[NodeConfig]):
  known_hosts_path = str(Path.home()) + "/.ssh/known_hosts"

  if os.path.exists(known_hosts_path):
    os.remove(known_hosts_path)
  for node in nodes:
    host_key = subprocess.check_output(["ssh-keyscan", node.location])[:-1]\
      .decode("utf-8")
    with open(known_hosts_path, "a") as known_hosts:
      known_hosts.write(host_key)
