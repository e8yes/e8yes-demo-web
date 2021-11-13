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
import random
import subprocess
import time
import _thread

SYSTEM_UPDATER_PORT = 1027
SYSTEM_UPDATER_LOG = "./system_updater.log"
SYSTEM_ONLINE_MIN_SECS = 12*60*60
SYSTEM_ONLINE_MAX_SECS = 24*60*60
REVISION_STAGGERING_MIN_CYCLES = 1
REVISION_STAGGERING_MIN_CYCLES = 5

def UpdateSystem(system_name: str):
    subprocess.call(["sudo", "./update-demoweb.sh", system_name])
    subprocess.call(["sudo", "apt", "update"])
    subprocess.call(["sudo", "apt", "upgrade", "-y"])

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

            staggering_cycles = random.randint(
                REVISION_STAGGERING_MIN_CYCLES, REVISION_STAGGERING_MIN_CYCLES)
            for i in range(staggering_cycles):
                time.sleep(request.status_update_interval_millis/1000)

                yield ApplyClusterMapRevisionResponse(
                    successful=False,
                    num_objects_processed=i + 1,
                    total_num_objects=staggering_cycles)

            yield ApplyClusterMapRevisionResponse(successful=True)
            return None
        
def Track(system_name: str):
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=1))
    server.add_insecure_port("[::]:{0}".format(SYSTEM_UPDATER_PORT))
    add_ResourceWorkerServiceServicer_to_server(
        SystemUpdaterService(system_name), server)

    server.start()

    online_duration = random.randint(
        SYSTEM_ONLINE_MIN_SECS, SYSTEM_ONLINE_MAX_SECS)
    time.sleep(online_duration)

    subprocess.call(["sudo", "shutdown", "--reboot", "now"])

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
