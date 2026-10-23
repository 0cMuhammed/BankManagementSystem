#pragma once
#include "User.h"
#include "Service.h"
#include "Authenticator.h"
#include "Screen.h"

class LoginScreen : public Screen
{
private :     
	                                                               
	static constexpr uint8_t MAX_ATTEMPTS = 3;      
	
    Authenticator& m_Auth;                                        
	                                                             
	User* _Login() {

		uint8_t attempts = 0;

		while (attempts < MAX_ATTEMPTS) 
		{
			term::Prompt("USERNAME");                                // NEW: prints "  USERNAME > " (centered, colored) and stays on the same line
			std::string Username = Validator::ReadString();          // unchanged: reads the whole line the user types

			std::string Password = term::ReadPassword("PASSWORD");   // NEW: same prompt style, but every typed character shows as * (the old code echoed the real password)
			
			attempts++;

			User * user = m_Auth.Login(Username, Password);          // unchanged: the real login check lives in Authenticator, the UI never touches it

			if (user == nullptr)
			{
				term::Error("Invalid username or password. " + std::to_string(MAX_ATTEMPTS - attempts) + " attempt(s) left."); // NEW: red cross + message (std::to_string turns the number into text)
				term::Blank();                                       // NEW: empty line between attempts
			} 
			else 
			{
				return user;
			}

		}

		term::Error("Too many failed attempts. Exiting...");         // NEW: colored version of the old _Message(...)
		return nullptr;
	}

	User* PerformLogin()  {
		_ClearScreen();                                              // unchanged call, but it now paints the screen black instead of running system("cls")
		term::Header("SIGN IN");                           // NEW: replaces the whole old PrintHeader override (9 lines of tabs and underscores). No user name yet, so only title + status.

		return _Login();
	}

public :
	LoginScreen(Service& Ref) : Screen(Ref), m_Auth( Ref.AccessAuthenticator() ) {};

	User Login() override {

		User *user = PerformLogin();

		if ( user == nullptr ) 
		{
			term::WaitKey("Press any key to exit");                 // NEW: without this the window closes instantly and you never see the error above
			exit(0);
		}

		return *user;
	}

};
