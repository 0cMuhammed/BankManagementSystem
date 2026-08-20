#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"
#include "Validator.h"
#include "BankOperations.h"
#include "UI.h"
#include "Operations.h"
#include "Parser.h"
#include<iomanip>

class TotalBalanceScreen : public UI
{
	BankOperations& m_BankOperationsReference;

	void _ClearScreen() {
		system("cls");
	}
	void _Message(const char* Message) {
		std::cout << '\n' + Message;

	}
	void _GetBackToMenu(const char* Message = nullptr) {
		std::cout << '\n' + (((Message != nullptr) ? Message : "Press Enter to go back to Transactions Menu")); std::cout << ".....\n";

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
	}

	static void _PrintLine() {
		std::cout << std::setw(25) << std::left << "" << "\n\t\t_______________________________________________________";
		std::cout << "__________________________\n" <<'\n';
	}
	static void _PrintLayout() {

		_PrintLine();
		std::cout << std::setw(25) << std::left << "" << "| " << std::left << std::setw(15) << "Account Number";
		std::cout << "| " << std::left << std::setw(40) << "Client Name";
		std::cout << "| " << std::left << std::setw(12) << "Balance";
		_PrintLine();
	}
	static void _PrintFormattedClient(const Client& client)
	{

		std::cout << std::setw(25) << std::left << "" << "| " << std::setw(15) << std::left << client.getAccountNumber();
		std::cout << "| " << std:: setw(40) << std::left << client.getFullName();
		std::cout << "| " << std::setw(12) << std::left << client.getBalance();

	}

	void _PrintAll() {
		for (const Client& client : m_BankOperationsReference.AccsessOperations().GetList())
		{

			_PrintFormattedClient(client);
			std::cout << std::endl;
		}
		_PrintLine();

	}
	void _PrintTotalBalance() {
		double total = m_BankOperationsReference.AccsessOperations().GetTotalBalances();
		std::cout << std::setw(8) << std::left << "" << "\t\t\t\t\t\t\t     Total Balances = " << total << '\n';
		std::cout << std::setw(8) << std::left << "" << "\t\t\t\t  ( " << Parser::BalanceToText(total) << ")"; // will get cut off to the an integer 
		
	}

	void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {

		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t  " << (((ScreenName != nullptr) ? ScreenName : "Balances List Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";

	}
	void PerformMenu(const char* Message = nullptr) override {
		if (m_BankOperationsReference.AccsessOperations().GetList().size() == 0)
		{

			_Message("\t\t\t\tNo Clients Available In the System!");


		}
		else
		{
			const std::string SubTitle = "\t    (" + std::to_string(m_BankOperationsReference.AccsessOperations().GetList().size()) + ") Client(s).";

			PrintHeader(nullptr, SubTitle.c_str());
			_PrintLayout();
			_PrintAll();
			_PrintTotalBalance();

		}

	}


};

