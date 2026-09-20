#pragma once
#include "User.h"
#include "Service.h"
#include "Authenticator.h"
#include "Screen.h"

class LoginScreen : public Screen
{
private :     
	                                                               
	static constexpr uint8_t MAX_ATTEMPTS = 4;      
	
    Authenticator& m_Auth;                                        
	                                                             
	User* _Login() {

		uint8_t attempts = 0;

		while (attempts < MAX_ATTEMPTS) 
		{
			


			_Message("Please enter your username : ");
			std::string Username = Validator::ReadString();
			
			_Message("Please enter a your password : ");
			std::string Password = Validator::ReadString();

			
			attempts++;

			User * user = m_Auth.Login(Username, Password);

			if (user == nullptr)
			{
				std::cout << "\nInvalid username/password ! " << (MAX_ATTEMPTS - attempts) << " Attempt(s) remaining" << "\n";
			} 
			else 
			{
				return user;
			}

		

		}

		
		_Message("\n\nToo Many failed attempts, Exiting...");
		return nullptr;
	}

	void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t\t     " << (((ScreenName != nullptr) ? ScreenName : "Login Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";
	}
	User* PerformLogin(const char* Message = nullptr)  {
		_ClearScreen();
		PrintHeader(Message);

		return _Login();
        
		

	}

public :
	LoginScreen(Service& Ref) : Screen(Ref), m_Auth( Ref.AccessAuthenticator() ) {};

	User Login() override {

		User *user = PerformLogin();

		if ( user == nullptr ) exit(0);

		return *user;
	}

};

