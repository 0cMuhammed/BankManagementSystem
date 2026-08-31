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


		bool isConfirm = Validator::GetConfirmation('\n' + std::string ( (((Message != nullptr) ? Message : "Are you sure you want to perform this transaction?"))));
		return isConfirm;
	}

	void _PrintWithdrawStatus(Client& ExistingClient, double amount) {

		switch (m_ServicesRef.Withdraw(ExistingClient, amount))
		{

		case ClientRepository::OperationStates::AccountNumberNotFound:
		{
			_Message("\nAccount Number is not found.\n");
			break;
		}
		case ClientRepository::OperationStates::Successful:
		{
			_Message("\nAmount Withdrawn Sucessfully.\n");
			_PrintBalance(ExistingClient);
			break;
		}
		case ClientRepository::OperationStates::InsufficentBalance:
		{
			_Message("\nCannot Withdraw, Insufficent Balance !\n");
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
		ClientRepository::PrintClient(client);
		_Message("Please enter Withdraw amount : ");

		return Validator::returnNumber();
	}

	void _Withdraw(const std::string& AccountNumber) {


		Client client = m_ServicesRef.AccessRepository().Find(AccountNumber);

		if (!client.isEmpty())
		{
			double amount = GetAmount(client);

			(_PerformConfirmation(client)) ? _PrintWithdrawStatus(client, amount) : _Message("\nOperations is Cancelled.\n");

		}
		else
		{
			_Message("\nAccount number is not found.\n");
		}




	}

	void _PerformWithdraw() {


		_ClearScreen();
		PrintHeader();

		_Message("Please enter your account number : ");
		std::string AccountNumber = Validator::ReadString();

		_Withdraw(AccountNumber);



	}

	void PerformMenu(const char* Message = nullptr) override {

		bool IsContinueOperation = true;

		do
		{


			_PerformWithdraw();

			IsContinueOperation = Validator::GetConfirmation('\n' +  std::string (((Message != nullptr) ? Message : "Do you want to continue this operation?")));

		} while (IsContinueOperation);

	}
	void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Withdraw Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";
	}

public:

	WithdrawScreen(Service& Ref) : Screen(Ref), m_ServicesRef(Ref.AccessClientServices()) {};

	void Start() override {
		PerformMenu();
		_GetBackToMenu("Press Enter to go back to Transactions Menu");
	}

};


