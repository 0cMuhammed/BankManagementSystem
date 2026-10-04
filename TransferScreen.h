#pragma once
#include <iostream>
#include<vector>
#include<fstream>

#include "Validator.h"
#include "Authorizer.h"

#include "Screen.h"

class TransferScreen : public Screen
{
private:

	ClientServices& m_ServicesRef;



	void _PrintBalance(const Client& client) {
		_Message("Your balance is : ");
		std::cout << client.getBalance() << "\n";
	}
	void _PrintWithdrawAmount(double amount) {
		_Message("Amount to withdraw : ");
		std::cout << amount << "\n";
	}

	void _PrintAmountAndBalance(const Client& client, double amount) {
		_PrintWithdrawAmount(amount);
		_PrintBalance(client);

	}

	bool _PerformConfirmation(const Client& client, const char* Message = nullptr) {


		bool isConfirm = Validator::GetConfirmation('\n' + std::string((((Message != nullptr) ? Message : "Are you sure you want to perform this transaction?"))));
		return isConfirm;
	}

	void _PrintWithdrawStatus(const User& CurrentUser, Client& ExistingClient, double amount) {
		std::string AccNum = ExistingClient.getAccountNumber();

		switch (m_ServicesRef.AccessTransactions().Withdraw(ExistingClient, amount))
		{

		case ClientRepository::OperationStates::AccountNumberNotFound:
		{
			_Message("\nAccount Number is not found.\n");
			break;
		}
		case ClientRepository::OperationStates::Successful:
		{
			Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::Withdraw, Logger::Level::INFO, "", AccNum, amount);
			_Message("\nAmount Withdrawn Sucessfully.\n");
			_PrintBalance(ExistingClient);
			break;
		}
		case ClientRepository::OperationStates::InsufficentBalance:
		{
			_Message("\nCannot Withdraw, Insufficent Balance !\n");
			_PrintAmountAndBalance(ExistingClient, amount);
			break;
		}

		default:
		{
			break;

		}

		}
	}

	double GetAmount(const Client& client) {
		ClientRepository::PrintClient(client);
		_Message("Please enter Withdraw amount : ");

		return Validator::returnNumber();
	}

	void _Transfer(const User& CurrentUser, const std::string& AccountNumberFrom, const std::string &AccountNumberTo) {


		Client client = m_ServicesRef.AccessRepository().Find(AccountNumber);

		if (!client.isEmpty())
		{
			double amount = GetAmount(client);

			(_PerformConfirmation(client)) ? _PrintWithdrawStatus(CurrentUser, client, amount) : _Message("\nOperations is Cancelled.\n");

		}
		else
		{
			_Message("\nAccount number is not found.\n");
		}




	}

	void _PerformTransfer(const User& CurrentUser) {


		_ClearScreen();

		if ( Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::Transactions ))
		{
			PrintHeader(CurrentUser);

			_Message("Please enter account number to transfer from : ");
			std::string AccNumFrom = Validator::ReadString();

			_Message("Please enter account number to transfer to: ");
			std::string AccNumTo = Validator::ReadString();

			_Withdraw(CurrentUser, AccNumFrom, AccNumTo);

		}
		else
		{
			Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Withdraw");
			NoAccessMsg();
		}



	}

	void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

		bool IsContinueOperation = true;

		do
		{


			

			IsContinueOperation = Validator::GetConfirmation('\n' + std::string(((Message != nullptr) ? Message : "Do you want to continue this operation?")));

		} while (IsContinueOperation);

	}
	void PrintHeader(const User& CurrentUser, const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Transfer Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";
		ShowUserAndDate(CurrentUser);
	}

public:

	TransferScreen(Service& Ref) : Screen(Ref), m_ServicesRef(Ref.AccessClientServices()) {};

	void Start(const User& CurrentUser) override {
		PerformMenu(CurrentUser);
		_GetBackToMenu("Press Enter to go back to Transactions Menu");
	}

};
{
};

