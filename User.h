#pragma once
#include<vector>
#include <cstdint>
#include<fstream>
#include "Person.h"
#include "Authorizer.h"

using Permission = Authorizer::Permissions;
class FileHandler;


class User : public Person {

public :

	enum class ObjectMode : uint8_t
	{ 
		EmptyMode = 1, ExistingMode = 2, newMode = 3, DeleteMode = 4 
	};


private :

	std::string m_username = "";
	std::string m_password = "";  // hashed
	int32_t m_userPermissions = 0;
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
	int32_t GetPermissions() const noexcept { 
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
		m_password = Hasher::GetHash(password);
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



