#pragma once

#include "Socket.h"

namespace nwayinterview
{
	// A wrapper of CompletionPort and more.
	class Scheduler
	{
		NONCOPYABLE(Scheduler)

	public:
		Scheduler();
		virtual ~Scheduler();

		void AddHandle(Socket& socket);

		void PostStatus(ULONG_PTR completionKey,
			DWORD dwNumBytes = 0,
			OVERLAPPED *pOverlapped = 0);

		void GetStatus(ULONG_PTR *pCompletionKey,
			PDWORD pdwNumBytes,
			OVERLAPPED **ppOverlapped);

	private:

		HANDLE m_iocp;
	};

}