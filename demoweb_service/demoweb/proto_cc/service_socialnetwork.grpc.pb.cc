// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: service_socialnetwork.proto

#include "service_socialnetwork.pb.h"
#include "service_socialnetwork.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace e8 {

static const char* SocialNetworkService_method_names[] = {
  "/e8.SocialNetworkService/GetUserRelations",
  "/e8.SocialNetworkService/SendInvitation",
  "/e8.SocialNetworkService/DeleteInvitation",
  "/e8.SocialNetworkService/ProcessInvitation",
  "/e8.SocialNetworkService/DeleteContact",
  "/e8.SocialNetworkService/GetRelatedUserList",
};

std::unique_ptr< SocialNetworkService::Stub> SocialNetworkService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< SocialNetworkService::Stub> stub(new SocialNetworkService::Stub(channel));
  return stub;
}

SocialNetworkService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_GetUserRelations_(SocialNetworkService_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_SendInvitation_(SocialNetworkService_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DeleteInvitation_(SocialNetworkService_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ProcessInvitation_(SocialNetworkService_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DeleteContact_(SocialNetworkService_method_names[4], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetRelatedUserList_(SocialNetworkService_method_names[5], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status SocialNetworkService::Stub::GetUserRelations(::grpc::ClientContext* context, const ::e8::GetUserRelationsRequest& request, ::e8::GetUserRelationsResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_GetUserRelations_, context, request, response);
}

void SocialNetworkService::Stub::experimental_async::GetUserRelations(::grpc::ClientContext* context, const ::e8::GetUserRelationsRequest* request, ::e8::GetUserRelationsResponse* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_GetUserRelations_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::e8::GetUserRelationsResponse>* SocialNetworkService::Stub::AsyncGetUserRelationsRaw(::grpc::ClientContext* context, const ::e8::GetUserRelationsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::GetUserRelationsResponse>::Create(channel_.get(), cq, rpcmethod_GetUserRelations_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::e8::GetUserRelationsResponse>* SocialNetworkService::Stub::PrepareAsyncGetUserRelationsRaw(::grpc::ClientContext* context, const ::e8::GetUserRelationsRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::GetUserRelationsResponse>::Create(channel_.get(), cq, rpcmethod_GetUserRelations_, context, request, false);
}

::grpc::Status SocialNetworkService::Stub::SendInvitation(::grpc::ClientContext* context, const ::e8::SendInvitationRequest& request, ::e8::SendInvitationResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_SendInvitation_, context, request, response);
}

void SocialNetworkService::Stub::experimental_async::SendInvitation(::grpc::ClientContext* context, const ::e8::SendInvitationRequest* request, ::e8::SendInvitationResponse* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_SendInvitation_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::e8::SendInvitationResponse>* SocialNetworkService::Stub::AsyncSendInvitationRaw(::grpc::ClientContext* context, const ::e8::SendInvitationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::SendInvitationResponse>::Create(channel_.get(), cq, rpcmethod_SendInvitation_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::e8::SendInvitationResponse>* SocialNetworkService::Stub::PrepareAsyncSendInvitationRaw(::grpc::ClientContext* context, const ::e8::SendInvitationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::SendInvitationResponse>::Create(channel_.get(), cq, rpcmethod_SendInvitation_, context, request, false);
}

::grpc::Status SocialNetworkService::Stub::DeleteInvitation(::grpc::ClientContext* context, const ::e8::DeleteInvitationRequest& request, ::e8::DeleteInvitationResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_DeleteInvitation_, context, request, response);
}

void SocialNetworkService::Stub::experimental_async::DeleteInvitation(::grpc::ClientContext* context, const ::e8::DeleteInvitationRequest* request, ::e8::DeleteInvitationResponse* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_DeleteInvitation_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::e8::DeleteInvitationResponse>* SocialNetworkService::Stub::AsyncDeleteInvitationRaw(::grpc::ClientContext* context, const ::e8::DeleteInvitationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::DeleteInvitationResponse>::Create(channel_.get(), cq, rpcmethod_DeleteInvitation_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::e8::DeleteInvitationResponse>* SocialNetworkService::Stub::PrepareAsyncDeleteInvitationRaw(::grpc::ClientContext* context, const ::e8::DeleteInvitationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::DeleteInvitationResponse>::Create(channel_.get(), cq, rpcmethod_DeleteInvitation_, context, request, false);
}

::grpc::Status SocialNetworkService::Stub::ProcessInvitation(::grpc::ClientContext* context, const ::e8::ProcessInvitationRequest& request, ::e8::ProcessInvitationResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_ProcessInvitation_, context, request, response);
}

void SocialNetworkService::Stub::experimental_async::ProcessInvitation(::grpc::ClientContext* context, const ::e8::ProcessInvitationRequest* request, ::e8::ProcessInvitationResponse* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_ProcessInvitation_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::e8::ProcessInvitationResponse>* SocialNetworkService::Stub::AsyncProcessInvitationRaw(::grpc::ClientContext* context, const ::e8::ProcessInvitationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::ProcessInvitationResponse>::Create(channel_.get(), cq, rpcmethod_ProcessInvitation_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::e8::ProcessInvitationResponse>* SocialNetworkService::Stub::PrepareAsyncProcessInvitationRaw(::grpc::ClientContext* context, const ::e8::ProcessInvitationRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::ProcessInvitationResponse>::Create(channel_.get(), cq, rpcmethod_ProcessInvitation_, context, request, false);
}

::grpc::Status SocialNetworkService::Stub::DeleteContact(::grpc::ClientContext* context, const ::e8::DeleteContactRequest& request, ::e8::DeleteContactResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_DeleteContact_, context, request, response);
}

void SocialNetworkService::Stub::experimental_async::DeleteContact(::grpc::ClientContext* context, const ::e8::DeleteContactRequest* request, ::e8::DeleteContactResponse* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_DeleteContact_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::e8::DeleteContactResponse>* SocialNetworkService::Stub::AsyncDeleteContactRaw(::grpc::ClientContext* context, const ::e8::DeleteContactRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::DeleteContactResponse>::Create(channel_.get(), cq, rpcmethod_DeleteContact_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::e8::DeleteContactResponse>* SocialNetworkService::Stub::PrepareAsyncDeleteContactRaw(::grpc::ClientContext* context, const ::e8::DeleteContactRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::DeleteContactResponse>::Create(channel_.get(), cq, rpcmethod_DeleteContact_, context, request, false);
}

::grpc::Status SocialNetworkService::Stub::GetRelatedUserList(::grpc::ClientContext* context, const ::e8::GetRelatedUserListRequest& request, ::e8::GetRelatedUserListResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_GetRelatedUserList_, context, request, response);
}

void SocialNetworkService::Stub::experimental_async::GetRelatedUserList(::grpc::ClientContext* context, const ::e8::GetRelatedUserListRequest* request, ::e8::GetRelatedUserListResponse* response, std::function<void(::grpc::Status)> f) {
  return ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_GetRelatedUserList_, context, request, response, std::move(f));
}

::grpc::ClientAsyncResponseReader< ::e8::GetRelatedUserListResponse>* SocialNetworkService::Stub::AsyncGetRelatedUserListRaw(::grpc::ClientContext* context, const ::e8::GetRelatedUserListRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::GetRelatedUserListResponse>::Create(channel_.get(), cq, rpcmethod_GetRelatedUserList_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::e8::GetRelatedUserListResponse>* SocialNetworkService::Stub::PrepareAsyncGetRelatedUserListRaw(::grpc::ClientContext* context, const ::e8::GetRelatedUserListRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::e8::GetRelatedUserListResponse>::Create(channel_.get(), cq, rpcmethod_GetRelatedUserList_, context, request, false);
}

SocialNetworkService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      SocialNetworkService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< SocialNetworkService::Service, ::e8::GetUserRelationsRequest, ::e8::GetUserRelationsResponse>(
          std::mem_fn(&SocialNetworkService::Service::GetUserRelations), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      SocialNetworkService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< SocialNetworkService::Service, ::e8::SendInvitationRequest, ::e8::SendInvitationResponse>(
          std::mem_fn(&SocialNetworkService::Service::SendInvitation), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      SocialNetworkService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< SocialNetworkService::Service, ::e8::DeleteInvitationRequest, ::e8::DeleteInvitationResponse>(
          std::mem_fn(&SocialNetworkService::Service::DeleteInvitation), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      SocialNetworkService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< SocialNetworkService::Service, ::e8::ProcessInvitationRequest, ::e8::ProcessInvitationResponse>(
          std::mem_fn(&SocialNetworkService::Service::ProcessInvitation), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      SocialNetworkService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< SocialNetworkService::Service, ::e8::DeleteContactRequest, ::e8::DeleteContactResponse>(
          std::mem_fn(&SocialNetworkService::Service::DeleteContact), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      SocialNetworkService_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< SocialNetworkService::Service, ::e8::GetRelatedUserListRequest, ::e8::GetRelatedUserListResponse>(
          std::mem_fn(&SocialNetworkService::Service::GetRelatedUserList), this)));
}

SocialNetworkService::Service::~Service() {
}

::grpc::Status SocialNetworkService::Service::GetUserRelations(::grpc::ServerContext* context, const ::e8::GetUserRelationsRequest* request, ::e8::GetUserRelationsResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status SocialNetworkService::Service::SendInvitation(::grpc::ServerContext* context, const ::e8::SendInvitationRequest* request, ::e8::SendInvitationResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status SocialNetworkService::Service::DeleteInvitation(::grpc::ServerContext* context, const ::e8::DeleteInvitationRequest* request, ::e8::DeleteInvitationResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status SocialNetworkService::Service::ProcessInvitation(::grpc::ServerContext* context, const ::e8::ProcessInvitationRequest* request, ::e8::ProcessInvitationResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status SocialNetworkService::Service::DeleteContact(::grpc::ServerContext* context, const ::e8::DeleteContactRequest* request, ::e8::DeleteContactResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status SocialNetworkService::Service::GetRelatedUserList(::grpc::ServerContext* context, const ::e8::GetRelatedUserListRequest* request, ::e8::GetRelatedUserListResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace e8

