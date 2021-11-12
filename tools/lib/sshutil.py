import subprocess
from os.path import basename

def CopyFileToRpiHome(file_path: str,
                      target: str,
                      ssh_user: str,
                      sshkey_path: str):
    file_name = basename(file_path)

    print("Copy {0} to ~/{1}".format(file_path, file_name))

    subprocess.call(["scp", "-i", sshkey_path, file_path, 
                     ssh_user + "@" + target + ":~/" + file_name])

def CopyFileToRpi(file_path: str,
                  destination: str,
                  target: str,
                  ssh_user: str,
                  sshkey_path: str):
    print("Copy {0} to {1}".format(file_path, destination))

    subprocess.call(["scp", "-i", sshkey_path, file_path, 
                    ssh_user + "@" + target + ":" + destination])

def RunCommandOnRpi(command: str, 
                    target: str,
                    ssh_user: str,
                    sshkey_path: str):
    print(command)

    subprocess.call([
        "ssh",
        "-i", sshkey_path,
        ssh_user + "@" + target,
        command])

def RunProgramOnRpi(program_file_path: str,
                    target: str,
                    ssh_user: str,
                    sshkey_path: str):
    CopyFileToRpiHome(
        file_path=program_file_path,
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

    RunCommandOnRpi(
        command="chmod +x ~/" + basename(program_file_path),
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)

    RunCommandOnRpi(
        command="~/" + basename(program_file_path),
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
    
    RunCommandOnRpi(
        command="rm ~/" + basename(program_file_path),
        target=target,
        ssh_user=ssh_user,
        sshkey_path=sshkey_path)
