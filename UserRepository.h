#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "User.h"

#include "Authorizer.h"

#include "FileHandler.h"
#include "Validator.h"

using Permission = Authorizer::Permissions;

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

	static User _GetEmptyObject() noexcept {
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

	
	std::string  _ReadUsername() {

		_Message("Enter your username : ");
		std::string Username = Validator::ReadString();

		while (IsExists(Username)) {

			_Message("Username is Already Used, Please choose another username : \n");

			Username = Validator::ReadString();
		}

		return Username;
	}

	static std::string  _ReadUsername() {

		_Message("Enter your username : ");
		std::string Username = Validator::ReadString();

		while (IsExists(Username)) {

			_Message("Username is Already Used, Please choose another username : \n");

			Username = Validator::ReadString();
		}

		return Username;
	}

	static void _Message(const std::string & Message) {
		std::cout << '\n' + Message;
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



	static User _FindObjectFromFile(const std::string& username, const char * password = nullptr)  {


		std::vector<User> users = FileHandler::LoadUsers();

		for (const User& user: users)
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
	User _FindObject(const std::string& username, const char* password = nullptr) {

		for (const User& user : m_List)
		{
			bool usernameMatch = user.GetUsername() == username;
			bool passwordMatch = (password == nullptr) ? true : user.GetPassword() == std::string(password);

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
		std::cout << "\nUsername    : " << user.GetUsername();
		std::cout << "\nPassword    : " << user.GetPassword();
		std::cout << "\nPermissions : " << std::to_string(user.GetPermissions());
		std::cout << "\n___________________\n";


	}
	static User ReadNewUser(const User& CurrentUser)
	{
		
		std::string Username =  _ReadUsername();


		_Message("Enter First name : ");
		std::string FirstName = Validator::ReadString();

		_Message("Enter Last name : ");
		std::string LastName = Validator::ReadString();

		_Message("Enter Email : ");
		std::string Email = Validator::ReadString();

		
		std::string Phone = Validator::ReadPhoneNumber();

		_Message("Enter Password : ");
		std::string Password = Hasher::GetHash(Validator::ReadString());

		_Message("Enter Permissions : "); 
		int32_t Permissions = Authorizer::ReadPermissions(CurrentUser); // function that sets up the permissions
		

		return User(FirstName, LastName, Email, Phone, Username, Password, Permissions, User::ObjectMode::newMode);
	}
	static User ReadUser(const User & CurrentUser, const std::string& ExistingUsername)
	{

		_Message("Enter First name : ");
		std::string FirstName = Validator::ReadString();

		_Message("Enter Last name : ");
		std::string LastName = Validator::ReadString();

		_Message("Enter Email : ");
		std::string Email = Validator::ReadString();

		
		std::string Phone = Validator::ReadPhoneNumber();

		_Message("Enter Password : ");
		std::string Password = Hasher::GetHash(Validator::ReadString());

		_Message("Enter Permissions : ");
		int32_t Permissions = Authorizer::ReadPermissions(CurrentUser); // function that sets up the permissions


		return User(FirstName, LastName, Email, Phone, ExistingUsername, Password, Permissions, User::ObjectMode::newMode);
	}
	User ReadUser(const User& CurrentUser)
	{
		
		std::string Username = _ReadUsername();

		_Message("Enter First name : ");
		std::string FirstName = Validator::ReadString();

		_Message("Enter Last name : ");
		std::string LastName = Validator::ReadString();

		_Message("Enter Email : ");
		std::string Email = Validator::ReadString();

	
		std::string Phone = Validator::ReadPhoneNumber();

		_Message("Enter Password : ");
		std::string Password = Hasher::GetHash(Validator::ReadString());

		_Message("Enter Permissions : ");
		int32_t Permissions = Authorizer::ReadPermissions(CurrentUser); // function that sets up the permissions


		return User(FirstName, LastName, Email, Phone, Username, Password, Permissions, User::ObjectMode::newMode);
	}

	bool IsExists(const std::string& username, const char * password = nullptr) {

		User user = _FindObject(username, password);
		return (!user.isEmpty());
	}

	static bool IsExists(const std::string& username, const char* password = nullptr) {

		User user = _FindObjectFromFile(username, password);
		return (!user.isEmpty());
	}


	User Find(const std::string& username, const char * password = nullptr) {
		return _FindObject(username, password);
	}

	OperationStates AddUser(User& FilledObject)
	{
		if ( IsExists(FilledObject.GetUsername(), FilledObject.GetPassword().c_str() ))
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

