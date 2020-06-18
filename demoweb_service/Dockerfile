FROM demoweb_dev_env:latest

# Run the service.
WORKDIR /home

EXPOSE 50051

ENV LD_LIBRARY_PATH="./bin/"

CMD ["./bin/demowebmain", \
     "--port=50051", \
     "--demoweb_db_host_name=192.168.1.203", \
     "--demoweb_db_port=5432", \
     "--demoweb_db_user_name=postgres", \
     "--demoweb_db_password=password"]

