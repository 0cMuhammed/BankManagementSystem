#pragma once
#include <string>

class UI {

public :
	virtual void PrintHeader(const char* ScreenName = nullptr, const char *SubTitle = nullptr) = 0;
	virtual void PerformMenu(const char *Message = nullptr) = 0;
	virtual void Show() = 0;

	

	virtual ~UI() = default;

};