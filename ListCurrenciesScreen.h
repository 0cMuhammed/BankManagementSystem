#pragma once
#include <iostream>
#include<vector>
#include <limits>
#include "Session.h"
#include "Screen.h"

class ListCurrenciesScreen : public Screen
{
private:

     CurrencyRepository& m_RepositoryReference; //hmm since the getters depends on a private function i thinks its what causing this, it should be const

    static constexpr int W = 94;          // NEW: table width (2 + 28+2+6+2+36+2+12 = 90, plus margin)
    static constexpr size_t PAGE = 18;    // NEW: rows per page. The list has 200+ currencies, far more than fits in a window, so it is shown page by page.

    // NEW: column titles + thin line (widths must match _PrintRow)
    static void _PrintLayout() {
        term::Print(std::string(term::col::Muted) + "  " + term::Pad("COUNTRY", 28) + "  " + term::Pad("CODE", 6) + "  " + term::Pad("NAME", 36)
                    + "  " + term::PadLeft("RATE / 1$", 12), W);
        term::Rule(W);
    }

    static void _PrintRow(Currency& c) {
        term::Print(std::string("  ") + term::col::Base + term::Pad(term::Fit(c.GetCountry(), 28), 28)
                    + term::col::Cyan + "  " + term::Pad(c.GetCode(), 6)
                    + term::col::Base + "  " + term::Pad(term::Fit(c.GetName(), 36), 36)
                    + term::col::Green + "  " + term::PadLeft(term::Fixed(c.GetRate(), 4), 12), W);
    }

    void _ShowList(const User& CurrentUser) {

        std::vector<Currency>& list = m_RepositoryReference.GetMutableList();

        _ClearScreen();

        if (list.empty())
        {
            _Header(CurrentUser, "CURRENCIES", "0 RECORDS", W);
            term::Warn("No currencies available in the system.", W);
            _GetBackToMenu("Press any key to go back to the Currency Exchange menu");
            return;
        }

        Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::ListCurrencies, Logger::Level::INFO);   // (was inside _PrintAll)

        // NEW: term::Paginate does the page loop and the keys. We only say how to print the header, the column titles, and row number i.
        term::Paginate(list.size(), PAGE, W,
            [&](const std::string& info) { _Header(CurrentUser, "CURRENCIES", info, W); },
            []() { _PrintLayout(); },
            [&](size_t i) { _PrintRow(list[i]); });
    }

    void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {
            _ShowList(CurrentUser);
    }

public:

    ListCurrenciesScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessCurrencyServices().AccessRepository()) {};

    void Start(const User& CurrentUser) override {

        PerformMenu(CurrentUser);   // NEW: no extra "press any key" afterwards, the pager already ends with Esc / Enter

    }
};
