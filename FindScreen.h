#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "ClientRepository.h"
#include "Validator.h"


#include "Screen.h"

class FindScreen : public Screen
{
private :

	ClientRepository& m_RepositoryReference; 
	
	//universal 
	

	void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Find Client Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";
	}
	void PerformMenu(const User &CurrentUser, const char * Message = nullptr) override {

		bool isContinue = true;
		
		do
		{
			_PerformFind(CurrentUser);

			isContinue = Validator::GetConfirmation('\n' + std::string ( ( (Message != nullptr) ? Message : "Do you want to continue this operation?") ));

		} while (isContinue);

	}
	

	

	 void _PerformFind(const User &CurrentUser, const char* NotFoundMessage = nullptr) {

		 _ClearScreen();

		 if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::FindClient)) 
		 {
			 PrintHeader();

			 _Message("Please enter your account number : ");
			 std::string AccountNumber = Validator::ReadString();

			 Client c = m_RepositoryReference.Find(AccountNumber);

			 (!c.isEmpty()) ? ClientRepository::PrintClient(c) : _Message(std::string(((NotFoundMessage != nullptr) ? NotFoundMessage : "Account is not found.\n")));
		 }
		 else 
		 {
			 NoAccessMsg();
		 }

		 

	}


public:
	FindScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessClientServices().AccessRepository()) {};
	
	//well obviously
	void Start(const User &CurrentUser) override {
		PerformMenu(CurrentUser);
		_GetBackToMenu();
	}
};

