#pragma once
#include <iostream>
#include<vector>

#include "ClientRepository.h"
#include "Validator.h"

#include "Screen.h"

class FindScreen : public Screen
{
private :

	ClientRepository& m_RepositoryReference;

	void PerformMenu(const User &CurrentUser, const char * Message = nullptr) override {

		bool isContinue = true;

		while (isContinue)
		{
			if (!_PerformFind(CurrentUser))
			{
				_GetBackToMenu();
				return;
			}

			term::Blank();
			isContinue = term::Confirm((Message != nullptr) ? Message : "Find another client?");
		}

	}

	 bool _PerformFind(const User &CurrentUser, const char* NotFoundMessage = nullptr) {

		 _ClearScreen();

		 if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::FindClient))
		 {
			 _Header(CurrentUser, "FIND CLIENT");

			 term::Prompt("ACCOUNT NUMBER");
			 std::string AccountNumber = Validator::ReadString();

			 Client c = m_RepositoryReference.Find(AccountNumber);
			 Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::FindClient, Logger::Level::INFO, "", AccountNumber);

			 if (!c.isEmpty()) _PrintClientCard(c);
			 else term::Error((NotFoundMessage != nullptr) ? NotFoundMessage : "Account was not found.");
			 return true;
		 }
		 else
		 {
			 Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Find Client");
			 _Header(CurrentUser, "FIND CLIENT");
			 NoAccessMsg();
			 return false;
		 }

	}

public:
	FindScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessClientServices().AccessRepository()) {};

	void Start(const User &CurrentUser) override {
		PerformMenu(CurrentUser);
	}
};
