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

	    bool _PerformConfirmation(const Client& client,  const char* Message = nullptr) {
			return term::Confirm((Message != nullptr) ? Message : "Are you sure you want to perform this transaction?"); // NEW: single-key Y/N
		}

		void _PrintDepositStatus(const User& CurrentUser, Client &ExistingClient, double amount) {

			std::string AccNum = ExistingClient.getAccountNumber();
			std::string Name = ExistingClient.GetFullName();          // NEW: capture BEFORE Deposit(): it empties the Client object
			double OldBalance = ExistingClient.getBalance();          // NEW: needed for the "old --> new" line

			switch (m_ServicesRef.AccessTransactions().Deposit(ExistingClient, amount))
			{

			case ClientRepository::OperationStates::AccountNumberNotFound:
			{
				term::Error("Account number was not found.");
				break;
			}
			case ClientRepository::OperationStates::Successful:
			{
				Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::Deposit, Logger::Level::INFO, "", AccNum, amount);
				_PrintMoneyReceipt("DEPOSIT COMPLETE", Name, AccNum, OldBalance, ExistingClient.getBalance(), amount, term::col::Green); // NEW: shared receipt (green = money arrived)
				break;
			}

			default:
			{
				break;

			}

			}
		}

		double GetAmount(const Client &client) {
			_PrintClientCard(client);                                 // NEW: replaces ClientRepository::PrintClient
			term::Prompt("DEPOSIT AMOUNT");

			return Validator::returnNumber();
		}

		void _Deposit(const User& CurrentUser, const std::string &AccountNumber) {

			Client client = m_ServicesRef.AccessRepository().Find(AccountNumber);

			if (!client.isEmpty())
			{
				double amount = GetAmount(client);

				term::Blank();
				(_PerformConfirmation(client)) ?  _PrintDepositStatus(CurrentUser, client, amount) : term::Warn("Operation cancelled.");

			}
			else
			{
				term::Error("Account number was not found.");
			}

	    }

		// NEW: returns false when the user has no access, so PerformMenu can stop instead of asking "continue?" after a denial
		bool _PerformDeposit(const User &CurrentUser)  {

			    _ClearScreen();

				if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::Transactions))
				{
					_Header(CurrentUser, "DEPOSIT");

					term::Prompt("ACCOUNT NUMBER");
					std::string AccountNumber = Validator::ReadString();

					_Deposit(CurrentUser, AccountNumber);
					return true;
		     	}
				else
				{
					Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Deposit");
					_Header(CurrentUser, "DEPOSIT");
					NoAccessMsg();
					return false;
				}

		}

		void PerformMenu(const User &CurrentUser, const char* Message = nullptr) override {

			bool IsContinueOperation = true;

			while (IsContinueOperation)
			{
				if (!_PerformDeposit(CurrentUser))
				{
					_GetBackToMenu("Press any key to go back to the Transactions menu");
					return;
				}

				term::Blank();
				IsContinueOperation = term::Confirm((Message != nullptr) ? Message : "Make another deposit?");
			}

		}

		public :

			DepositScreen(Service & Ref) : Screen(Ref), m_ServicesRef(Ref.AccessClientServices()) {};

			void Start(const User& CurrentUser) override {
				PerformMenu(CurrentUser);
			}

};
