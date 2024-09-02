#include "DxgiInfoManager.h"

#include <dxgidebug.h>
#include <memory>

#include "../Window/Window.h"
#include "../Graphics/Renderer.h"

#pragma comment(lib, "dxguid.lib")

DxgiInfoManager::DxgiInfoManager()
{

}

DxgiInfoManager::~DxgiInfoManager()
{
	if (dxgiInfoQueue != nullptr)
		dxgiInfoQueue->Release();
}
