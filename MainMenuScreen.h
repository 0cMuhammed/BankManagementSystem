#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include <string>
#include "Validator.h"
#include "Service.h"
#include "ListScreen.h"
#include "AddScreen.h"
#include "DeleteScreen.h"
#include "UpdateScreen.h"
#include "FindScreen.h"
#include "TransactionsScreen.h"
#include "ManageUsersScreen.h"
#include "LoggerScreen.h"
#include "CurrencyExchangeMainScreen.h"
#include "Screen.h"


class MainMenuScreen : public Screen
{

private:

    enum MainMenuComponents { List = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Transactions = 6, ManageUsers = 7, Logs = 8, CurrencyExchange = 9, Logout = 10 };

    Session& m_Session;
    Service& m_ServicesRef;

    int m_LastIndex = 0;   // NEW: remembers which menu row was highlighted, so coming back from a screen puts the highlight where you left it


    // NEW: the three numbers at the top of the dashboard.
    // They are real values from your services. A user WITHOUT permission sees "---" instead (a dashboard must not leak what a screen would refuse to show).
    void _PrintStats()
    {
        const User& user = m_Session.GetUser();                                                    // who is logged in

        const bool canSeeClients = Authorizer::HasAccess(user, Authorizer::Permissions::ShowClientList); // same permission ListScreen checks
        const bool canSeeUsers   = Authorizer::HasAccess(user, Authorizer::Permissions::ShowUserList);   // same permission ListUserScreen checks

        std::string balance = "---";                                                               // default text when the user may not see it
        std::string clients = "---";
        std::string team    = "---";

        if (canSeeClients)
        {
            balance = term::Money(m_ServicesRef.AccessClientServices().GetTotalBalances());        // sum of all balances, formatted as $104,601.25
            clients = std::to_string(m_ServicesRef.AccessClientServices().AccessRepository().GetList().size()); // number of clients, as text
        }
        if (canSeeUsers)
        {
            team = std::to_string(m_ServicesRef.AccessUserServices().AccessRepository().GetList().size());      // number of staff users, as text
        }

        term::Print(std::string(term::col::Muted) + "  " + term::Pad("TOTAL BALANCE", 24) + term::Pad("CLIENTS", 18) + "TEAM"); // grey labels; Pad() makes each label exactly N columns wide so the numbers below line up
        term::Print(std::string(term::col::Cyan) + "  " + term::Pad(balance, 24)                                                  // cyan balance
                    + term::col::Magenta + term::Pad(clients, 18)                                                                // magenta client count
                    + term::col::Amber + team);                                                                                  // amber team count
        term::Blank();
    }

    void _MainMenuLayout()
    {
        _ClearScreen();                                              // black, instantly

        term::Header("BankManagementSystem", "", m_Session.GetUser().GetUsername()); // title + status + "admin . date" line (replaces the old PrintHeader override + ShowUserAndDate)

        _PrintStats();                                               // the 3 dashboard numbers

        term::Section("MODULES", std::string(term::g::Up) + term::g::Down + " move  " + term::g::Mid + "  Enter open  " + term::g::Mid + "  1-9 / 0 jump"); // small title + key hints
    }

    MainMenuComponents _NavigateUser()
    {
        // NEW: the old code printed "[1] Show Client List." ten times by hand and read a number.
        // Now the menu is DATA (a list of label + hint) and term::Menu draws it and handles the keyboard.
        // The ORDER matters: row 1 = enum value 1 (List), row 2 = 2 (Add) ... row 10 = 10 (Logout).
        static const std::vector<term::MenuItem> items = {
            { "SHOW CLIENTS",      "view every account" },
            { "ADD CLIENT",        "open a new account" },
            { "DELETE CLIENT",     "close an account" },
            { "UPDATE CLIENT",     "edit account details" },
            { "FIND CLIENT",       "look up one account" },
            { "TRANSACTIONS",      "deposit, withdraw, transfer" },
            { "MANAGE USERS",      "staff accounts and permissions" },
            { "LOGS",              "activity and audit history" },
            { "CURRENCY EXCHANGE", "rates and converter" },
            { "LOGOUT",            "sign out of this session" }
        };

        int choice = term::Menu(items, m_LastIndex);                 // draws the menu, waits for Enter (returns 0-9) or Esc (returns -1)

        if (choice < 0)                                              // Esc pressed
            return static_cast<MainMenuComponents>(0);               // 0 matches no case in the switch below, so nothing happens and the menu is redrawn

        m_LastIndex = choice;                                        // remember the highlighted row
        return static_cast<MainMenuComponents>(choice + 1);          // row index 0-9 -> enum value 1-10 (what your switch already expects)
    }

    void _Menu(bool& isInMainMenu) {
        do
        {
            _MainMenuLayout();

            const MainMenuComponents choice = _NavigateUser();       // NEW: stored first so we can clear the screen BEFORE opening the chosen screen

            if (choice != static_cast<MainMenuComponents>(0))        // a real choice (not Esc)
                _ClearScreen();                                      // NEW: every screen now starts on a clean black page, even the ones not converted yet

            switch (choice)
            {
            case MainMenuComponents::List:
            {
                ListScreen List(m_ServicesRef);
                List.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::Add:
            {
                AddScreen Add(m_ServicesRef);
                Add.Start(m_Session.GetUser());
                break;
            }
            case MainMenuComponents::Delete:
            {
                DeleteScreen Delete(m_ServicesRef);
                Delete.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::Update:
            {
                UpdateScreen Update(m_ServicesRef);
                Update.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::Find:
            {
                FindScreen Find(m_ServicesRef);
                Find.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::Transactions:
            {
                TransactionsScreen Transactions(m_ServicesRef, m_Session);
                Transactions.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::ManageUsers:
            {
                ManageUsersScreen ManageUsers(m_ServicesRef, m_Session);
                ManageUsers.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::Logs:
            {
                LoggerScreen Logs(m_ServicesRef, m_Session);
                Logs.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::CurrencyExchange:
            {
                CurrencyExchangeMainScreen CurrencyExchange(m_ServicesRef, m_Session);
                CurrencyExchange.Start(m_Session.GetUser());
                break;
            }
            case MainMenuComponents::Logout:
            {
                m_Session.Logout(isInMainMenu);
                break;

            }
            default: //Esc (0) or later enum choices
            {
                break;

            }



            }

        } while (isInMainMenu);
    }

    void PerformMainMenu(Session& CurrentUser, const char* Message = nullptr) override {

        bool isInMainMenu = true;

        if (CurrentUser.isActive())
        {


            _Menu(isInMainMenu);
        }
        else
        {
            NoAccessMsg();
            CurrentUser.SetIsActive(false);
        }


    }




public:

    MainMenuScreen(Service& Ref, Session& CurrentUser) : Screen(Ref), m_Session(CurrentUser), m_ServicesRef(Ref) {};


    void StartMainMenu(Session& CurrentUser) override {
        PerformMainMenu(CurrentUser);
    }

};

