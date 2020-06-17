FROM ubuntu:19.10

# Install required packages.
WORKDIR /home/demoweb_src
ENV DEBIAN_FRONTEND=noninteractive
RUN apt update
RUN apt install -y haveged
RUN apt install -y build-essential
RUN echo 13 | apt install -y postgresql
RUN apt install -y libpqxx-dev
RUN apt install -y libprotobuf-dev
RUN apt install -y protobuf-compiler
RUN apt install -y libgrpc++-dev
RUN apt install -y libcrypto++-dev
RUN apt install -y libcrypt2-dev
RUN apt install -y qt5-default

# Copy source code.
WORKDIR /home

RUN mkdir -p demoweb_src
ADD . /home/demoweb_src
RUN ls -lh

# Build project.
WORKDIR /home
RUN mkdir -p build
WORKDIR /home/build
RUN qmake ../demoweb_src/e8.pro
RUN make -j `nproc --all`

# Copy out the binaries.
WORKDIR /home
RUN mkdir -p bin
RUN find build -name *.so -exec cp -f {} ./bin \;
RUN find build -name *.1 -exec cp -f {} ./bin \;
RUN find build -name *main -exec cp -f {} ./bin \;
