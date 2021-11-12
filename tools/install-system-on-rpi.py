#!/bin/python3

from lib.builder_rpi import GetBuilderRpi
from lib.sshutil import CopyFileToRpi
from lib.sshutil import RunCommandOnRpi
from lib.sshutil import RunProgramOnRpi

import argparse

def SetUpDocker(target: str, ssh_user: str, sshkey_path: str):
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

def InstallEssentialPackages(target: str, ssh_user: str, sshkey_path: str):
    RunProgramOnRpi(
        program_file_path="./lib/install-rpi-build-tools.sh",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

    SetUpDocker(target=target, ssh_user=ssh_user, sshkey_path=sshkey_path)

def DownloadSystem(target: str, ssh_user: str, sshkey_path: str) -> str:
    builder_rpi_address, _, _ = GetBuilderRpi()
    system_name = "{0}:5000/demoweb".format(builder_rpi_address)

    RunCommandOnRpi(
        command="sudo docker pull {0}".format(system_name),
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    
    return system_name

def RunSystem(system_name: str, target: str, ssh_user: str, sshkey_path: str):
    RunCommandOnRpi(
        command="mkdir -p ~/demoweb-data/task_standard_output",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

    RunCommandOnRpi(
        command="sudo docker container rm --force "
                "$(sudo docker container ls -q --filter name=demoweb_system)",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    RunCommandOnRpi(
        command="sudo docker run --name=demoweb_system "\
                "--restart=always "\
                "--network=host "\
                "--hostname=$(hostname) "\
                "--volume=/home/{0}/demoweb-data:/host/home "\
                "--detach "\
                "{1}".format(ssh_user, system_name),
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

def UploadCode(file: str,
               destination: str,
               target: str,
               ssh_user: str,
               sshkey_path: str):
    CopyFileToRpi(
        file_path=file,
        destination=destination,
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    RunCommandOnRpi(
        command="chmod +x {0}".format(destination),
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

def UploadSystemUpdaterCode(target: str, ssh_user: str, sshkey_path: str):
    RunCommandOnRpi(
        command="mkdir -p ~/demoweb-programs",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    RunCommandOnRpi(
        command="mkdir -p ~/demoweb-programs",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

    UploadCode(
        file="./lib/demoweb-system-updater.py",
        destination="~/demoweb-programs/demoweb-system-updater.py",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    UploadCode(
        file="./lib/proto_py/__init__.py",
        destination="~/demoweb-programs",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    UploadCode(
        file="./lib/proto_py/service_resource_worker_pb2_grpc.py",
        destination="~/demoweb-programs",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    UploadCode(
        file="./lib/proto_py/service_resource_worker_pb2.py",
        destination="~/demoweb-programs",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    UploadCode(
        file="./lib/proto_py/cluster_revision_pb2.py",
        destination="~/demoweb-programs",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    UploadCode(
        file="./lib/proto_py/cluster_pb2.py",
        destination="~/demoweb-programs",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    UploadCode(
        file="./lib/proto_py/bucket_pb2.py",
        destination="~/demoweb-programs",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    UploadCode(
        file="./lib/proto_py/machine_pb2.py",
        destination="~/demoweb-programs",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

def InstallSystemUpdater(target: str, ssh_user: str, sshkey_path: str):
    UploadSystemUpdaterCode(
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

    RunCommandOnRpi(
        command="echo '"
                "[Unit]\n"
                "Description=Demoweb system updater service\n"
                "\n"
                "[Service]\n"
                "Type=simple\n"
                "ExecStart='/home/{0}/demoweb-programs/demoweb-system-updater.py --system_name={1}'\n"
                "WorkingDirectory=/home/{2}/demoweb-programs\n"
                "\n"
                "[Install]\n"
                "WantedBy=multi-user.target"
                "' | "
                "sudo tee /etc/systemd/system/demoweb-system-updater.service"\
                    .format(ssh_user, system_name, ssh_user),
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    RunCommandOnRpi(
        command="sudo chmod 644 /etc/systemd/system/demoweb-system-updater.service",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    RunCommandOnRpi(
        command="sudo systemctl enable demoweb-system-updater.service",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

def RebootSystem(target: str, ssh_user: str, sshkey_path: str):
    RunCommandOnRpi(
        command="sudo reboot",
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Install and run the demoweb system on a Raspberry PI machine.")
    parser.add_argument("--target",
        type=str,
        help="IP address of the target Raspberry PI machine.")
    parser.add_argument("--ssh_user",
        type=str,
        help="The user to use while SSHing into the target machine.")
    parser.add_argument("--sshkey_path",
        type=str,
        help="The file path to the target machine's SSH key .")

    args = parser.parse_args()

    if args.target is None:
        print("target is required.")
        exit(-1)

    if args.ssh_user is None:
        print("ssh_user is required.")
        exit(-1)

    if args.sshkey_path is None:
        print("sshkey_path is required.")
        exit(-1)

    InstallEssentialPackages(
        target=args.target,
        ssh_user=args.ssh_user,
        sshkey_path=args.sshkey_path)
    system_name = DownloadSystem(
        target=args.target,
        ssh_user=args.ssh_user,
        sshkey_path=args.sshkey_path)
    RunSystem(
        system_name=system_name,
        target=args.target,
        ssh_user=args.ssh_user,
        sshkey_path=args.sshkey_path)
    InstallSystemUpdater(
        target=args.target,
        ssh_user=args.ssh_user,
        sshkey_path=args.sshkey_path)
    RebootSystem(
        target=args.target,
        ssh_user=args.ssh_user,
        sshkey_path=args.sshkey_path)
