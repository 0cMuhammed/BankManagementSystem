#pragma once
#include "User.h"
#include "Authorizer.h"

class Session
{

private :

	User m_CurrentUser;
	bool m_isActive;

	static User _GetEmptyObject() noexcept {
		return User("", "", "", "", "", "", 0, User::ObjectMode::EmptyMode);
	}
	static void _ExitMenu(bool& isInMainMenu, const char* message = "\nLogging Out...")
	{

		std::cout << message << "\n\n";

		isInMainMenu = false;

	}

public :

	Session() : m_CurrentUser(_GetEmptyObject()), m_isActive(false) {};

	
	Session(User user) : m_CurrentUser(std::move(user)), m_isActive(true) {};

	
	 User& GetUser() noexcept {
		return m_CurrentUser;
	}

	void SetIsActive(bool setter) {
		m_isActive = setter;
	}
	bool isActiveState() const noexcept {
		return  m_isActive;
	}
	bool isActive() const noexcept{
		return !m_CurrentUser.isEmpty() && m_isActive;
	}
	bool isEndedState() const noexcept {
		return m_CurrentUser.isEmpty() && !m_isActive;
	}
	bool isEnded() const noexcept {
		return !m_isActive;
	}
	void Logout(bool &isInMainMenu) {
		m_isActive = false;
	    _ExitMenu(isInMainMenu);
	}


};

