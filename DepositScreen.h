#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"
#include "Validator.h"
#include "BankOperations.h"
#include "UI.h"
#include "Operations.h"

class DepositScreen : public UI
{
private :
	   
	   BankOperations& m_BankOperationsReference;

	    void _ClearScreen() {
		   system("cls");
	   }
		void _Message(const char* Message) {
			std::cout << '\n' + Message;

		}
		void _GetBackToMenu(const char* Message = nullptr)  {
			std::cout << '\n' + (((Message != nullptr) ? Message : "Press Enter to go back to Transactions Menu")); std::cout << ".....\n";

			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cin.get();
		}

		void _PrintAmount(const Client &client) {
			_Message("New Balance = ");
			std::cout << client.getBalance() << "\n";

		}

		 bool _PerformConfirmation(const Client& client,  const char* Message = nullptr) {

			
			bool isConfirm = Validator::GetConfirmation('\n' + (((Message != nullptr) ? Message : "Are you sure you want to perform this transaction?")));
			return isConfirm;
		}

		void _PrintDepositStatus(Client &ExistingClient, double amount) {

			switch (m_BankOperationsReference.Deposit(ExistingClient,amount))
			{

			case Operations::OperationStates::AccountNumberNotFound:
			{
				std::cout << "\nAccount Number is not found.";
				break;
			}
			case Operations::OperationStates::Successful:
			{
				std::cout << "\nAmount Deposited Sucessfully.";
				_PrintAmount(ExistingClient);
				break;
			}

			default:
			{
				break;

			}

			}
		}

		double GetAmount(const Client &client) {
			Operations::PrintClient(client);
			_Message("Please enter Deposit amount :");

			return Validator::returnNumber();
		}

		void _Deposit(const std::string &AccountNumber) {

			
			Client client = m_BankOperationsReference.AccsessOperations().Find(AccountNumber);
			
			if (!client.isEmpty()) 
			{
				double amount = GetAmount(client);

				(_PerformConfirmation(client)) ?  _PrintDepositStatus(client, amount) : _Message("Operations is Cancelled.");
				   
			}
			else 
			{
				_Message("Account number is not found");
			}
		   



	    }

		void _PerformDeposit()  {

			
			    _ClearScreen();
				PrintHeader();

				_Message("Please enter your account number");
				std::string AccountNumber = Validator::ReadString();

				_Deposit(AccountNumber);

			

		}

		void PerformMenu(const char* Message = nullptr) override {

			bool IsContinueOperation = true;

			do
			{


				_PerformDeposit();

				IsContinueOperation = Validator::GetConfirmation('\n' + ((Message != nullptr) ? Message : "Do you want to continue this operation?"));

			} while (IsContinueOperation);

		}
		void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
			std::cout << "\t\t\t\t\t______________________________________";

			std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Deposit Screen"));

			if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

			std::cout << "\n\t\t\t\t\t______________________________________\n\n";
		}

		public :

			DepositScreen(BankOperations& Bank) : m_BankOperationsReference(Bank) {};

			void Show() override {
				PerformMenu();
				_GetBackToMenu();
			}

};

