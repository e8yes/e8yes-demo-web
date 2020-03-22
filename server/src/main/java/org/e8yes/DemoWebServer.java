package org.e8yes;

import io.grpc.Server;
import io.grpc.ServerBuilder;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.e8yes.environment.EnvironmentContextInterface;
import org.e8yes.environment.Initializer;
import org.e8yes.service.SocialNetworkService;
import org.e8yes.service.SystemService;
import org.e8yes.service.UserService;
import org.e8yes.service.interceptor.AuthorizationServerInterceptor;

/**
 * Adapted from
 * https://github.com/grpc/grpc-java/blob/v1.19.0/examples/src/main/java/io/grpc/examples/helloworld/HelloWorldServer.java
 *
 * @author davis
 */
public class DemoWebServer {

  private static final Logger LOGGER = Logger.getLogger(DemoWebServer.class.getName());

  private Server server;

  private void start(EnvironmentContextInterface env) throws IOException {
    // The port on which the server should run
    int port = 50051;

    server =
        ServerBuilder.forPort(port)
            .addService(new UserService())
            .addService(new SocialNetworkService())
            .addService(new SystemService())
            .intercept(
                new AuthorizationServerInterceptor(env.authorizationJwtProvider().jwtverifier()))
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

  public static void main(String[] args) throws Exception {
    Initializer.init(EnvironmentContextInterface.Environment.Prod);

    final DemoWebServer server = new DemoWebServer();
    server.start(Initializer.environmentContext());
    server.blockUntilShutdown();

    Initializer.cleanUp();
  }
}
