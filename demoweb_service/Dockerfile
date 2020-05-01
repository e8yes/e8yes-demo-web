FROM grpc_service:latest

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

