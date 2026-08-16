#pragma once
#include <string>

class UI {

public :
	virtual void PrintHeader(const std::string* ScreenName = nullptr, const std::string *SubTitle = nullptr) = 0;
	virtual void PerformMenu(const std::string *Message = nullptr) = 0;
	virtual void Show() = 0;

	virtual ~UI() = default;

};