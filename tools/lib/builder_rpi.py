import os.path

BUILDER_RPI_CONFIG_FILE = "./logs/builder_rpi.conf"

def GetBuilderRpi() -> str:
    f = open(BUILDER_RPI_CONFIG_FILE, "r")
    fields = f.readlines()
    f.close()

    assert(len(fields) == 3)
    return fields[0][:-1], \
           fields[1][:-1], \
           fields[2]

def SetBuilderRpi(target: str, ssh_user: str, sshkey_path: str) -> bool:
    if os.path.isfile(BUILDER_RPI_CONFIG_FILE):
        previous_target, _, _ = GetBuilderRpi()
        if target != previous_target:
            return False
    
    f = open(BUILDER_RPI_CONFIG_FILE, "w")
    f.truncate(0)
    f.writelines([
        target + "\n",
        ssh_user + "\n",
        sshkey_path])
    f.close()

    return True
