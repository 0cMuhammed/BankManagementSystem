#pragma once
#include <iostream>
#include<vector>
#include <string>

#include "Validator.h"

#include "DepositScreen.h"
#include "WithdrawScreen.h"
#include "TransferScreen.h"
#include "TotalBalanceScreen.h"

#include "Screen.h"

class TransactionsScreen : public Screen
{
private :
	enum MenuComponents { Deposit = 1, Withdraw = 2, Transfer = 3, TotalBalances = 4, BackToMain = 5};

    Session& m_Session;
    Service & m_ServiceRef;

    int m_LastIndex = 0;                          // NEW: remembers the highlighted row

     void _TransactionsLayout() {

        _ClearScreen();
        term::Header("TRANSACTIONS", "", m_Session.GetUser().GetUsername());   // NEW: replaces PrintHeader override + the hand-printed menu box
        term::Section("MENU", std::string(term::g::Up) + term::g::Down + " move  " + term::g::Mid + "  Enter open  " + term::g::Mid + "  Esc back");
    }

	MenuComponents _NavigateUser()
	{
		// NEW: the menu is data. Row order = enum order (row 1 = Deposit ... row 5 = BackToMain).
		static const std::vector<term::MenuItem> items = {
			{ "DEPOSIT",         "add money to an account" },
			{ "WITHDRAW",        "take money out of an account" },
			{ "TRANSFER",        "move money between accounts" },
			{ "TOTAL BALANCES",  "every account, summed up" },
			{ "BACK",            "return to the main menu" }
		};

		int choice = term::Menu(items, m_LastIndex);

		if (choice < 0) return MenuComponents::BackToMain;   // Esc = go back

		m_LastIndex = choice;
		return static_cast<MenuComponents>(choice + 1);
	}

     void _Menu(bool& isInMainMenu) {
         do
         {
             _TransactionsLayout();

             const MenuComponents choice = _NavigateUser();

             if (choice != MenuComponents::BackToMain) _ClearScreen();   // every screen starts on a clean black page

             switch (choice)
             {
             case MenuComponents::Deposit:
             {
                 DepositScreen Deposit(m_ServiceRef);
                 Deposit.Start(m_Session.GetUser());
                 break;

             }
             case MenuComponents::Withdraw:
             {
                 WithdrawScreen Withdraw(m_ServiceRef);
                 Withdraw.Start(m_Session.GetUser());
                 break;
             }
             case MenuComponents::Transfer:
             {
                 TransferScreen Transfers(m_ServiceRef);
                 Transfers.Start(m_Session.GetUser());
                 break;
             }
             case MenuComponents::TotalBalances:
             {
                 TotalBalanceScreen TotalBalances(m_ServiceRef);
                 TotalBalances.Start(m_Session.GetUser());
                 break;
             }
             case MenuComponents::BackToMain:
             {
                 isInMainMenu = false;        // NEW: leave the loop right away (the old "wait + Getting back..." message is gone)
                 break;

             }
             default: //for later enum choices
             {
                 break;

             }
             }

         } while (isInMainMenu);
     }

    void PerformMenu(const User &CurrentUser, const char* Message = nullptr) override {

        bool isInMainMenu = true;

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::Transactions))
        {
            _Menu(isInMainMenu);
        }
        else
        {
            Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Transactions");
            _Header(CurrentUser, "TRANSACTIONS");
            NoAccessMsg();
            _GetBackToMenu();
        }

    }

public :

    TransactionsScreen(Service& Ref, Session& SessionRef) : Screen(Ref), m_Session(SessionRef), m_ServiceRef(Ref) {};

    void Start(const User &CurrentUser) override {
        PerformMenu(CurrentUser);

    }

};
