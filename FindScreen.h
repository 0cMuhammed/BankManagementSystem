#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Client.h"
#include "Repository.h"
#include "Validator.h"

#include "UI.h"
class FindScreen : public UI 
{
private :
	const Repository& m_RepositoryReference; // read only dependency injection

	static void _Message(const std::string& Message) {
		std::cout << "\n" + Message;
	}
	static void _PrintClient(const Client& client) {
		_Message("Client Found!.");
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
	

	void PrintHeader(const std::string* ScreenName = nullptr, const std::string* SubTitle = nullptr) override {
		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? *ScreenName : "Find Client Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << *SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";
	}
	void _PerformMenu(const std::string* NotFoundMessage = nullptr) {

		std::string AccountNumber = Validator::ReadString();
		Client c  = m_RepositoryReference.Find(AccountNumber);
		
		(!c.isEmpty()) ? _PrintClient(c) : _Message(((NotFoundMessage!=nullptr) ? *NotFoundMessage : "Account is not found." ));

	}
	void PerformMenu(const std::string* Message = nullptr) override {

	}
public:
	FindScreen(Repository& Repo) : m_RepositoryReference(Repo) {};

	void Show() override {

	}
};

