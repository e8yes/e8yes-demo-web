from typing import List
import argparse
import os
import re
import subprocess
    
def GenerateProtobufCode():
    subprocess.call(["lib/compile-proto.sh"])

def PushPostgresSchema():
    subprocess.call(["lib/push-schema.sh"])

def BuildBinaries():
    subprocess.call(["lib/build-for-devel.sh"])

def GrabTestBinaries() -> List[str]:
    test_binaries = list()

    test_file_pattern = re.compile("_test.*")
    for file in os.scandir("../build/bin"):
        if test_file_pattern.match(file.name):
            test_binaries.append(file.name)
    
    return test_binaries

def RunTests():
    test_binaries = GrabTestBinaries()

    for test_binary in test_binaries:
        ret_code = subprocess.call(["lib/run-test.sh", test_binary])
        if ret_code != 0:
            print("Test {0} just failed.".format(test_binary))
            os.abort()
    
    print("Test suite ({0} collections) has passed.".\
        format(len(test_binaries)))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Build and test the demoweb system locally.")
    parser.add_argument("--regen_proto_code",
        type=bool,
        help="Whether or not to regenerate the protobuf code before building.")
    
    args = parser.parse_args()

    if args.regen_proto_code:
        GenerateProtobufCode()

    BuildBinaries()
    PushPostgresSchema()
    RunTests()
