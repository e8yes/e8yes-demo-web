from typing import Tuple
import datetime
import time
import json
import logging
import os
import requests
import subprocess
import signal

def UpdateDns(new_ip: str, 
              dyndns_user_name: str,
              dyndns_password: str) -> bool:
    try:
        requests.post("https://{0}:{1}@domains.google.com/nic/update?hostname=home.e8yes.org&myip={2}".format(dyndns_user_name,
                              dyndns_password,
                              new_ip), 
                      timeout=15)
        return True
    except:
        return False

def GetLatestIp() -> str:
    try:
        resp = requests.get("https://icanhazip.com", timeout=15)
        resp.raise_for_status()
        return str(resp.text[:-1])
    except:
        return None

def ConnectToProxy(ovpn_config_file: str,
                   ovpn_credential: str) -> subprocess.Popen:
    proc = subprocess.Popen(
            "openvpn --config /host/home/{0} --auth-user-pass /host/home/{1}"\
                    .format(ovpn_config_file, ovpn_credential), 
            close_fds=True,
            shell=True,
            preexec_fn=os.setsid)
    time.sleep(5)
    return proc

def NewCycle(current_ip: str, config) -> Tuple[str, bool]:
    new_ip = GetLatestIp()
    if new_ip is None:
        logging.error("Fails to obtain the latest public IP.")
        return current_ip, False

    if new_ip == current_ip:
        logging.info("No change applied")
        return current_ip, True

    logging.info("Detected IP change: old_ip={0}, new_ip={1}".format(current_ip, new_ip))

    proxy_subproc = None
    if config["proxy"]["enable_vpn"]:
        logging.info("Connecting to the OpenVPN proxy...")
        proxy_subproc = ConnectToProxy(config["proxy"]["ovpn_config_file_name"],
                                       config["proxy"]["ovpn_credential_file_name"])

    result = False 
    for i in range(10):
        result |= UpdateDns(new_ip, 
                            config["user_name"], 
                            config["password"])
        if result:
            logging.info("{0}th DNS update attempt succeeded.".format(i + 1))
            break

        logging.error("{0}th DNS update attempt failed.".format(i + 1))

    if proxy_subproc is not None:
        logging.info("Killing the proxy subprocess {0}..."\
                .format(proxy_subproc.pid))
        os.killpg(proxy_subproc.pid, signal.SIGTERM)

    if not result:
        return current_ip, False

    logging.info("Updated IP to {0}".format(new_ip))
    return new_ip, True

def LoadConfigFile(config_file: str):
    with open(config_file, "r") as content:
        return json.loads(content.read())

if __name__ == "__main__":
    today = datetime.date.today()

    logging.basicConfig(filename="/host/home/dyn-dns-{0}.log".format(str(today)), 
                        level=logging.INFO, 
                        format="%(asctime)s\t%(levelname)s\t%(message)s")

    config = LoadConfigFile("/host/home/dyn-dns.json")

    current_ip = str()
    while True:
        current_ip, ok = NewCycle(current_ip, config)
        if not ok:
            time.sleep(10)
        else:
            time.sleep(100)
