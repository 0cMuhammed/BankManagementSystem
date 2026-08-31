#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Validator.h"


#include "Screen.h"

class FindUserScreen : public Screen
{
private:
	UserRepository& m_RepositoryReference;

	
	

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

			isContinue = Validator::GetConfirmation('\n' + std::string (((Message != nullptr) ? Message : "Do you want to continue this operation?")));

		} while (isContinue);

	}




	void _PerformFind(const char* NotFoundMessage = nullptr) {
		_Message("Please enter a username : ");
		std::string Username = Validator::ReadString();
		User user = m_RepositoryReference.Find(Username);

		(!user.isEmpty()) ? UserRepository::PrintUser(user) : _Message(std::string ( ((NotFoundMessage != nullptr) ? NotFoundMessage : "User is not found.")));

	}


public:

	FindUserScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

	//well obviously
	void Start() override {
		PerformMenu();
		_GetBackToMenu("Press Enter to go back to Manage Users Menu");
	}
};

