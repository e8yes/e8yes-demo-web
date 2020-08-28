from typing import Dict
from typing import List
import grpc
from script.config import NodeConfig
from script.config import TopologyConfig
from service_node_state_pb2_grpc import NodeStateServiceStub
from service_node_state_pb2 import AddPeersRequest
from node_pb2 import NodeState
from node_pb2 import NDS_READY
from node_pb2 import NDF_DISTRIBUTOR
from node_pb2 import NDF_FILE_STORE
from node_pb2 import NDF_MESSAGE_QUEUE
from node_pb2 import NDF_TASK_EXECUTOR
from node_pb2 import NDF_RELATIONAL_STORE
from node_pb2 import NDF_LOAD_BALANCER
from node_pb2 import NDF_IMAGE_BUILDER
from node_pb2 import NodeFunction

DISTRIBUTOR_PORT=20021

def HostTargetStr(node: NodeConfig) -> str:
    return node.location + ":" + str(DISTRIBUTOR_PORT)

def IpAddressToBytes(ipv4_str: str):
    components = ipv4_str.split(".")
    return bytes([int(components[0]), 
                  int(components[1]), 
                  int(components[2]), 
                  int(components[3])])

def ToEnum(node_functions: List[str]) -> List[NodeFunction]:
    result = list()
    for node_func in node_functions:
        if node_func == "distributor":
            result.append(NDF_DISTRIBUTOR)
        elif node_func == "file_store":
            result.append(NDF_FILE_STORE)
        elif node_func == "message_queue":
            result.append(NDF_MESSAGE_QUEUE)
        elif node_func == "task_executor":
            result.append(NDF_TASK_EXECUTOR)
        elif node_func == "relational_store":
            result.append(NDF_RELATIONAL_STORE)
        elif node_func == "load_balancer":
            result.append(NDF_LOAD_BALANCER)
        elif node_func == "image_builder":
            result.append(NDF_IMAGE_BUILDER)
    return result


def AddPeers(node: NodeConfig, peers: List[NodeConfig]):
    target = HostTargetStr(node=node)
    with grpc.insecure_channel(target) as channel:
        stub = NodeStateServiceStub(channel)

        nodes = list()
        for peer in peers:
            node = NodeState(
                name=peer.name,
                ip_address=IpAddressToBytes(peer.location),
                status=NDS_READY,
                functions=ToEnum(node.functions))

            nodes.append(node)
        
        request = AddPeersRequest(nodes=nodes)
        stub.AddPeers(request=request)

def ApplyTopology(nodes: Dict[str, NodeConfig], 
                  topology: TopologyConfig):
    for entry in topology.connections.items():
        src = entry[0]
        dsts = entry[1]

        src_node = nodes[src]
        dst_nodes = list()
        for dst in dsts:
            dst_nodes.append(nodes[dst])

        print("Adding connection from=", src_node, "|to=", dst_nodes)
        AddPeers(node=src_node, peers=dst_nodes)
