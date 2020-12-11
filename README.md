# Service Location
Deployed at HTTP/2 home.e8yes.org:28000; HTTP/1.4 home.e8yes.org:18000

## Raspberry PI cluster
![Alt text](images/pi_cluster.jpg?raw=true "PI cluster")

## Setup Requirements
- OS: Ubuntu 20.04 LTS

Run `./install_dev_pkgs.sh` in the top-level directory to initialize the development environment. To build and test the back-end services, run the script `./compile_proto.sh` to generate protocol buffer code, then run the script `./run_tests.sh` to initiate the build and test process. Run the script `./gitpush.sh` to push commits to the github repository. This script will try to build and test your commits before pushing to the repository.

### Client
 - [Android](https://github.com/e8yes/e8yes-chat-android)
 - [iOS](https://github.com/e8yes/e8yes-chat-ios)

### Note
`legacy-jee` branch contains a deprecated Java-EE version and a web client.

Created by [Chifeng Wen](https://www.linkedin.com/in/chifeng-wen-392b41ba/), [Zhaonian Luan](https://www.linkedin.com/in/zhaonianluan/).
