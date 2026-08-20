#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"
#include "Validator.h"
#include "BankOperations.h"
#include "UI.h"
#include "Operations.h"

class WithdrawScreen : public UI
{
private:

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

	void _PrintBalance(const Client& client) {
		_Message("Your balance is :");
		std::cout << client.getBalance() << "\n";
	}
	void _PrintWithdrawAmount(double amount ) {
		_Message("Amount to withdraw :");
		std::cout << amount << "\n";
	}

	void _PrintAmountAndBalance(const Client& client, double amount) {
		_PrintWithdrawAmount(amount);
		_PrintBalance(client);

	}
	
	bool _PerformConfirmation(const Client& client, const char* Message = nullptr) {


		bool isConfirm = Validator::GetConfirmation('\n' + (((Message != nullptr) ? Message : "Are you sure you want to perform this transaction?")));
		return isConfirm;
	}

	void _PrintWithdrawStatus(Client& ExistingClient, double amount) {

		switch (m_BankOperationsReference.Withdraw(ExistingClient, amount))
		{

		case Operations::OperationStates::AccountNumberNotFound:
		{
			_Message("Account Number is not found.");
			break;
		}
		case Operations::OperationStates::Successful:
		{
			_Message("Amount Withdrawn Sucessfully.");
			_PrintBalance(ExistingClient);
			break;
		}
		case Operations::OperationStates::InsufficentBalance:
		{
			_Message("Cannot Withdraw, Insufficent Balance !");
			_PrintAmountAndBalance(ExistingClient,amount);
			break;
		}

		default:
		{
			break;

		}

		}
	}

	double GetAmount(const Client& client) {
		Operations::PrintClient(client);
		_Message("Please enter Withdraw amount :");

		return Validator::returnNumber();
	}

	void _Withdraw(const std::string& AccountNumber) {


		Client client = m_BankOperationsReference.AccsessOperations().Find(AccountNumber);

		if (!client.isEmpty())
		{
			double amount = GetAmount(client);

			(_PerformConfirmation(client)) ? _PrintWithdrawStatus(client, amount) : _Message("Operations is Cancelled.");

		}
		else
		{
			_Message("Account number is not found");
		}




	}

	void _PerformWithdraw() {


		_ClearScreen();
		PrintHeader();

		_Message("Please enter your account number");
		std::string AccountNumber = Validator::ReadString();

		_Withdraw(AccountNumber);



	}

	void PerformMenu(const char* Message = nullptr) override {

		bool IsContinueOperation = true;

		do
		{


			_PerformWithdraw();

			IsContinueOperation = Validator::GetConfirmation('\n' + ((Message != nullptr) ? Message : "Do you want to continue this operation?"));

		} while (IsContinueOperation);

	}
	void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Deposit Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";
	}

public:

	WithdrawScreen(BankOperations& Bank) : m_BankOperationsReference(Bank) {};

	void Show() override {
		PerformMenu();
		_GetBackToMenu();
	}

};


