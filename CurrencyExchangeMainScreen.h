#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include "Validator.h"
#include "Session.h"
#include "ListCurrenciesScreen.h"
#include "FindCurrencyScreen.h"
#include "UpdateRateScreen.h"
#include "CurrencyCalculatorScreen.h"
#include "Screen.h"

class CurrencyExchangeMainScreen : public Screen
{
public:

    enum MenuComponents { List = 1, Find = 2, Update = 3, Calculator = 4, Exit = 5 };

private:

    Session& m_Session;
    Service& m_ServicesRef;


    void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {

        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Currency Exchange Main Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";
        ShowUserAndDate(m_Session.GetUser());

    }

    void _PrintLayout() {

        _ClearScreen();
        PrintHeader();

        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t\t Currency Exchange Menue\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t[1] List Currencies.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[2] Find Currency.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[3] Update Rate.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[4] Currency Calculator.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[5] Main Menu.\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
    }

    void _Menu(bool& isInMainMenu) {

        do
        {
            _PrintLayout();

            switch (_NavigateUser())
            {
            case MenuComponents::List:
            {
                ListCurrenciesScreen List(m_ServicesRef);
                List.Start(m_Session.GetUser());
                break;
            }
            case MenuComponents::Find:
            {
                FindCurrencyScreen Find(m_ServicesRef);
                Find.Start(m_Session.GetUser());
                break;
            }
            case MenuComponents::Update:
            {
                UpdateCurrencyRateScreen Update(m_ServicesRef);
                Update.Start(m_Session.GetUser());
                break;
            }
            case MenuComponents::Calculator:
            {
                CurrencyCalculatorScreen Calculator(m_ServicesRef);
                Calculator.Start(m_Session.GetUser());
                break;
            }
            case MenuComponents::Exit:
            {
                _ExitMenu(isInMainMenu);
                break;
            }
            default: //for later enum choices
            {
                break;
            }
            }

        } while (isInMainMenu);
    }

    static void _ExitMenu(bool& isInMainMenu, const char* message = "\nGetting Back to Main Menu...")
    {
        std::cout << message << "\n\n";

        isInMainMenu = false;
    }

    MenuComponents _NavigateUser(double from = 1, double to = 5)
    {
        _Message("Choose What do you want to do ? [1 to 5] : ");

        return  (MenuComponents)Validator::returnValidatedNumber(from, to);
    }

    void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

        bool isInMainMenu = true;

        _Menu(isInMainMenu); 
    }

public:

    CurrencyExchangeMainScreen(Service& Ref, Session& CurrentUser) : Screen(Ref), m_Session(CurrentUser), m_ServicesRef(Ref) {};

    void Start(const User& CurrentUser) override
    {
        PerformMenu(CurrentUser);
        _GetBackToMenu();
    }
};

