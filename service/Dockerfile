FROM ubuntu:19.10

# Install packages.
RUN apt update
RUN apt install -y build-essential libpqxx-dev libgrpc++-dev qt5-default

# Copy source code.
WORKDIR /home

RUN mkdir demoweb_src
ADD . /home/demoweb_src
RUN ls -lh

# Build project.
RUN mkdir build
WORKDIR /home/build

RUN qmake ../demoweb_src/demoweb.pro
RUN make -j `nproc --all`

# Run the service.
WORKDIR /home

EXPOSE 50051 

CMD ["./build/demoweb/demowebmain", "--port=50051"]

