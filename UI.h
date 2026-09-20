#pragma once
#include <string>

class User;
class Session;

class UI {

public :

	virtual void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) {

	}

	virtual void PerformMenu(const User& CurrentUser, const char* Message = nullptr) {

	}
	virtual void PerformMenu(User &CurrentUser, const char* Message = nullptr) {

	}
	virtual void Start(const User& CurrentUser) {

	}
	virtual void Start(User &CurrentUser) {

	}

	virtual void PerformMainMenu(Session& CurrentUser, const char* Message = nullptr)
	{

	}
	virtual void StartMainMenu(Session& CurrentUser)
	{

	}

	virtual ~UI() = default;

};