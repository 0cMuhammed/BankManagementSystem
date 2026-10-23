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


	// NEW: one line describing one side of the transfer:  FROM  Ahmed Ali   A1001   $12,450.00
	void _PrintParty(const char* label, const Client& client) {
		term::Print(std::string(term::col::Muted) + "  " + term::Pad(label, 6)                              // "FROM" / "TO" in grey
		            + term::col::Base + term::Pad(term::Fit(client.GetFullName(), 20), 22)                  // client name
		            + term::col::Cyan + term::Pad(client.getAccountNumber(), 10)                            // account number in cyan
		            + term::col::Muted + term::PadLeft(term::Money(client.getBalance()), 14));              // current balance
	}

	// NEW: the receipt shown after a successful transfer: old balance --> new balance for BOTH accounts.
	// (This also fixes an old bug: the previous code printed the receiver's balance under the label "sender" and vice versa.)
	// IMPORTANT: names and account numbers are passed in as plain text captured BEFORE the transfer.
	// Why: Transactions::Transfer ends with UpdateClient(), which moves the data out of the Client objects, so after the call
	// their names are empty (balances are numbers, so they survive). Capture first, print later.
	void _PrintReceipt(const std::string& nameFrom, const std::string& accFrom, double fromOld, double fromNew,
	                   const std::string& nameTo,   const std::string& accTo,   double toOld,   double toNew, double amount) {
		term::Blank();
		term::Success("TRANSFER COMPLETE");                                                                   // green tick banner
		term::Blank();
		term::Print(std::string(term::col::Muted) + "  " + term::Pad("FROM", 6)                              // sender row
		            + term::col::Base + term::Pad(term::Fit(nameFrom, 16), 18)
		            + term::col::Cyan + term::Pad(accFrom, 8)
		            + term::col::Muted + term::PadLeft(term::Money(fromOld), 14)                             // OLD balance (grey)
		            + term::col::Cyan + " " + term::g::Rule + term::g::Play + " "                             // the "-->" arrow
		            + term::col::Amber + term::PadLeft(term::Money(fromNew), 14));                           // NEW balance (amber = money left)
		term::Print(std::string(term::col::Muted) + "  " + term::Pad("TO", 6)                                // receiver row
		            + term::col::Base + term::Pad(term::Fit(nameTo, 16), 18)
		            + term::col::Cyan + term::Pad(accTo, 8)
		            + term::col::Muted + term::PadLeft(term::Money(toOld), 14)
		            + term::col::Cyan + " " + term::g::Rule + term::g::Play + " "
		            + term::col::Green + term::PadLeft(term::Money(toNew), 14));                              // NEW balance (green = money arrived)
		term::Blank();
		term::Print(std::string(term::col::Muted) + "  AMOUNT  " + term::col::White + term::col::Bold + term::Money(amount)); // amount moved
	}

	// NEW: replaces _PrintBalance / _PrintWithdrawAmount / _PrintAmountAndBalance (three helpers that printed with raw cout)
	void _PrintAmountAndBalance(const Client& client, double amount) {
		term::Field("AMOUNT", term::Money(amount), term::col::White);
		term::Field("BALANCE", term::Money(client.getBalance()), term::col::Amber);
	}

	bool _PerformConfirmation(const char* Message = nullptr) {


		return term::Confirm((Message != nullptr) ? Message : "Are you sure you want to perform this transaction?"); // NEW: single-key Y/N in the new style (replaces Validator::GetConfirmation)
	}

	void _PrintTransferStatus(const User& CurrentUser, Client& ExistingClientFrom, Client& ExistingClientTo, double amount) {

		//for logging
		std::string AccNumFrom = ExistingClientFrom.getAccountNumber(); 
		std::string AccNumTo = ExistingClientTo.getAccountNumber();
		double FromOldBalance = ExistingClientFrom.getBalance();
		double ToOldBalance = ExistingClientTo.getBalance();
		std::string NameFrom = ExistingClientFrom.GetFullName(); // NEW: capture names BEFORE Transfer() (it empties the Client objects, see _PrintReceipt)
		std::string NameTo = ExistingClientTo.GetFullName();

		

		switch (m_ServicesRef.AccessTransactions().Transfer(ExistingClientFrom,ExistingClientTo,amount))
		{

		case ClientRepository::OperationStates::AccountNumberNotFound:
		{
			term::Error("Account number was not found.");
			break;
		}
		case ClientRepository::OperationStates::Successful:
		{
			 Logger::LogTransfer(CurrentUser.GetUsername(), Logger::Category::Transfer, Logger::Level::INFO, AccNumFrom, AccNumTo, FromOldBalance, ToOldBalance,ExistingClientFrom.getBalance(),ExistingClientTo.getBalance(), amount);
			 Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::Transfer, Logger::Level::INFO, "", AccNumTo);

			_PrintReceipt(NameFrom, AccNumFrom, FromOldBalance, ExistingClientFrom.getBalance(), NameTo, AccNumTo, ToOldBalance, ExistingClientTo.getBalance(), amount); // NEW: replaces 3 old lines
			break;
		}
		case ClientRepository::OperationStates::InsufficentBalance:
		{
			term::Error("Cannot withdraw from account " + AccNumFrom + ": insufficient balance.");
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
				term::Error("Sender account number was not found.");
				return;
	    }

		Client To = m_ServicesRef.AccessRepository().Find(AccountNumberTo);

		if (To.isEmpty())
		{
			term::Error("Receiver account number was not found.");
			return;
		}

		if ( !(From.isEmpty() && To.isEmpty()) )
		{
			term::Blank();
			_PrintParty("FROM", From);                      // NEW: one tidy line per account instead of two big "Client Card" blocks
			_PrintParty("TO", To);
			term::Blank();

			term::Prompt("AMOUNT");                         // NEW: styled prompt, the number is still read by Validator
			double amount = Validator::returnNumber();
			
			term::Blank();
			(_PerformConfirmation()) ? _PrintTransferStatus(CurrentUser, From, To, amount) : term::Warn("Operation cancelled.");

		}
		
	}

	bool _PerformTransfer(const User& CurrentUser) {   // NEW: returns false when the user has no access


		_ClearScreen();

		if ( Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::Transactions ))
		{
			_Header(CurrentUser, "TRANSFER");    // NEW: replaces the PrintHeader override at the bottom of this file

			term::Prompt("FROM ACCOUNT");                  // NEW: styled prompts
			std::string AccNumFrom = Validator::ReadString();

			term::Prompt("TO ACCOUNT");
			std::string AccNumTo = Validator::ReadString();

			_Transfer(CurrentUser, AccNumFrom, AccNumTo);
			return true;
		}
		else
		{
			Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Transfer"); // FIXED: this said "Withdraw" (copy-paste), so the log blamed the wrong operation
			_Header(CurrentUser, "TRANSFER");
			NoAccessMsg();
			return false;
		}



	}

	void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

		bool IsContinueOperation = true;

		while (IsContinueOperation)
		{
			if (!_PerformTransfer(CurrentUser))
			{
				_GetBackToMenu("Press any key to go back to the Transactions menu");
				return;
			}

			term::Blank();
			IsContinueOperation = term::Confirm((Message != nullptr) ? Message : "Make another transfer?");
		}

	}
public:

	TransferScreen(Service& Ref) : Screen(Ref), m_ServicesRef(Ref.AccessClientServices()) {};

	void Start(const User& CurrentUser) override {
		PerformMenu(CurrentUser);
	}



};

