#pragma once
#include <iostream>
#include <vector>
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

    int m_LastIndex = 0;   // NEW: remembers the highlighted row

    void _PrintLayout() {

        _ClearScreen();
        term::Header("CURRENCY EXCHANGE", "", m_Session.GetUser().GetUsername());   // NEW: replaces PrintHeader override + hand-printed menu box
        term::Section("MENU", std::string(term::g::Up) + term::g::Down + " move  " + term::g::Mid + "  Enter open  " + term::g::Mid + "  Esc back");
    }

    MenuComponents _NavigateUser()
    {
        // NEW: row order = enum order
        static const std::vector<term::MenuItem> items = {
            { "LIST CURRENCIES",    "every currency and its rate" },
            { "FIND CURRENCY",      "by code or by country" },
            { "UPDATE RATE",        "change a currency's rate" },
            { "CALCULATOR",         "convert between two currencies" },
            { "BACK",               "return to the main menu" }
        };

        int choice = term::Menu(items, m_LastIndex);

        if (choice < 0) return MenuComponents::Exit;           // Esc = go back

        m_LastIndex = choice;
        return static_cast<MenuComponents>(choice + 1);
    }

    void _Menu(bool& isInMainMenu) {

        do
        {
            _PrintLayout();

            const MenuComponents choice = _NavigateUser();

            if (choice != MenuComponents::Exit) _ClearScreen();

            switch (choice)
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
                isInMainMenu = false;
                break;
            }
            default: //for later enum choices
            {
                break;
            }
            }

        } while (isInMainMenu);
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
    }
};
