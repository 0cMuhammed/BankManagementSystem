#pragma once
#include "UserRepository.h"
#include "Hasher.h"



class Authenticator
{
private:

	UserRepository& m_Ref; 
	
	
	bool _IsExistingUser(const User& user, const std::string& username, const std::string& PasswordText) {
		
		return user.GetUsername() == username && ( Hasher::VerifyUser(PasswordText, user.GetPassword()) );
	}

	User* GetUser(const std::string& username, const std::string& PasswordText) {


		for ( User& user : m_Ref.GetMutableList())
		{
			if (_IsExistingUser(user,username,PasswordText))
			{
				return &user;
			}
		}
	     
		return  nullptr;
	}

	

public:

	Authenticator(UserRepository& Ref) : m_Ref(Ref) {};

	User *Login(const std::string& username, const std::string& password)
	{
		return  GetUser(username, password);
	}

};

