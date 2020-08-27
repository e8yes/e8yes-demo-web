from typing import List
import grpc
from script.config import NodeConfig
from service_node_state_pb2_grpc import NodeStateServiceStub
from service_node_state_pb2 import AddPeersRequest
from node_pb2 import NodeState

DISTRIBUTOR_PORT=20021

def HostTargetStr(node: NodeConfig) -> str:
    return node.location + ":" + str(DISTRIBUTOR_PORT)

def IpAddressToBytes(ipv4_str: str):
    components = ipv4_str.split(".")
    return bytes([int(components[0]), 
                  int(components[1]), 
                  int(components[2]), 
                  int(components[3])])

def AddPeers(node: NodeConfig, peers: List[NodeConfig]):
    target = HostTargetStr(node=node)
    with grpc.insecure_channel(target) as channel:
        stub = NodeStateServiceStub(channel)

        nodes = list()
        for peer in peers:
            node = NodeState()
            node.name = peer.name
            node.ip_address = IpAddressToBytes(peer.location)

            nodes.append(node)
        
        request = AddPeersRequest(nodes=nodes)
        stub.AddPeers(request=request)
