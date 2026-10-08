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


	void _PrintSenderBalance(const Client& client) {
		_Message("New Balance for sender : ");
		std::cout << client.getBalance() << "\n";
	}
	void _PrintReceiverBalance(const Client& client) {
		_Message("New Balance for receiver : ");
		std::cout << client.getBalance() << "\n";
	}

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

	bool _PerformConfirmation(const char* Message = nullptr) {


		bool isConfirm = Validator::GetConfirmation('\n' + std::string((((Message != nullptr) ? Message : "Are you sure you want to perform this transaction?"))));
		return isConfirm;
	}

	void _PrintTransferStatus(const User& CurrentUser, Client& ExistingClientFrom, Client& ExistingClientTo, double amount) {

		//for logging
		std::string AccNumFrom = ExistingClientFrom.getAccountNumber(); 
		std::string AccNumTo = ExistingClientTo.getAccountNumber();
		double FromOldBalance = ExistingClientFrom.getBalance();
		double ToOldBalance = ExistingClientTo.getBalance();

		

		switch (m_ServicesRef.AccessTransactions().Transfer(ExistingClientFrom,ExistingClientTo,amount))
		{

		case ClientRepository::OperationStates::AccountNumberNotFound:
		{
			_Message("\nAccount Number is not found.\n");
			break;
		}
		case ClientRepository::OperationStates::Successful:
		{
			 Logger::LogTransfer(CurrentUser.GetUsername(), Logger::Category::Transfer, Logger::Level::INFO, AccNumFrom, AccNumTo, FromOldBalance, ToOldBalance,ExistingClientFrom.getBalance(),ExistingClientTo.getBalance(), amount);
			 Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::Transfer, Logger::Level::INFO, "", AccNumTo);

			_Message("\nAmount Transferred Sucessfully.\n");
			_PrintSenderBalance(ExistingClientTo);
			_PrintReceiverBalance(ExistingClientFrom);
			break;
		}
		case ClientRepository::OperationStates::InsufficentBalance:
		{
			_Message("\nCannot Withdraw From User : " + AccNumFrom +", Insufficent Balance !\n");
			_PrintAmountAndBalance(ExistingClientFrom, amount);
			break;
		}

		default:
		{
			break;

		}

		}
	}



	void _Transfer(const User& CurrentUser, const std::string& AccountNumberFrom, const std::string &AccountNumberTo) {


		Client From = m_ServicesRef.AccessRepository().Find(AccountNumberFrom);

		if (From.isEmpty())
		{
				_Message("\nAccount number is not found.\n");
				return;
	    }

		Client To = m_ServicesRef.AccessRepository().Find(AccountNumberTo);

		if (To.isEmpty())
		{
			_Message("\nAccount number is not found.\n");
			return;
		}

		if ( !(From.isEmpty() && To.isEmpty()) )
		{
			_Message("Transfer From : ");
			ClientRepository::PrintClient(From);

			_Message("Transfer To : ");
			ClientRepository::PrintClient(To);

			_Message("Please enter Transfer amount : ");
			double amount = Validator::returnNumber();
			
			(_PerformConfirmation()) ? _PrintTransferStatus(CurrentUser, From, To, amount) : _Message("\nOperations is Cancelled.\n");

		}
		
	}

	void _PerformTransfer(const User& CurrentUser) {


		_ClearScreen();

		if ( Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::Transactions ))
		{
			PrintHeader(CurrentUser);

			_Message("Please enter account number to transfer from : ");
			std::string AccNumFrom = Validator::ReadString();

			_Message("Please enter account number to transfer to : ");
			std::string AccNumTo = Validator::ReadString();

			_Transfer(CurrentUser, AccNumFrom, AccNumTo);

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

			_PerformTransfer(CurrentUser);
			
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

