#include "DxgiInfoManager.h"

#include <dxgidebug.h>
#include <memory>

#include "../Window/Window.h"
#include "../Graphics/Renderer.h"

#pragma comment(lib, "dxguid.lib")

#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw Renderer::HrException(__LINE__, __FILE__, hr)

DxgiInfoManager::DxgiInfoManager()
{
	// INFO: Defintion of the function signature
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// INFO: Load the DLL containing the DXGIGetDebugInterface function
	const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

	if (hModDxgiDebug == nullptr)
	{
		throw WINDOW_THROW_LAST_EXCEPTION();
	}

	// INFO: Get the address of the DXGIGetDebugInterface function from the DLL
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"));

	if (DxgiGetDebugInterface == nullptr)
	{
		throw WINDOW_THROW_LAST_EXCEPTION();
	}

	HRESULT hr;
	GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &dxgiInfoQueue));
}

void DxgiInfoManager::Set()
{
	// INFO: Sets the index so only new messages will be retrieved when GetMessages is called
	next = dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for (auto i = next; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;

		// INFO: Get Message Size in Bytes
		GFX_THROW_NOINFO(dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

		// INFO: Allocate Memory for Message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		// INFO: Get the Message and Push it to the Vector
		GFX_THROW_NOINFO(dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, &messageLength));

		messages.emplace_back(message->pDescription);
	}
	return messages;
}
