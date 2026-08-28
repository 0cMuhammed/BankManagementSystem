#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Validator.h"
#include "ClientServices.h"

#include "Screen.h"

class DepositScreen : public Screen
{
private :
	   
	    ClientServices & m_ServicesRef;


		void _PrintAmount(const Client &client) {
			_Message("New Balance = ");
			std::cout << client.getBalance() << "\n";

		}

	    bool _PerformConfirmation(const Client& client,  const char* Message = nullptr) {

			
			bool isConfirm = Validator::GetConfirmation('\n' + (((Message != nullptr) ? Message : "Are you sure you want to perform this transaction?")));
			return isConfirm;
		}

		void _PrintDepositStatus(Client &ExistingClient, double amount) {

			switch (m_ServicesRef.Deposit(ExistingClient,amount))
			{

			case ClientRepository::AccountNumberNotFound:
			{
				std::cout << "\nAccount Number is not found.";
				break;
			}
			case ClientRepository::Successful:
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
			ClientRepository::PrintClient(client);
			_Message("Please enter Deposit amount :");

			return Validator::returnNumber();
		}

		void _Deposit(const std::string &AccountNumber) {

			
			Client client = m_ServicesRef.AccessRepository().Find(AccountNumber);
			
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

			DepositScreen(Service & Ref) : Screen(Ref), m_ServicesRef(Ref.AccessClientServices()) {};

			void Start() override {
				PerformMenu();
				_GetBackToMenu("Press Enter to go back to Transactions Menu");
			}

};

