#pragma once

#include "../../../core/include/logger/Logger.h"

#include "../../include/startup/startup_task.h"
#include "../../include/rpc/rpc.h"

using JadeServer::StartupTask;

namespace JadeServer
{
	void StartupTask::Execute()
	{
		// 0. Execute base startup
		JadeCore::StartupTask::Execute();

		// 99. Finally listen to the rpc service
		JadeCore::Logger::LogInfo("Creating Rpc Listener.", "ServerStartup");

		// Create the a thread to run the RPC service
		rpc_ = new Rpc();
		std::thread rpc_thread(&Rpc::Listen, rpc_);

		// Detach and run it independently
		rpc_thread.detach();

		// Add the thread reference
		this->rpc_thread_ = &rpc_thread;
				
		// Finish the task
		Finish();
	}

	void StartupTask::OnComplete(TaskStatus status)
	{
		// Invoke the base method
		JadeCore::StartupTask::OnComplete(status);

		// Print success log
		JadeCore::Logger::LogInfo("Server startup complete!", "ServerStartup");
	}

	std::thread* StartupTask::GetRpcThread()
	{
		return this->rpc_thread_;
	}
}