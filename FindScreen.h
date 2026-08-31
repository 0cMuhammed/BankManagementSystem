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
	void PerformMenu(const char * Message = nullptr) override {
		bool isContinue = true;
		do
		{
			_ClearScreen();
			PrintHeader();
			_PerformFind();

			isContinue = Validator::GetConfirmation('\n' + std::string ( ( (Message != nullptr) ? Message : "Do you want to continue this operation?") ));

		} while (isContinue);

	}
	

	

	 void _PerformFind(const char* NotFoundMessage = nullptr) {
		 _Message("Please enter your account number : ");
		std::string AccountNumber = Validator::ReadString();
		Client c  = m_RepositoryReference.Find(AccountNumber);
		
		(!c.isEmpty()) ? ClientRepository::PrintClient(c) : _Message( std::string( ( (NotFoundMessage != nullptr) ? NotFoundMessage : "Account is not found.\n") ));

	}


public:
	FindScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessClientServices().AccessRepository()) {};
	
	//well obviously
	void Start() override {
		PerformMenu();
		_GetBackToMenu();
	}
};

