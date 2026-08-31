#pragma once
#include "Service.h"
#include "Hasher.h"

class Authenticator
{
private:

	UserRepository& m_Ref;
	

	bool _IsExistingUser(const User& user, const std::string& username, const std::string& PasswordText) {
		
		return user.GetUsername() == username && ( Hasher::VerifyUser(PasswordText, user.GetPassword()) );
	}

	User* GetUser(const std::string& username, const std::string& PasswordText) {

		std::vector<User> Users = m_Ref.GetList();

		for (User& user : Users)
		{
			if (_IsExistingUser(user,username,PasswordText))
			{
				return &user;
			}
		}
		return nullptr;

	}

	

public:

	Authenticator(Service& Service_Ref) : m_Ref(Service_Ref.AccessUserServices().AccessRepository()) {};

	User* Login(const std::string& username, const std::string& password)
	{
		return  GetUser(username, password);
	}

};

