#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "User.h"
#include "FileHandler.h"
#include "Validator.h"
#include "Global.h"



using Mode = User::ObjectMode;


class UserRepository {

public:

	enum  OperationStates { Failed = 1, Successful = 2, UserAlreadyExists = 3, UserNotFound = 4, UserIsAdmin = 5, SuccessfulSelfDelete = 6, SuccessfulSelfUpdate = 7 };

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
		return  User("", "", "", "", "", "", 0, Mode::newMode);
	}

	void _MakeEmptyIfToBeDeleted(User& user) {
		if (user.GetMode() == Mode::DeleteMode)
		{
			user = _GetEmptyObject();
		}

	}
	void _EmptyObject(User& user) {

		user = _GetEmptyObject();
	}

	static bool _IsModifiable(const User& user, const std::string& username, const std::string& password) {
		return (user.GetUsername() == username && (Hasher::VerifyUser(password, user.GetPassword()) || password == user.GetPassword())) && user.GetMode() == Mode::ExistingMode;
	}


	std::string  _ReadUsername() {

		_Message("Enter your username : ");
		std::string Username = Validator::ReadString(); 

			while (IsExists(Username) || Validator::IsInvalid(Username)) {

				_Message("Username is already used/invalid, Please choose another username : ");

				Username = Validator::ReadString();
			}

		return Username;
	}




	static std::string  s_ReadUsername() {

		_Message("Enter your username : ");
		std::string Username = Validator::ReadString();

		while (IsExistsInFile(Username) || Validator::IsInvalid(Username) ) {

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
				_MakeEmptyIfToBeDeleted(c);
				return true;
			}

		}
	

		return false;

	}

	bool _DeleteObject(const User& user) {
	

		for (User& c : m_List)
		{
			if ( _IsModifiable( c, user.GetUsername(), user.GetPassword() ) )
			{

				c.SetMode(Mode::DeleteMode);
				_MakeEmptyIfToBeDeleted(c);
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
	bool _UpdateObject(const User &OldUser, User & NewUser) {


		for (User & c : m_List)
		{

			if (_IsModifiable(c, OldUser.GetUsername(), OldUser.GetPassword()))
			{
				c = std::move(NewUser);
				return true;
			}
				
		}

		return false;

	}



	static User _FindObjectFromFile(const std::string& username)  {


		std::vector<User> users = FileHandler::LoadUsers();

		for (const User& user: users)
		{
			bool usernameMatch = user.GetUsername() == username;

			if (usernameMatch)
			{
				return user;

			}
		}


		return _GetEmptyObject();
	}
	User _FindObject(const std::string& username) {

		for (const User& user : m_List)
		{
			if (user.GetUsername() == username)
			{
				return user;

			}
		}


		return _GetEmptyObject();
	}
	

public:

	UserRepository(std::vector<User> Users) : m_List(std::move(Users)) {};


	const std::vector<User>& GetList() const {
		return m_List; // read only 
	}
	std::vector<User>& GetMutableList()  noexcept {
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
		std::cout << "\nPermissions : " << std::to_string(user.GetPermissions());
		std::cout << "\n___________________\n";


	}

	static bool IsPasswordsChanged(const User& Old, const std::string &NewHash) {
		return Old.GetPassword() != NewHash;
	}
	static bool IsPermissionsChanged(const User& Old, const User& New) {
		bool IsAllPermissions = Old.GetPermissions() == FullPermissions && New.GetPermissions() == FullPermisssionsInPositive || New.GetPermissions() == FullPermissions && Old.GetPermissions() == FullPermisssionsInPositive;
		return IsAllPermissions ? false : Old.GetPermissions() != New.GetPermissions();
	}
	static bool IsSelf(const User& CurrentUser, const std::string& OtherUsername) {
		return CurrentUser.GetUsername() == OtherUsername;
	}
	static bool IsUsernameDifferent(const std::string&Username, const std::string&OtherUsername) {
		return Username == OtherUsername;
	}
	static bool IsUsernameDifferent(const User & user, const User &OtherUser) {
		return user.GetUsername() != OtherUser.GetUsername();
	}

	static User ReadNewUser(int32_t permbits)
	{
		
		std::string Username =  s_ReadUsername(); // file 



		std::string FirstName = Validator::ReadNames("First");

		std::string LastName = Validator::ReadNames("Last");


		std::string Email = Validator::ReadEmails();


		std::string Phone = Validator::ReadPhoneNumber();

		_Message("Enter Password : ");
		std::string Password = Hasher::GetHash(Validator::ReadString());


		

		return User(FirstName, LastName, Email, Phone, Username, Password, permbits, User::ObjectMode::newMode);
	}
	 User ReadUser(int32_t permbits)
	{

		std::string Username = _ReadUsername(); // Vector


		
		std::string FirstName = Validator::ReadNames("First");

		std::string LastName = Validator::ReadNames("Last");

	
		std::string Email = Validator::ReadEmails();


		std::string Phone = Validator::ReadPhoneNumber();

		_Message("Enter Password : ");
		std::string Password = Hasher::GetHash(Validator::ReadString());




		return User(FirstName, LastName, Email, Phone, Username, Password, permbits, User::ObjectMode::newMode);
	}
	static User UpdateExistingUser(const User&target, const std::string& ExistingUsername, int32_t permbits)
	{

		std::string FirstName;
		std::string LastName;
		std::string email;
		std::string phone;
		std::string Password;
		std::string Username;


		if (Validator::GetConfirmation("\nDo you want to change your First name ? y/n : "))
		{


			_Message("Enter your new first name : \n");
			FirstName = Validator::ReadNames("First");
		}
		else
		{
			FirstName = target.GetFirstName();
		}
		if (Validator::GetConfirmation("\nDo you want to change your last name ? y/n : "))
		{


			_Message("Enter your new last name : \n");
			LastName = Validator::ReadNames("Last");
		}
		else
		{
			LastName = target.GetLastName();
		}
		if (Validator::GetConfirmation("\nDo you want to change your email ? y/n : "))
		{


			_Message("Enter your new email name : \n");
			email = Validator::ReadEmails();
		}
		else
		{
			email = target.GetEmail();
		}

		if (Validator::GetConfirmation("\nDo you want to change your phone number ? y/n : "))
		{


			_Message("Enter your new phone number : \n");
			phone = Validator::ReadPhoneNumber();
		}
		else
		{
			phone = target.GetPhoneNumber();
		}

		if (Validator::GetConfirmation("\nDo you want to change your username? y/n : "))
		{


			_Message("Enter your new username: \n");
			Username = s_ReadUsername();
		}
		else
		{
			Username = target.GetUsername();
		}

		if (Validator::GetConfirmation("\nDo you want to change your password? y/n : "))
		{


			_Message("Enter your new Password : ");
			Password = Hasher::GetHash(Validator::ReadString());
		}
		else
		{
		    Password = target.GetPassword();   
		}

		
	
		return User(FirstName, LastName, email, phone, Username, Password, permbits, User::ObjectMode::ExistingMode);
	}


	bool IsExists(const std::string& username) {

		User user = _FindObject(username);
		return (!user.isEmpty());
	}

	static bool IsExistsInFile(const std::string& username) {

		User user = _FindObjectFromFile(username);
		return (!user.isEmpty());
	}
	static bool IsAdmin(const User& user) {
		return	user.GetUsername() == ADMIN_USERNAME && user.GetPassword() == std::string(ADMIN_HASH);
	}
	static bool IsAdmin(const std::string& username, const std::string& password) {
		return	username == ADMIN_USERNAME && password == std::string(ADMIN_HASH);
	}


	User Find(const std::string& username) {
		return _FindObject(username);
	}

	OperationStates AddUser(User& FilledObject)
	{
		if ( IsExists(FilledObject.GetUsername()))
		{
			return OperationStates::UserAlreadyExists;
		}


		FilledObject.SetMode(Mode::newMode);
		FilledObject.Save();
		_AddInVector(FilledObject);



		return OperationStates::Successful;

	}

	OperationStates DeleteUser(User & CurrentUser,const User& ExistingObject) {

		if (IsAdmin(ExistingObject)) 
		{
		   return OperationStates::UserIsAdmin;
		}
	

		if (!_DeleteObject(ExistingObject))
		{
			return OperationStates::Failed;

		}

		


		FileHandler::SaveUsers(m_List);
		_UpdateVector();

		if (IsSelf(CurrentUser,ExistingObject.GetUsername()))
		{
			CurrentUser.SetMode(Mode::DeleteMode);
			_MakeEmptyIfToBeDeleted(CurrentUser);

			return OperationStates::SuccessfulSelfDelete;
		}

		return OperationStates::Successful;

	}
	OperationStates DeleteUser(User &CurrentUser,const std::string& username, const std::string & password) {



		if (!IsExists(username))
		{
			return OperationStates::UserNotFound;
		}
		if (IsAdmin(username,password))
		{
			return OperationStates::UserIsAdmin;
		}
		if (! _DeleteObject(username,password))
		{
			return OperationStates::Failed;

		}



		FileHandler::SaveUsers(m_List);
		_UpdateVector();

		if (IsSelf(CurrentUser,username))
		{
			CurrentUser.SetMode(Mode::DeleteMode);
			_MakeEmptyIfToBeDeleted(CurrentUser);

			return OperationStates::SuccessfulSelfDelete;
		}

		return OperationStates::Successful;

	}

	OperationStates UpdateUser(const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newUsername, const std::string newPassword, const std::string& OldUsername, const std::string &OldPassword) {

		if (!IsExists( OldUsername ))
		{
			return OperationStates::UserNotFound;
		}
		if (IsAdmin(OldUsername, OldPassword)) 
		{
			return OperationStates::UserIsAdmin;
		}
		

		if (!_UpdateObject(newFirstName, newLastName, newEmail, newPhoneNumber, newUsername,newPassword,OldUsername,OldPassword))
		{
			return  OperationStates::UserNotFound;

		}

		FileHandler::SaveUsers(m_List);

		return OperationStates::Successful;

	}

	OperationStates UpdateUser(User& CurrentUser, const User& OldUser, User& ExistingObject) {


		bool ArePasswordsDifferent = IsPasswordsChanged(OldUser, ExistingObject.GetPassword());
		bool ArePermissionsDifferent = IsPermissionsChanged(OldUser, ExistingObject);
		bool AreUsernamesDifferent = IsUsernameDifferent(OldUser, ExistingObject);

		if (IsAdmin(OldUser))
		{
			return OperationStates::UserIsAdmin;
		}

		User Temp = ExistingObject;

		if (!_UpdateObject(OldUser, ExistingObject))
		{
			return OperationStates::Failed;
		}

		FileHandler::SaveUsers(m_List);

		if (IsSelf(CurrentUser, OldUser.GetUsername() ))
		{
			if (ArePasswordsDifferent || ArePermissionsDifferent || AreUsernamesDifferent)
			{
				_EmptyObject(CurrentUser);

				return OperationStates::SuccessfulSelfUpdate;
			}
			
			 CurrentUser = Temp; // if changes are not passwords or permissions then refresh all the other changes in sync
			

		}
		
			return OperationStates::Successful;
		

	}


};
