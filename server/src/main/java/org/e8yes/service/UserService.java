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
package org.e8yes.service;

import com.google.protobuf.ByteString;
import io.grpc.Status;
import io.grpc.stub.StreamObserver;
import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.List;
import java.util.Optional;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.e8yes.environment.Initializer;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.exception.ResourceMissingException;
import org.e8yes.service.identity.JwtAuthorizer;
import org.e8yes.service.identity.UserCreation;
import org.e8yes.service.identity.UserEntity;
import org.e8yes.service.identity.UserProfile;
import org.e8yes.service.identity.UserRetrieval;
import org.e8yes.util.PaginationValidator;

/** Service for user management. */
public class UserService extends UserServiceGrpc.UserServiceImplBase {

  @Override
  public void register(RegistrationRequest req, StreamObserver<RegistrationReponse> res) {
    RegistrationReponse response;
    UserEntity user;
    try {
      user =
          UserCreation.createBaselineUser(
              req.getSecurityKey().toByteArray(),
              /*userId=*/ Optional.empty(),
              Initializer.environmentContext().demowebDbConnections().connectionReservoir());
      response =
          RegistrationReponse.newBuilder()
              .setErrorType(RegistrationReponse.RegistrationErrorType.RET_NoError)
              .setUserId(user.id.value())
              .build();

      res.onNext(response);
      res.onCompleted();
    } catch (SQLException | IllegalAccessException ex) {
      Logger.getLogger(UserService.class.getName()).log(Level.SEVERE, null, ex);
      res.onError(Status.INTERNAL.withDescription(ex.getMessage()).asException());
    }
  }

  @Override
  public void authorize(AuthorizationRequest req, StreamObserver<AuthorizationResponse> res) {
    try {
      UserEntity user =
          UserRetrieval.retrieveUserEntity(
              req.getUserId(),
              Initializer.environmentContext().demowebDbConnections().connectionReservoir());
      if (user == null) {
        throw new ResourceMissingException("User with ID=" + req.getUserId() + " doesn't exist.");
      }
      JwtAuthorizer.AuthorizationToken token =
          JwtAuthorizer.signAuthorizationToken(
              user,
              req.getSecurityKey().toByteArray(),
              Initializer.environmentContext().authorizationJwtProvider().algorithm());

      res.onNext(
          AuthorizationResponse.newBuilder()
              .setJwtToken(ByteString.copyFrom(token.jwtToken))
              .build());
      res.onCompleted();
    } catch (SQLException
        | NoSuchMethodException
        | InstantiationException
        | IllegalAccessException
        | IllegalArgumentException
        | InvocationTargetException ex) {
      Logger.getLogger(UserService.class.getName()).log(Level.SEVERE, null, ex);
      res.onError(Status.INTERNAL.withDescription(ex.getMessage()).asException());
    } catch (ResourceMissingException ex) {
      res.onError(Status.NOT_FOUND.withDescription(ex.getMessage()).asException());
    } catch (AccessDeniedException ex) {
      res.onError(Status.UNAUTHENTICATED.withDescription(ex.getMessage()).asException());
    }
  }

  @Override
  public void getPublicProfile(
      GetPublicProfileRequest req, StreamObserver<GetPublicProfileResponse> res) {}

  @Override
  public void updatePublicProfile(
      UpdatePublicProfileRequest req, StreamObserver<UpdatePublicProfileResponse> res) {}

  @Override
  public void search(SearchUserRequest req, StreamObserver<SearchUserResponse> res) {
    try {
      PaginationValidator.validate(req.getPagination(), /*resultPerPageLimit=*/ 100);
      Optional<Long> userIdPrefix =
          req.getUserId() == null || req.getUserId().getValue() == 0
              ? Optional.empty()
              : Optional.of(req.getUserId().getValue());
      Optional<String> aliasPrefix =
          req.getAlias() == null || req.getAlias().getValue().isEmpty()
              ? Optional.empty()
              : Optional.of(req.getAlias().getValue());

      List<UserEntity> results =
          UserRetrieval.searchUserEntity(
              userIdPrefix,
              aliasPrefix,
              req.getPagination(),
              Initializer.environmentContext().demowebDbConnections().connectionReservoir());

      SearchUserResponse.Builder builder = SearchUserResponse.newBuilder();
      for (UserEntity user : results) {
        UserPublicProfile profile = UserProfile.extractPublicInfo(user);
        builder.addEntries(profile);
      }

      res.onNext(builder.build());
      res.onCompleted();
    } catch (SQLException
        | NoSuchMethodException
        | InstantiationException
        | IllegalAccessException
        | InvocationTargetException ex) {
      Logger.getLogger(UserService.class.getName()).log(Level.SEVERE, null, ex);
      res.onError(Status.INTERNAL.withDescription(ex.getMessage()).asException());
    } catch (IllegalArgumentException ex) {
      res.onError(Status.INVALID_ARGUMENT.withDescription(ex.getMessage()).asException());
    }
  }
}
