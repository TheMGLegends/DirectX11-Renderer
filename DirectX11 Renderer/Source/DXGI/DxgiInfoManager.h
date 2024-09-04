#pragma once

#include <vector>
#include <string>
#include <wrl.h>
#include <dxgidebug.h>

#include "../Generics/MinimalWindows.h"

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager() = default;

	void Set();
	std::vector<std::string> GetMessages() const;

private:
	unsigned long long next;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
};

