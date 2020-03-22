/**
 * e8yes demo web server.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */
package org.e8yes.service.interceptor;

import com.auth0.jwt.JWTVerifier;
import com.auth0.jwt.exceptions.JWTVerificationException;
import io.grpc.Context;
import io.grpc.Contexts;
import io.grpc.Metadata;
import io.grpc.ServerCall;
import io.grpc.ServerCallHandler;
import io.grpc.ServerInterceptor;
import io.grpc.Status;
import java.util.HashSet;
import java.util.Set;
import org.e8yes.constant.GrpcContexts;
import org.e8yes.service.identity.Identity;
import org.e8yes.service.identity.JwtAuthorizer;

/**
 * Intercepts RPC call and decode the JWT token from the metadata. The decoded identity will be
 * inserted into the context if the token can be verified. Adapts from
 * https://github.com/AnarSultanov/examples/blob/master/grpc-java-authentication/src/main/java/dev/sultanov/grpc/authentication/server/AuthorizationServerInterceptor.java.
 */
public class AuthorizationServerInterceptor implements ServerInterceptor {

  private static final Metadata.Key<byte[]> AUTHORIZATION_METADATA_KEY =
      Metadata.Key.of("A-bin", Metadata.BINARY_BYTE_MARSHALLER);

  private static final Set<String> publicEndpoints =
      new HashSet() {
        {
          add("org.e8yes.services.UserService/Authorize");
          add("org.e8yes.services.UserService/Register");
          add("org.e8yes.services.UserService/Search");
        }
      };

  private final JWTVerifier jwtverifier;

  public AuthorizationServerInterceptor(JWTVerifier jwtverifier) {
    this.jwtverifier = jwtverifier;
  }

  private <Req, Resp> boolean isPublicEndpoint(ServerCall<Req, Resp> serverCall) {
    return publicEndpoints.contains(serverCall.getMethodDescriptor().getFullMethodName());
  }

  @Override
  public <Req, Resp> ServerCall.Listener<Req> interceptCall(
      ServerCall<Req, Resp> serverCall,
      Metadata metadata,
      ServerCallHandler<Req, Resp> serverCallHandler) {
    if (isPublicEndpoint(serverCall)) {
      // Skip JWT authorization if the endpoint is public.
      return Contexts.interceptCall(Context.current(), serverCall, metadata, serverCallHandler);
    }

    ServerCall.Listener<Req> NoOpListener = new ServerCall.Listener() {};

    // Requires JWT authorization.
    byte[] jwtToken = metadata.get(AUTHORIZATION_METADATA_KEY);
    if (jwtToken == null) {
      // Missing token.
      Status status = Status.UNAUTHENTICATED.withDescription("Authorization token is missing");
      serverCall.close(status, metadata);
      return NoOpListener;
    }

    try {
      Identity identity = JwtAuthorizer.parseToken(jwtToken, jwtverifier);
      Context ctx = Context.current().withValue(GrpcContexts.IDENTITY_CONTEXT_KEY, identity);
      return Contexts.interceptCall(ctx, serverCall, metadata, serverCallHandler);
    } catch (JWTVerificationException e) {
      Status status = Status.UNAUTHENTICATED.withDescription(e.getMessage()).withCause(e);
      serverCall.close(status, metadata);
      return NoOpListener;
    }
  }
}
