#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "User.h"
#include "FileHandler.h"
#include "Validator.h"

using Permission = User::UserPermissions;
using Mode = User::ObjectMode;

class UserRepository {

public :

	enum  OperationStates { Failed = 1, Successful = 2, UserAlreadyExists = 3, UserNotFound = 4 };

private:


	std::vector<User> m_List;

	void _UpdateVector() {
		m_List.clear();
		m_List = FileHandler::LoadUsers();
	}

	void _AddInVector(const User& user) {
		m_List.push_back(user);
	}

	User _GetEmptyObject() const noexcept {
		return User("", "", "", "", "", "", 0, Mode::EmptyMode);
	}

	User  _GetNewObject() const noexcept {
		return  User("", "", "", "", "", "", 0, Mode::EmptyMode);
	}

	void _MakeEmpty(User& user) {
		if (user.GetMode() == Mode::DeleteMode)
		{
			user = _GetEmptyObject();
		}

	}

	static bool _IsModifiable(const User& user, const std::string& username, const std::string &password) {
		return ( user.GetUsername() == username && user.GetPassword() == password) && user.GetMode() == Mode::ExistingMode;
	}

	static int8_t _SetUserPermissions(User& user) {

		user.SetPermissions(0);

		std::cout << "\nDo you want to give access to : \n";

		if (Validator::GetConfirmation("\nShow client list ? y/n : "))
		{
			user.SetPermissions(Permission::ShowList);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nAdd Client? y/n : "))
		{
			user.SetPermissions(Permission::AddClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nDelete Client ? y/n : "))
		{
			user.SetPermissions(Permission::DeleteClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nUpdate Client ? y/n : "))
		{
			user.SetPermissions(Permission::UpdateClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nFind Client ? y/n : "))
		{
			user.SetPermissions(Permission::FindClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nTransactions Client ? y/n : "))
		{
			user.SetPermissions(Permission::Transactions);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nManage Users ? y/n : "))
		{
			user.SetPermissions(Permission::ManageUsers);
			std::cout << "\n";
		}

		return user.GetPermissions();
	}
	static int8_t _SetUserPermissions() {

		int8_t Permissions = 0;

		std::cout << "\nDo you want to give access to : \n";

		if (Validator::GetConfirmation("\nShow client list ? y/n : "))
		{
			Permissions |= static_cast<int8_t>(Permission::ShowList);

			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nAdd Client? y/n : "))
		{
			Permissions |= static_cast<int8_t>(Permission::AddClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nDelete Client ? y/n : "))
		{
			Permissions |= static_cast<int8_t>(Permission::DeleteClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nUpdate Client ? y/n : "))
		{
			Permissions |= static_cast<int8_t>(Permission::UpdateClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nFind Client ? y/n : "))
		{
			Permissions |= static_cast<int8_t>(Permission::FindClient);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nTransactions Client ? y/n : "))
		{
			Permissions |= static_cast<int8_t>(Permission::Transactions);
			std::cout << "\n";
		}
		if (Validator::GetConfirmation("\nManage Users ? y/n : "))
		{
			Permissions |= static_cast<int8_t>(Permission::ManageUsers);
			std::cout << "\n";
		}

		return Permissions;
	}

	static int8_t _ReadPermissions(User& user) {

		if (Validator::GetConfirmation("\nDo you want to give full access?"))
		{
			user.SetPermissions(static_cast<int>(Permission::AllPermissions));
		}
		else
		{
			_SetUserPermissions(user);
		}
		return user.GetPermissions();

	}
	static int8_t _ReadPermissions() {
		int8_t Permissions = 0;

		Permissions =  (Validator::GetConfirmation("\nDo you want to give full access?")) ? static_cast<int8_t>(Permission::AllPermissions) : _SetUserPermissions();
	
		return Permissions;

	}

	static void _Message(const char* Message) {
		std::cout << '\n' + std::string(Message);
	}

	bool _DeleteObject(const std::string& username, const std::string &password) {


		for (User& c : m_List)
		{
			if (_IsModifiable(c, username,password))
			{
				c.SetMode(Mode::DeleteMode);
				_MakeEmpty(c);
				return true;
			}

		}


		return false;

	}

	bool _DeleteObject(const User& user) {


		for (User& c : m_List)
		{
			if (_IsModifiable(c, user.GetUsername(), user.GetPassword()))
			{
				c.SetMode(Mode::DeleteMode);
				_MakeEmpty(c);
				return true;
			}

		}


		return false;

	}

	bool _UpdateObject(const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newUsername, const std::string &newPassword, const std::string& OldUsername, const std::string &OldPassword) {

		for (User& c : m_List)
		{
			if (_IsModifiable(c, OldUsername,OldPassword))
			{
				c.SetObject(newFirstName, newLastName, newEmail, newPhoneNumber, newUsername, newPassword);
				c.Save();
				return true;
			}

		}

		return false;

	}
	bool _UpdateObject(User & user) {

		for (User & c : m_List)
		{
			if (_IsModifiable(c, user.GetUsername(), user.GetPassword()))
			{
				c = std::move(user);
				c.Save();
				return true;
			}

		}

		return false;

	}



	User _FindObject(const std::string& username, const char * password = nullptr) const {



		for (const User& user: m_List)
		{
			bool usernameMatch = user.GetUsername() == username;
			bool passwordMatch = (password == nullptr) ? true :  user.GetPassword() == std::string(password);

			if (usernameMatch && passwordMatch)
			{
				return user;

			}
		}


		return _GetEmptyObject();
	}

public:

	UserRepository(std::vector<User> Users) : m_List(std::move(Users)) {};


	const std::vector<User>& GetList() const {
		return m_List;
	}

	static void PrintUser(const User& user) {

		std::cout << "\nUser Card:";
		std::cout << "\n___________________";
		std::cout << "\nFirstName   : " << user.GetFirstName();
		std::cout << "\nLastName    : " << user.GetLastName();
		std::cout << "\nFull Name   : " << user.GetFullName();
		std::cout << "\nEmail       : " << user.GetEmail();
		std::cout << "\nPhone       : " << user.GetPhoneNumber();
		std::cout << "\nUsername : " << user.GetUsername();
		std::cout << "\nPassword    : " << user.GetPassword();
		std::cout << "\nPermissions  : " << user.GetPermissions(); 
		std::cout << "\n___________________\n";


	}
	static User ReadUser()
	{
	
		_Message("Enter First name");
		std::string FirstName = Validator::ReadString();

		_Message("Enter Last name");
		std::string LastName = Validator::ReadString();

		_Message("Enter Email");
		std::string Email = Validator::ReadString();

		_Message("Enter Phone");
		std::string Phone = Validator::ReadString();

		_Message("Enter Username");
		std::string Username = Validator::ReadString();

		_Message("Enter Password");
		std::string Password = Validator::ReadString();

		_Message("Enter Permissions"); 
		int8_t Permissions = _ReadPermissions(); // function that sets up the permissions
		

		return User(FirstName, LastName, Email, Phone, Username, Password, Permissions, User::ObjectMode::newMode);
	}
	static User ReadUser(const std::string& ExistingUsername)
	{

		_Message("Enter First name");
		std::string FirstName = Validator::ReadString();

		_Message("Enter Last name");
		std::string LastName = Validator::ReadString();

		_Message("Enter Email");
		std::string Email = Validator::ReadString();

		_Message("Enter Phone");
		std::string Phone = Validator::ReadString();

		_Message("Enter Password");
		std::string Password = Validator::ReadString();

		_Message("Enter Permissions");
		int8_t Permissions = _ReadPermissions(); // function that sets up the permissions


		return User(FirstName, LastName, Email, Phone, ExistingUsername, Password, Permissions, User::ObjectMode::newMode);
	}


	bool IsExists(const std::string& username, const std::string & password) {

		User user = _FindObject(username, password.c_str());
		return (!user.isEmpty());
	}


	User Find(const std::string& username, const char * password = nullptr) {
		return _FindObject(username, password);
	}

	OperationStates AddUser(User& FilledObject)
	{
		if ( IsExists(FilledObject.GetUsername(), FilledObject.GetPassword() ))
		{
			return OperationStates::UserAlreadyExists;
		}


		FilledObject.SetMode(Mode::newMode);
		FilledObject.Save();
		_AddInVector(FilledObject);



		return OperationStates::Successful;

	}

	OperationStates DeleteUser(const User& ExistingObject) {


		if (!_DeleteObject(ExistingObject))
		{
			return OperationStates::Failed;

		}



		FileHandler::SaveUsers(m_List);
		_UpdateVector();

		return OperationStates::Successful;

	}
	OperationStates DeleteUser(const std::string& username, const std::string & password) {

		if (!IsExists(username,password.c_str()))
		{
			return OperationStates::UserNotFound;
		}

		if (! _DeleteObject(username,password))
		{
			return OperationStates::Failed;

		}



		FileHandler::SaveUsers(m_List);
		_UpdateVector();

		return OperationStates::Successful;

	}

	OperationStates UpdateUser(const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newUsername, const std::string newPassword, const std::string& OldUsername, const std::string &OldPassword) {

		if (!IsExists(OldUsername,OldPassword.c_str()))
		{
			return OperationStates::UserNotFound;
		}

		if (!_UpdateObject(newFirstName, newLastName, newEmail, newPhoneNumber, newUsername,newPassword,OldUsername,OldPassword))
		{
			return  OperationStates::UserNotFound;

		}

		FileHandler::SaveUsers(m_List);

		return OperationStates::Successful;

	}
	OperationStates UpdateUser(User& ExistingObject) {



		if (!_UpdateObject(ExistingObject))
		{
			return OperationStates::Failed;

		}

		FileHandler::SaveUsers(m_List);

		return OperationStates::Successful;

	}

};

