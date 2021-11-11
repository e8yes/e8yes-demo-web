import subprocess
from os.path import basename

def CopyFileToRpiHome(file_path: str,
                      target: str,
                      ssh_user: str,
                      sshkey_path: str):
  file_name = basename(file_path)
  subprocess.call(["scp", "-i", sshkey_path, file_path, 
                   ssh_user + "@" + target + ":~/" + file_name])

def RunCommandOnRpi(command: str, 
                    target: str,
                    ssh_user: str,
                    sshkey_path: str):
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
