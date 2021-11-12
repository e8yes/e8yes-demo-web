#!/usr/bin/python3

from concurrent import futures
from threading import Lock
from service_resource_worker_pb2_grpc import add_ResourceWorkerServiceServicer_to_server
from service_resource_worker_pb2_grpc import ResourceWorkerServiceServicer
from service_resource_worker_pb2 import ApplyClusterMapRevisionRequest
from service_resource_worker_pb2 import ApplyClusterMapRevisionResponse

import argparse
import grpc
import os
import subprocess
import time
import _thread

SYSTEM_UPDATER_PORT = 1027
SYSTEM_UPDATER_LOG = "./system_updater.log"

def UpdateSystem(system_name: str):
    subprocess.call(["sudo", "docker", "pull", system_name])
    subprocess.call(["sudo", "apt", "update"])
    subprocess.call(["sudo", "apt", "upgrade", "-y"])
    subprocess.call(["sudo", "shutdown", "--reboot", "+1"])

class SystemUpdaterService(ResourceWorkerServiceServicer):
    def __init__(self, system_name: str):
        self.system_name = system_name
        self.mu = Lock()

    def ReadVersionEpoch(self) -> int:
        if not os.path.isfile(SYSTEM_UPDATER_LOG):
            f = open(SYSTEM_UPDATER_LOG, "w")
            f.write(str(0))
            f.close()
            return 0

        f = open(SYSTEM_UPDATER_LOG, "r")
        version_epoch = f.read()
        f.close()

        return int(version_epoch)
    
    def WriteVersionEpoch(self, version_epoch):
        f = open(SYSTEM_UPDATER_LOG, "w")
        f.write(str(version_epoch))
        f.close()
    
    def ApplyClusterMapRevision(self,
                                request: ApplyClusterMapRevisionRequest,
                                context: grpc.ServicerContext) -> ApplyClusterMapRevisionResponse:
        with self.mu:
            current_version_epoch = self.ReadVersionEpoch()
            if request.revision.from_version_epoch != current_version_epoch:
                yield ApplyClusterMapRevisionResponse(
                    successful=False,
                    require_from_version_epoch=current_version_epoch)
                return None
            
            _thread.start_new_thread(UpdateSystem, (self.system_name,))

            self.WriteVersionEpoch(version_epoch=request.revision.to_version_epoch)
            yield ApplyClusterMapRevisionResponse(successful=True)
            return None
        
def Track(system_name: str):
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=1))
    server.add_insecure_port("[::]:{0}".format(SYSTEM_UPDATER_PORT))
    add_ResourceWorkerServiceServicer_to_server(
        SystemUpdaterService(system_name), server)

    server.start()

    while True:
        time.sleep(10)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Pulls and updates the demoweb system and the operating system on cluster revision event.")
    parser.add_argument("--system_name",
        type=str,
        help="Name of the demoweb system to pull.")
    
    args = parser.parse_args()

    if args.system_name is None:
        print("system_name is required.")
        exit(-1)
    
    Track(args.system_name)
