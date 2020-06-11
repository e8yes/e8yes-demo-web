dlls=($(find . -regextype sed -regex ".*\(demoweb_service/demoweb\|postgres/query_runner\|common/container\|keygen\)"))
for dll in ${dlls[@]}
do
	echo $dll
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$dll
done

demoweb_service/demoweb/demowebmain --demoweb_db_host_name=localhost --demoweb_db_port=5432 --demoweb_db_user_name=postgres --demoweb_db_password=password

