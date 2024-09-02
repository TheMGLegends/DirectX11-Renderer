#pragma once

#include <vector>
#include <string>

#include "../Generics/MinimalWindows.h"

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager();

	void Set();
	std::vector<std::string> GetMessages() const;

private:
	unsigned long long next;
	struct IDXGIInfoQueue* dxgiInfoQueue;
};

