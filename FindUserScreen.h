#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Validator.h"
#include "UserServices.h"
#include "MainMenuScreen.h"

#include "UI.h"

class FindUserScreen : public ManageUsersScreen
{
private:
	UserRepository& m_RepositoryReference;

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

		std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Find User Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";
	}
	void PerformMenu(const char* Message = nullptr) override {
		bool isContinue = true;
		do
		{
			_ClearScreen();
			PrintHeader();
			_PerformFind();

			isContinue = Validator::GetConfirmation('\n' + ((Message != nullptr) ? Message : "Do you want to continue this operation?"));

		} while (isContinue);

	}




	void _PerformFind(const char* NotFoundMessage = nullptr) {

		std::string Username = Validator::ReadString();
		User user = m_RepositoryReference.Find(Username);

		(!user.isEmpty()) ? UserRepository::PrintUser(user) : _Message(((NotFoundMessage != nullptr) ? NotFoundMessage : "User is not found."));

	}


public:

	FindUserScreen(UserServices& Ref) : ManageUsersScreen(Ref), m_RepositoryReference(Ref.AccessRepository()) {};

	//well obviously
	void Start() override {
		PerformMenu();
		_GetBackToMenu();
	}
};

