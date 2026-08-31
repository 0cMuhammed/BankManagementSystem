#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include<iomanip>

#include "Parser.h"
#include "ClientServices.h"


#include "Screen.h"



class TotalBalanceScreen : public Screen
{
private :
	const ClientServices& m_ServicesRef; // read only dependency injection


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
		std::cout << "| " << std:: setw(40) << std::left << client.GetFullName();
		std::cout << "| " << std::setw(12) << std::left << client.getBalance();

	}

	void _PrintAll() {
		for (const Client& client : m_ServicesRef.AccessRepository().GetList())
		{

			_PrintFormattedClient(client);
			std::cout << std::endl;
		}
		_PrintLine();

	}
	void _PrintTotalBalance() {
		double total = m_ServicesRef.GetTotalBalances();
		std::cout << std::setw(8) << std::left << "" << "\t\t\t\t\t\t\t     Total Balances = " << total << '\n';
		std::cout << std::setw(8) << std::left << "" << "\t\t\t\t  ( " << Parser::BalanceToText(total) << ")\n\n"; // will get cut off to the an integer 
		
	}

	void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {

		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t\t   " << (((ScreenName != nullptr) ? ScreenName : "Balances List Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";

	}
	void PerformMenu(const char* Message = nullptr) override {
		if (m_ServicesRef.AccessRepository().GetList().size() == 0)
		{

			_Message("\t\t\t\tNo Clients Available In the System!");


		}
		else
		{
			const std::string SubTitle = "\t    (" + std::to_string(m_ServicesRef.AccessRepository().GetList().size()) + ") Client(s).";

			PrintHeader(nullptr, SubTitle.c_str());
			_PrintLayout();
			_PrintAll();
			_PrintTotalBalance();

		}

	}

public :

	TotalBalanceScreen(Service& Ref) : Screen(Ref), m_ServicesRef(Ref.AccessClientServices()) {};

	void Start() override {
		PerformMenu();
		_GetBackToMenu("Press Enter to go back to Transactions Menu");
	}

};

