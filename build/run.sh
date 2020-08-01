rm -rf bin
mkdir -p bin

find . -name *.so -exec cp -f {} bin/ \;
find . -name *.1 -exec cp -f {} bin/ \;
find . -name *main -exec cp -f {} bin/ \;

pushd bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
./demoweb_service_main \
	--demoweb_db_host_name=localhost \
	--demoweb_db_port=5432 \
	--demoweb_db_user_name=postgres \
	--demoweb_db_password=password
popd

