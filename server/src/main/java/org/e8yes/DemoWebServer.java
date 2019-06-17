package org.e8yes;

import io.grpc.Server;
import io.grpc.ServerBuilder;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.e8yes.srvs.AuthService;
import org.e8yes.srvs.EnvironmentContext;
import org.e8yes.srvs.FriendshipService;
import org.e8yes.srvs.InitializerService;
import org.e8yes.srvs.UserService;

/**
 * Adapted from
 * https://github.com/grpc/grpc-java/blob/v1.19.0/examples/src/main/java/io/grpc/examples/helloworld/HelloWorldServer.java
 *
 * @author davis
 */
public class DemoWebServer {

  private static final Logger LOGGER = Logger.getLogger(DemoWebServer.class.getName());

  private Server server;

  private void start() throws IOException {
    /* The port on which the server should run */
    int port = 50051;
    server =
        ServerBuilder.forPort(port)
            .addService(new AuthService())
            .addService(new UserService())
            .addService(new FriendshipService())
            .build()
            .start();
    LOGGER.log(Level.INFO, "Server started, listening on {0}", port);
    Runtime.getRuntime()
        .addShutdownHook(
            new Thread() {
              @Override
              public void run() {
                // Use stderr here since the logger may have been reset by its JVM shutdown hook.
                System.err.println("*** shutting down gRPC server since JVM is shutting down");
                DemoWebServer.this.stop();
                System.err.println("*** server shut down");
              }
            });
  }

  private void stop() {
    if (server != null) {
      server.shutdown();
    }
  }

  /** Await termination on the main thread since the grpc library uses daemon threads. */
  private void blockUntilShutdown() throws InterruptedException {
    if (server != null) {
      server.awaitTermination();
    }
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    final DemoWebServer server = new DemoWebServer();
    server.start();
    InitializerService.init(new EnvironmentContext(EnvironmentContext.Mode.Prod));
    server.blockUntilShutdown();
  }
}
