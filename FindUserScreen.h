#pragma once
#include <iostream>
#include<vector>

#include "Validator.h"

#include "Screen.h"

class FindUserScreen : public Screen
{
private:

	UserRepository& m_RepositoryReference;

	void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {
		bool isContinue = true;

		while (isContinue)
		{
			_ClearScreen();
			_Header(CurrentUser, "FIND USER");

			if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::FindUser))
			{
				_PerformFind(CurrentUser);
			}
			else
			{
				Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Find User");
				NoAccessMsg();
				_GetBackToMenu("Press any key to go back to the Manage Users menu");
				return;
			}

			term::Blank();
			isContinue = term::Confirm((Message != nullptr) ? Message : "Find another user?");
		}

	}

	void _PerformFind(const User& CurrentUser, const char* NotFoundMessage = nullptr) {

		term::Prompt("USERNAME");
		std::string Username = Validator::ReadString();

		User user = m_RepositoryReference.Find(Username);
		Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::FindUser, Logger::Level::INFO, "", Username);

		if (!user.isEmpty()) _PrintUserCard(user);
		else term::Error((NotFoundMessage != nullptr) ? NotFoundMessage : "User was not found.");

	}

public:

	FindUserScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

	void Start(const User& CurrentUser) override {
		PerformMenu(CurrentUser);
	}
};
