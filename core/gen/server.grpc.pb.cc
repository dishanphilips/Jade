// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: server.proto

#include "server.pb.h"
#include "server.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace JadeCore {

static const char* RpcBase_method_names[] = {
  "/JadeCore.RpcBase/Handle",
};

std::unique_ptr< RpcBase::Stub> RpcBase::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< RpcBase::Stub> stub(new RpcBase::Stub(channel));
  return stub;
}

RpcBase::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Handle_(RpcBase_method_names[0], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  {}

::grpc::ClientReaderWriter< ::JadeCore::Command, ::JadeCore::Command>* RpcBase::Stub::HandleRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::JadeCore::Command, ::JadeCore::Command>::Create(channel_.get(), rpcmethod_Handle_, context);
}

void RpcBase::Stub::experimental_async::Handle(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::JadeCore::Command,::JadeCore::Command>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::JadeCore::Command,::JadeCore::Command>::Create(stub_->channel_.get(), stub_->rpcmethod_Handle_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::JadeCore::Command, ::JadeCore::Command>* RpcBase::Stub::AsyncHandleRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::JadeCore::Command, ::JadeCore::Command>::Create(channel_.get(), cq, rpcmethod_Handle_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::JadeCore::Command, ::JadeCore::Command>* RpcBase::Stub::PrepareAsyncHandleRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::JadeCore::Command, ::JadeCore::Command>::Create(channel_.get(), cq, rpcmethod_Handle_, context, false, nullptr);
}

RpcBase::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RpcBase_method_names[0],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< RpcBase::Service, ::JadeCore::Command, ::JadeCore::Command>(
          std::mem_fn(&RpcBase::Service::Handle), this)));
}

RpcBase::Service::~Service() {
}

::grpc::Status RpcBase::Service::Handle(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::JadeCore::Command, ::JadeCore::Command>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace JadeCore

