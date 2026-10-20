#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include <limits>
#include "Session.h"
#include "Screen.h"

class ListCurrenciesScreen : public Screen
{
private:

     CurrencyRepository& m_RepositoryReference; //hmm since the getters depends on a private function i thinks its what causing this, it should be const

    void _ShowList(const User& CurrentUser) {



        _ClearScreen();
        if (m_RepositoryReference.GetList().size() == 0)
        {

            _Message("\t\t\t\tNo currencies available in the system!.\n");

        }

        else
        {
            const std::string SubTitle = "\t    (" + std::to_string(m_RepositoryReference.GetList().size()) + ") Currencies.";

             PrintHeader(CurrentUser, nullptr, SubTitle.c_str());
            _PrintLayout();
            _PrintAll(CurrentUser.GetUsername(), m_RepositoryReference.GetMutableList());

        }
    }

    void PrintHeader(const User& CurrentUser, const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {

        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t\t  " << (((ScreenName != nullptr) ? ScreenName : "Currencies List Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";

        ShowUserAndDate(CurrentUser);
    }

    void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {
            _ShowList(CurrentUser);
    }

    //exclusive
    static void _PrintFormattedCurrency(Currency& c)
    {

        std::cout << std::setw(8) << std::left << "" << "| " << std::setw(30) << std::left << c.GetCountry();
        std::cout << "| " << std::setw(8) << std::left << c.GetCode();
        std::cout << "| " << std::setw(45) << std::left << c.GetName();
        std::cout << "| " << std::setw(10) << std::left << c.GetRate();

    }
    static void _PrintLine() {
        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_________________________________________\n" << std::endl;
    }
    static void _PrintLayout() {

        _PrintLine();
        std::cout << std::setw(8) << std::left << "" << "| " << std::left << std::setw(30) << "Country";
        std::cout << "| " << std::left << std::setw(8) << "Code";
        std::cout << "| " << std::left << std::setw(45) << "Name";
        std::cout << "| " << std::left << std::setw(10) << "Rate/(1$)";
        _PrintLine();
    }
    static void _PrintAll(const std::string& CurrentUser, std::vector<Currency>& Currencies) {

        Logger::LogUser(CurrentUser, Logger::Category::ListCurrencies, Logger::Level::INFO);

        for (Currency& c: Currencies)
        {

            _PrintFormattedCurrency(c);
            std::cout << std::endl;
        }
        _PrintLine();

    }


public:


    ListCurrenciesScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessCurrencyServices().AccessRepository()) {};

    void Start(const User& CurrentUser) override {



        PerformMenu(CurrentUser);
        _GetBackToMenu("Press Enter to go back to Currency Exchange Menu");

    }
};

