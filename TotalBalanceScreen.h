#pragma once
#include <iostream>
#include<vector>
#include<fstream>

#include "Parser.h"
#include "ClientServices.h"

#include "Screen.h"

class TotalBalanceScreen : public Screen
{
private :
	const ClientServices& m_ServicesRef; // read only dependency injection

	// NEW: column titles + thin line (widths must match _PrintRow: 8 + 30 + 16 + 2 gaps of 2 + 2 left = 60)
	static void _PrintLayout() {
		term::Print(std::string(term::col::Muted) + "  " + term::Pad("ACCOUNT", 8) + "  " + term::Pad("NAME", 30) + "  " + term::PadLeft("BALANCE", 16));
		term::Rule();
	}

	static void _PrintRow(const Client& client)
	{
		term::Print(std::string("  ") + term::col::Cyan + term::Pad(client.getAccountNumber(), 8)
		            + term::col::Base + "  " + term::Pad(term::Fit(client.GetFullName(), 30), 30)
		            + term::col::Green + "  " + term::PadLeft(term::Money(client.getBalance()), 16));
	}

	// NEW: splits long text into lines of at most 'width' characters, breaking between words
	static std::vector<std::string> _Wrap(const std::string& text, size_t width) {
		std::vector<std::string> lines;
		std::string line, word;
		for (size_t i = 0; i <= text.size(); ++i) {
			if (i == text.size() || text[i] == ' ') {                  // end of a word
				if (!line.empty() && line.size() + 1 + word.size() > width) { lines.push_back(line); line.clear(); }
				line += (line.empty() ? "" : " ") + word;
				word.clear();
			} else word += text[i];
		}
		if (!line.empty()) lines.push_back(line);
		return lines;
	}

	void _PrintTotalBalance() {
		double total = m_ServicesRef.GetTotalBalances();
		term::Rule();
		term::Print(std::string(term::col::Muted) + "  TOTAL" + term::col::Cyan + term::col::Bold + term::PadLeft(term::Money(total), term::kW - 4 - 5));
		term::Blank();
		bool first = true;
		for (const std::string& l : _Wrap(Parser::BalanceToText(total), 56))   // the total spelled out in words, wrapped to fit
		{
			term::Print(std::string(term::col::Muted) + "  " + (first ? "IN WORDS  " : "          ") + term::col::Base + l);
			first = false;
		}
	}

	void _ShowTotal(const User &CurrentUser)
	{
		const std::vector<Client>& clients = m_ServicesRef.AccessRepository().GetList();

		if (clients.empty())
		{
			_Header(CurrentUser, "TOTAL BALANCES", "0 RECORDS");
			term::Warn("No clients available in the system.");
		}
		else
		{
			_Header(CurrentUser, "TOTAL BALANCES", std::to_string(clients.size()) + " RECORDS");
			_PrintLayout();
			for (const Client& c : clients) _PrintRow(c);
			_PrintTotalBalance();
		}
	}

	void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

		if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::Transactions))
		{
			Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::ShowTotalBalance, Logger::Level::INFO);
			_ShowTotal(CurrentUser);
		}
		else
		{
			Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Total Balances");
			_Header(CurrentUser, "TOTAL BALANCES");
            NoAccessMsg();
		}

	}

public :

	TotalBalanceScreen(Service& Ref) : Screen(Ref), m_ServicesRef(Ref.AccessClientServices()) {};

	void Start(const User & CurrentUser) override {
		PerformMenu(CurrentUser);
		_GetBackToMenu("Press any key to go back to the Transactions menu");
	}

};
