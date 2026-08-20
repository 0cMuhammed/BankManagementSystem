#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include <string>
#include "Client.h"
#include "Repository.h"
#include "Validator.h"

#include "DepositScreen.h"
#include "WithdrawScreen.h"
#include "TotalBalanceScreen.h"

#include "UI.h"

class TransactionsScreen : public MainMenuScreen
{
private :
	enum TransactionsMenuComponents { Deposit = 1, Withdraw = 2, TotalBalances = 3, BackToMain = 4};

    Repository& m_RepositoryReference;

	void _GetBackToMenu(const char* Message = nullptr) override {
		std::cout << '\n' + (((Message != nullptr) ? Message : "Press Enter to go back to Main Menu")); std::cout << ".....\n";

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get();
	}
	void _ClearScreen() override {
		system("cls");
	}
	void _Message(const char* Message = nullptr) override {
		std::cout << '\n' + Message;
	}
    void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {

        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Transactions Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";

    }

     void _TransactionsLayout() {
         
        _ClearScreen();
        PrintHeader();

        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t\t  Transactions Menue\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t[1] Deposit.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[2] Withdraw.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[3] Total Balances.\n";
        std:: cout << std::setw(37) << std::left << "" << "\t[4] Main Menue.\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
    }
     static void _ExitMenu(bool& isInMainMenu, const char* message = "\nLogging Out...")
     {
         std::cout << message << "\n\n";

         isInMainMenu = false;

     }

	TransactionsMenuComponents _NavigateUser(double from = 1, double to = 4)
	{
		_Message("Choose What do you want to do ? [1 to 4] : ");

		return  (TransactionsMenuComponents)Validator::returnValidatedNumber(from, to);
	}
    void PerformMenu(const char* Message = nullptr) override {

        bool isInMainMenu = true;
        do
        {
            _TransactionsLayout();

            switch (_NavigateUser())
            {
            case TransactionsMenuComponents::Deposit:
            {
                DepositScreen Deposit(m_RepositoryReference);
                Deposit.Start();
                break;

            }
            case TransactionsMenuComponents::Withdraw:
            {
                WithdrawScreen Withdraw(m_RepositoryReference);
                Withdraw.Start();
                break;
            }
            case TransactionsMenuComponents::TotalBalances: 
            {
                TotalBalanceScreen TotalBalances(m_RepositoryReference);
                TotalBalances.Start();
                break;
            }
            case TransactionsMenuComponents::BackToMain: 
            {
                _GetBackToMenu();
                _ExitMenu(isInMainMenu,"\nGetting Back to Main Menu....");
                break;

            }
            default: //for later enuchoice
            {
                break;

            }
            }

        } while (isInMainMenu);

    }

public :

    TransactionsScreen(Repository & Repo) : MainMenuScreen(m_RepositoryReference), m_RepositoryReference(Repo) {};

    void Start() override {
        PerformMenu();
    }

};

