#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Client.h"
#include "Repository.h"
#include "Validator.h"

#include "MainMenuScreen.h"
#include "UI.h"
class FindScreen : public MainMenuScreen
{
private :
	const Repository& m_RepositoryReference; // read only dependency injection
	
	//universal 
	 void _Message(const char* Message) override {
		std::cout << '\n' + Message;
	}
	 void _ClearScreen() override {
		system("cls");
	}
	 void _GetBackToMenu(const char* Message = nullptr) override {
		 std::cout << '\n' + (((Message != nullptr) ? Message : "Press Enter to go back to Main Menu")); std::cout << ".....\n";

		 std::cin.clear();
		 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		 std::cin.get();
	 }

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

			isContinue = Validator::GetConfirmation('\n' + ( (Message != nullptr) ? Message : "Do you want to continue this operation?") );

		} while (isContinue);

	}
	

	//exclusive to Opeartions Class
	void _PrintClient(const Client& client) {
		_Message("Client Found!");
		std::cout << "\nClient Card:";
		std::cout << "\n___________________";
		std::cout << "\nFirstName   : " << client.getFirstName();
		std::cout << "\nLastName    : " << client.getLastName();
		std::cout << "\nFull Name   : " << client.getFullName();
		std::cout << "\nEmail       : " << client.getEmail();
		std::cout << "\nPhone       : " << client.getPhoneNumber();
		std::cout << "\nAcc. Number : " << client.getAccountNumber();
		std::cout << "\nPassword    : " << client.getPinCode();
		std::cout << "\nBalance     : " << client.getBalance();
		std::cout << "\n___________________\n";


	}

	 void _PerformFind(const char* NotFoundMessage = nullptr) {

		std::string AccountNumber = Validator::ReadString();
		Client c  = m_RepositoryReference.Find(AccountNumber);
		
		(!c.isEmpty()) ? _PrintClient(c) : _Message( ( (NotFoundMessage != nullptr) ? NotFoundMessage : "Account is not found.") );

	}


public:
	FindScreen(Repository& Repo) : MainMenuScreen(Repo), m_RepositoryReference(Repo) {};
	
	//well obviously
	void Show() override {
		PerformMenu();
		_GetBackToMenu();
	}
};

