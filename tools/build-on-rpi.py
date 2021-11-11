#!/bin/python3

from lib.builder_rpi import GetBuilderRpi
from lib.builder_rpi import SetBuilderRpi
from lib.sshutil import CopyFileToRpiHome
from lib.sshutil import RunCommandOnRpi
from lib.sshutil import RunProgramOnRpi
import argparse

def SetUpDocker(target: str, ssh_user: str, sshkey_path: str):
    RunCommandOnRpi(
        command="sudo docker pull registry:latest",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    RunCommandOnRpi(
        command="sudo docker container rm --force "\
                "`sudo docker container ls -q --filter name=registry`",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    RunCommandOnRpi(
        command="sudo docker run -d -p 5000:5000 --restart=always "\
                "--name=registry registry:latest",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    
    update_daemon_config_cmd = \
        "echo '{{ \"insecure-registries\":[\"{0}:5000\"] }}' | sudo tee /etc/docker/daemon.json"\
            .format(target)
    RunCommandOnRpi(
        command=update_daemon_config_cmd,
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

    RunCommandOnRpi(
        command="sudo systemctl restart docker",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

def InstallBuildSystemOnRpi():
    target, ssh_user, sshkey_path = GetBuilderRpi()

    RunProgramOnRpi(
        program_file_path="./lib/install-rpi-build-tools.sh",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    
    SetUpDocker(target, ssh_user, sshkey_path)

def BuildDockerImagesOnRpi():
    target, ssh_user, sshkey_path = GetBuilderRpi()

    CopyFileToRpiHome(
        file_path="../docker/demoweb",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

    RunCommandOnRpi(
        command="sudo docker build --tag={0}:5000/demoweb --file=demoweb ."\
            .format(target),
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    RunCommandOnRpi(
        command="rm ./demoweb",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    RunCommandOnRpi(
        command="sudo docker push {0}:5000/demoweb"\
            .format(target),
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Build docker images on a Raspberry PI machine.")
    parser.add_argument("--target",
        type=str,
        help="Specifies the IP address of the target machine on the first run. "\
             "In the subsequent runs, this argument isn't needed.")
    parser.add_argument("--ssh_user",
        type=str,
        help="Sets or updates the user to used when SSHing to the target machine. "\
             "This is required when the target argument is used.")
    parser.add_argument("--sshkey_path",
        type=str,
        help="Sets or updates the file path to the ssh key of the target machine. "\
             "This is required when the target argument is used.")
    
    args = parser.parse_args()

    if args.target is not None:
        if args.ssh_user is None:
            print("ssh_user is required when target is used.")
            exit(-1)

        if args.sshkey_path is None:
            print("sshkey_path is required when target is used.")
            exit(-1)

        if not SetBuilderRpi(args.target, args.ssh_user, args.sshkey_path):
            print("Can't set a different target. Target has already been set to {0}."\
                .format(str(GetBuilderRpi())))
            exit(-1)

    InstallBuildSystemOnRpi()
    BuildDockerImagesOnRpi()
