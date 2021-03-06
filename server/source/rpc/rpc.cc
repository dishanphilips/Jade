#include <grpcpp/server_builder.h>

#include "jadecore.h"

#include "rpc/rpc.h"
#include "rpc/rpc_handler.h"

namespace JadeServer
{
	void Rpc::Listen()
	{
		// Create server address
		std::string server_address("0.0.0.0:50051");

		// Enable health check
		grpc::EnableDefaultHealthCheckService(true);

		// Create a server builder
		grpc::ServerBuilder builder;

		// Add listening port
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

		// Register service
		builder.RegisterService(&service_);
		
		// Add the completion queue for Rpcs
		rpc_queue = builder.AddCompletionQueue();

		// Add the completion queue for notifications
		notification_queue_ = builder.AddCompletionQueue();
		
		// Create the server
		server_ = builder.BuildAndStart();
		JadeCore::Logger::LogInfo("StartUp", "Server Listening On : " + server_address);

		// Handle RPCs
		this->Handle();
	}
	
	void Rpc::Handle()
	{
		// Set the status of the listener to true
		listening_ = true;
				
		// Start the rpc completion queue thread
		rpc_thread_ = std::thread([this] {this->ProcessCompletionQueue(this->rpc_queue);});

		// Start the notification queue thread
		notification_thread_ = std::thread([this] {this->ProcessCompletionQueue(this->notification_queue_); });

		// Spawn initial handler
		SpawnHandler();

		// Detach all threads to the mother ship
		rpc_thread_.detach();
		notification_thread_.detach();
	}

	void Rpc::Stop()
	{
		// Check if the server is listening
		if(listening_ == false)
		{
			return;
		}
		else
		{
			// Disable the server listening flag
			listening_ = false;

			// Lock all the handlers
			std::lock_guard<std::mutex> {handlers_lock_};

			for (const auto id_handler : handlers_)
			{
				std::lock_guard<std::mutex> { *id_handler.second->GetHandlerLock()};

				if(id_handler.second->GetStatus() == RpcStatus::Connecting)
				{
					id_handler.second->GetServerContext()->TryCancel();
				}
			}

			// Start the shutdown with shutting down the server
			JadeCore::Logger::LogInfo("Rpc", "Shutting down server.");
			server_->Shutdown();
			
			// Shutdown the RPC completion queue
			JadeCore::Logger::LogInfo("Rpc", "Shutting down completion queue.");
			rpc_queue->Shutdown();

			// Shutdown the notification completion queue
			JadeCore::Logger::LogInfo("Rpc", "Shutting down notification queue.");
			notification_queue_->Shutdown();
		}
	}

	void Rpc::ProcessCompletionQueue(std::unique_ptr<grpc::ServerCompletionQueue> & completion_queue_)
	{	
		uint64_t handler_id_;
		bool ok;

		// Loop over the queue
		while(completion_queue_->Next(reinterpret_cast<void**>(&handler_id_), &ok))
		{
			const RpcEvent rpc_event = static_cast<RpcEvent>(GRPC_EVENT_MASK & handler_id_);
			handler_id_ = handler_id_ >> GRPC_EVENT_BIT_LENGTH;
			
			JadeCore::Logger::LogInfo("RpcEvent", "Handling Id : %d", handler_id_);

			// Obtain the handler
			auto handler = GetHandler(handler_id_);
			
			if(rpc_event == RpcEvent::Finished || ok == false)
			{
				this->RemoveHandler(handler_id_);
			}
			else if(handler != nullptr)
			{
				// Lock the handler and process the event
				std::lock_guard<std::mutex> { *handler->GetHandlerLock().get() };
				handler->Process(rpc_event);

				// If the rpc event a new connection, then spawn a new handler
				if(rpc_event == Initialized)
				{
					SpawnHandler();
				}
			}
		}
	}
	
	std::shared_ptr<RpcHandler> Rpc::SpawnHandler()
	{
		// Create a new handler with a new id
		uint64_t new_handler_id = ++handler_allocator_;
		auto handler = std::make_shared<RpcHandler>(new_handler_id);

		if (handler->Create(service_, rpc_queue.get(), notification_queue_.get()))
		{
			// Obtain a lock on the handlers
			std::lock_guard<std::mutex>handlers_lock(handlers_lock_);

			// Add the new handler to the list of handlers
			handlers_[new_handler_id] = handler;
			JadeCore::Logger::LogInfo("RpcEvent", "Spawning Handler with Id : %d", new_handler_id);
		}
		else
		{
			JadeCore::Logger::LogError("Rpc", "Could not spawn new handler!");
			return nullptr;
		}
		return handler;
	}

	std::shared_ptr<RpcHandler> Rpc::GetHandler(uint64_t handler_id)
	{
		// Obtain a lock on the handlers
		std::lock_guard<std::mutex> {handlers_lock_};

		// Try to find the handler
		const auto id_and_handler = handlers_.find(handler_id);

		// Check if the handler was found, if not found, return null
		if (id_and_handler == handlers_.end())
		{
			return nullptr;
		}
		else
		{
			return id_and_handler->second;
		}
	}

	void Rpc::RemoveHandler(uint64_t handler_id_)
	{
		// Obtain a lock on the handlers
		std::lock_guard<std::mutex> {handlers_lock_};

		// Remove the handler with the given handler id
		handlers_.erase(handler_id_);
	}

	void Rpc::SendClientCommand(string client_id, int operation, grpc::protobuf::Message* command)
	{
		 const auto client_and_id = clients_.find(client_id);

		if(client_and_id == clients_.end())
		{
			return;
		}
		else
		{
			this->SendClientCommand(client_and_id->second, operation, command);
		}
	}

	void Rpc::SendClientCommand(uint64_t handler_id, int operation, grpc::protobuf::Message* command)
	{
		// Obtain the handler
		auto handler = this->GetHandler(handler_id);

		// If the handler is found, write the client command
		if(handler != nullptr)
		{
			handler->SendClientCommand(operation, command);
		}
	}
}
