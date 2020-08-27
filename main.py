from script.config import NodeConfig
from script.peering import AddPeers

if __name__ == "__main__":
    local_node = NodeConfig(
        name="localhost",
        user="davis",
        location="127.0.0.1",
        ssh_password="320100")
    AddPeers(node=local_node, peers=[local_node])
