#pragma once
#include<vector>
#include<fstream>
#include "Person.h"

class FileHandler;

/*/ bool hasAccess(const stUser & user, eUserPermissions requiredPerm) {


	if (user.userPermissions == static_cast<int>(eUserPermissions::allPermissions))
		return true;


	return user.userPermissions & static_cast<int>(requiredPerm);

} /*/


class User : public Person {
public :
	enum ObjectMode 
	{ 
		EmptyMode = 1, ExistingMode = 2, newMode = 3, DeleteMode = 4 
	};
	enum UserPermissions 
	{
		ShowList = 1 << 0, AddClient = 1 << 1, DeleteClient = 1 << 2, UpdateClient = 1 << 3, FindClient = 1 << 4, Transactions = 1 << 5, ManageUsers = 1 << 6, AllPermissions = -1
	};

private :

	std::string m_username;
	std::string m_password;
	int8_t m_userPermissions = 0;
	ObjectMode m_mode = ObjectMode::EmptyMode;

	//using FileHandler
	void _SaveExistingObject();
	void _SaveNewObject();

public :

	User(std::string firstName, std::string lastName, std::string email, std::string phoneNumber,std::string username, std::string password, int8_t userPermissions, ObjectMode mode) : Person(std::move(firstName), std::move(lastName), std::move(email), std::move(phoneNumber)), m_username(std::move(username)), m_password(std::move(password)), m_userPermissions(userPermissions), m_mode(mode) {};


	bool operator==(const User& user) const {
		return (this == &user || (GetFirstName() == user.GetFirstName() && GetLastName() == user.GetLastName() && GetEmail() == user.GetEmail() && GetPhoneNumber() == user.GetPhoneNumber() && m_username == user.GetUsername() && m_password == user.GetPassword() && m_userPermissions == user.GetPermissions() && m_mode == user.GetMode()) );
	}
	bool operator !=(const User& user) const {
		return !(*this == user);
	}

	bool isEmpty() const noexcept {
		return (m_mode == ObjectMode::EmptyMode);
	}

	const std::string& GetUsername() const noexcept {
		return m_username;
	}
	const std::string& GetPassword() const noexcept { 
		return m_password;
	}
	int8_t GetPermissions() const noexcept { 
		return m_userPermissions; // read only. and only the admins can change the permissions of other users 
	}
	ObjectMode GetMode() const noexcept {
		return m_mode;
	}

	void SetUsername(const std::string &userName) {
		m_username = userName;
	}
	void SetMode(ObjectMode mode) {
		m_mode = mode;
	}
	void SetPassword(const std::string& password) {
		m_password = password;
	}
	void SetPermissions(UserPermissions Permission) {
		m_userPermissions |= static_cast<int8_t>(Permission);
	}
	void SetPermissions(int8_t number) {
		m_userPermissions = number;
	}

	void SetObject(const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newUserName, const std::string &newPassword) {
		SetFirstName(newFirstName);
		SetLastName(newLastName);
		SetEmail(newEmail);
		SetPhoneNumber(newPhoneNumber);
		SetUsername(newUserName);
		SetPassword(newPassword);
	}

	void Save() {
		
		switch (m_mode)
		{
		case ObjectMode::EmptyMode:
		{
			return;
		}
		case ObjectMode::ExistingMode:
		{
			_SaveExistingObject();
			break;
		}
		case ObjectMode::newMode:
		{
			_SaveNewObject();
			break;

		}
		default:
			return;

		}



	}

};

#include "FileHandler.h"
inline void User::_SaveExistingObject() {
	std::vector<User> Users = FileHandler::LoadUsers();

	for (User& user : Users)
	{
		if (user.m_password == m_password)
		{
			user = *this;
			break;

		}

	}

	FileHandler::SaveUsers(Users);
}
inline void User::_SaveNewObject() {
	m_mode = ObjectMode::ExistingMode;
	FileHandler::SaveUsers(*this);
}

