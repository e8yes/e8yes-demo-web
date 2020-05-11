import subprocess

def GetHostIp(location: str) -> str:
  output = subprocess.check_output(["getent", "hosts", location])
  output = output[:-1].decode("utf-8")
  parts = output.split(" ")
  return parts[0]
