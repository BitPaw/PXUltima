#pragma once

#include <OS/Network/PXServer.h>
#include <CS/OS/Error/ActionResult.hpp>
#include <OS/Memory/PXMemory.h>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace PX
{
	public ref class Server
	{
		private:
		PXServer* _pxServer;

		delegate void SocketDataRawSendEvent(const void* const pxSocket, const void* message, const PXSize messageSize);

	

		void OnSocketDataRawSend(const void* const pxSocket, const void* message, const PXSize messageSize)
		{

		}

		delegate void SocketDataRawReceiveEvent(const void* const pxSocket, const void* message, const PXSize messageSize);

		SocketDataRawReceiveEvent^ OnSocketDataRawReceiveManaged;

		void OnSocketDataRawReceive(const void* const pxSocket, const void* const message, const PXSize messageSize)
		{
			array<System::Byte>^ buffer = gcnew array<System::Byte>(messageSize);
			
			long long xxx = (long long)message;

			IntPtr adress = static_cast<IntPtr>(xxx);

			Marshal::Copy(adress, buffer, 0, messageSize);	

			this->OnDataReceive(-1, buffer);
		}

		public:

		delegate void SocketDataSendEvent(const PXSocketID pxSocket, array<System::Byte>^ buffer);
		event SocketDataSendEvent^ OnDataSend;

		delegate void SocketDataReceiveEvent(const PXSocketID pxSocket, array<System::Byte>^ buffer);
		event SocketDataReceiveEvent^ OnDataReceive;

		Server();
		~Server();

		PX::ActionResult Start(const unsigned short port);
		PX::ActionResult Stop();
		PX::ActionResult ClientKick(const PXSocketID socketID);
	};
}