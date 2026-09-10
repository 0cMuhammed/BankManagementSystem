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

public :

	Session() : m_CurrentUser(_GetEmptyObject()), m_isActive(false) {};

	
	Session(User user) : m_CurrentUser(std::move(user)), m_isActive(true) {};

	
	const User& GetUser() const noexcept {
		return m_CurrentUser;
	}
	void SetIsActive(bool setter) {
		m_isActive = setter;
	}
	bool isActive() const noexcept{
		return !m_CurrentUser.isEmpty() && m_isActive;
	}
	bool isEnded() const noexcept {
		return m_CurrentUser.isEmpty() && !m_isActive;
	}


};

