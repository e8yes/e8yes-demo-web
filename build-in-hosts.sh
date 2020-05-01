#!/bin/bash

set +e

function prepare_directory {
    host_name=$1
    password=$2

    sshpass -p $password ssh $host_name "mkdir -p demoweb" < /dev/null
}

function copy_file {
    host_name=$1
    password=$2
    file_path=$3

    echo "Copying $file_path..."
    sshpass -p $password scp -v -r $file_path "$host_name:~/demoweb/$file_path" < /dev/null
}

function copy_src {
    host_name=$1
    password=$2

    copy_file $host_name $password service
    copy_file $host_name $password postgres
    copy_file $host_name $password build.sh
    copy_file $host_name $password install-build-tools.sh
}

function build_project {
    host_name=$1
    password=$2

    sshpass -p $password ssh $host_name "cd demoweb && ./build.sh" < /dev/null
}

function upload_and_build {
	config_file=$1
	
	while read -r line
	do
		if [[ "$line" = "#"* || -z "$line" ]]
		then
			continue
		fi

		IFS=':' read -r -a splits <<< "$line"
		user_and_host=${splits[0]}
		password=${splits[1]}

		echo "================================================"
		echo "Upload and building in host $user_and_host..."
		
		# Copy source code to the host machines.
		prepare_directory $user_and_host $password
		copy_src $user_and_host $password

		# Build the entire project.
		build_project $user_and_host $password
		echo "================================================"
	done < $config_file

	echo "All done."
}


sudo apt install sshpass

HOSTS_CONFIG_FILE="./host-machines.conf"

upload_and_build $HOSTS_CONFIG_FILE

