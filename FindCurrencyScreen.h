#pragma once
#include <iostream>
#include "CurrencyRepository.h"
#include "Validator.h"
#include "Screen.h"

class FindCurrencyScreen : public Screen
{
private:

	CurrencyRepository& m_RepositoryReference;

	void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

		bool isContinue = true;

		while (isContinue)
		{
			if (!_PerformFind(CurrentUser)) return;     // NEW: Esc on the "by code / by country" menu leaves the screen

			term::Blank();
			isContinue = term::Confirm((Message != nullptr) ? Message : "Find another currency?");
		}
	}

    CurrencyRepository::OperationStates _FindByCode(const std::string Query, Currency& Result) {
		return m_RepositoryReference.FindByCode(Query, Result);
	}

    CurrencyRepository::OperationStates _FindByCountry(const std::string Query, Currency& Result) {
		return m_RepositoryReference.FindByCountry(Query, Result);
	}

    bool _PerformFind(const User & CurrentUser, const char* NotFoundMessage = nullptr) {

			_ClearScreen();
			_Header(CurrentUser, "FIND CURRENCY");
			Currency currency("", 0, "", 0, Currency::ObjectMode::EmptyMode);

			// NEW: the old "[1] Code or [2] Country ?" question + number reading is now a 2-row menu.
			static const std::vector<term::MenuItem> options = {
				{ "BY CODE",    "for example USD" },
				{ "BY COUNTRY", "for example Malaysia" }
			};
			const int choice = term::Menu(options, 0);
			if (choice < 0) return false;                            // Esc

			const bool ByCode = (choice == 0);

			term::Blank();
			term::Prompt(ByCode ? "CURRENCY CODE" : "COUNTRY NAME");
			std::string Query = Validator::ReadString();

			CurrencyRepository::OperationStates State = ByCode ? _FindByCode(Query,currency) : _FindByCountry(Query,currency);

			switch (State)
			{
			case CurrencyRepository::OperationStates::Successful:
			{
				_PrintCurrency(currency);
				term::Success("Currency found.");
				Logger::LogCurrency(CurrentUser.GetUsername(), Logger::Category::FindCurrency, Logger::Level::INFO, Query, 0, 0, ByCode ? "Code" : "Country");
				break;
			}
			case CurrencyRepository::OperationStates::NotFound:
			{
				term::Error((NotFoundMessage != nullptr) ? NotFoundMessage : "Currency was not found.");
				Logger::LogCurrency(CurrentUser.GetUsername(), Logger::Category::FindCurrency, Logger::Level::WARN, Query, 0, 0, ByCode ? "Code" : "Country");
				break;
			}
			default:
			{
				term::Error("Something went wrong, please try again.");
				break;
			}
			}
			return true;
		}

public:

	FindCurrencyScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessCurrencyServices().AccessRepository()) {};

	void Start(const User& CurrentUser) override {
		PerformMenu(CurrentUser);
	}

};
