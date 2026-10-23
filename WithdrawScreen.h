#pragma once
#include <iostream>
#include<vector>
#include<fstream>

#include "Validator.h"
#include "ClientServices.h"

#include "Screen.h"

class WithdrawScreen : public Screen
{
private:

	ClientServices& m_ServicesRef;

	bool _PerformConfirmation(const Client& client, const char* Message = nullptr) {
		return term::Confirm((Message != nullptr) ? Message : "Are you sure you want to perform this transaction?"); // NEW: single-key Y/N
	}

	void _PrintWithdrawStatus(const User& CurrentUser, Client& ExistingClient, double amount) {
		std::string AccNum = ExistingClient.getAccountNumber();
		std::string Name = ExistingClient.GetFullName();          // NEW: capture BEFORE Withdraw(): it empties the Client object
		double OldBalance = ExistingClient.getBalance();

		switch (m_ServicesRef.AccessTransactions().Withdraw(ExistingClient, amount))
		{

		case ClientRepository::OperationStates::AccountNumberNotFound:
		{
			term::Error("Account number was not found.");
			break;
		}
		case ClientRepository::OperationStates::Successful:
		{
			Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::Withdraw, Logger::Level::INFO, "", AccNum, amount);
			_PrintMoneyReceipt("WITHDRAWAL COMPLETE", Name, AccNum, OldBalance, ExistingClient.getBalance(), amount, term::col::Amber); // amber = money left
			break;
		}
		case ClientRepository::OperationStates::InsufficentBalance:
		{
			term::Error("Cannot withdraw: insufficient balance.");
			term::Field("AMOUNT", term::Money(amount), term::col::White);                              // NEW: replaces _PrintAmountAndBalance (3 helper functions)
			term::Field("BALANCE", term::Money(ExistingClient.getBalance()), term::col::Amber);
			break;
		}

		default:
		{
			break;

		}

		}
	}

	double GetAmount(const Client& client) {
		_PrintClientCard(client);
		term::Prompt("WITHDRAW AMOUNT");

		return Validator::returnNumber();
	}

	void _Withdraw(const User& CurrentUser, const std::string& AccountNumber) {

		Client client = m_ServicesRef.AccessRepository().Find(AccountNumber);

		if (!client.isEmpty())
		{
			double amount = GetAmount(client);

			term::Blank();
			(_PerformConfirmation(client)) ? _PrintWithdrawStatus(CurrentUser, client, amount) : term::Warn("Operation cancelled.");

		}
		else
		{
			term::Error("Account number was not found.");
		}

	}

	// NEW: returns false when the user has no access
	bool _PerformWithdraw(const User &CurrentUser) {

		_ClearScreen();

		if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::Transactions))
		{
			_Header(CurrentUser, "WITHDRAW");

			term::Prompt("ACCOUNT NUMBER");
			std::string AccountNumber = Validator::ReadString();

			_Withdraw(CurrentUser, AccountNumber);
			return true;
		 }
		else
		{
			Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Withdraw");
			_Header(CurrentUser, "WITHDRAW");
			NoAccessMsg();
			return false;
		}

	}

	void PerformMenu(const User &CurrentUser, const char* Message = nullptr) override {

		bool IsContinueOperation = true;

		while (IsContinueOperation)
		{
			if (!_PerformWithdraw(CurrentUser))
			{
				_GetBackToMenu("Press any key to go back to the Transactions menu");
				return;
			}

			term::Blank();
			IsContinueOperation = term::Confirm((Message != nullptr) ? Message : "Make another withdrawal?");
		}

	}

public:

	WithdrawScreen(Service& Ref) : Screen(Ref), m_ServicesRef(Ref.AccessClientServices()) {};

	void Start(const User& CurrentUser) override {
		PerformMenu(CurrentUser);
	}

};
