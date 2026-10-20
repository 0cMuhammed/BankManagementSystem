#pragma once
#include <iostream>
#include "CurrencyRepository.h"
#include "Validator.h"
#include "Screen.h"

class FindCurrencyScreen : public Screen
{
private:

	CurrencyRepository& m_RepositoryReference;

	void PrintHeader(const User& CurrentUser, const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Find Currency Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";
		ShowUserAndDate(CurrentUser);
	}

	void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

		bool isContinue = true;

		do
		{
			_PerformFind(CurrentUser);

			isContinue = Validator::GetConfirmation('\n' + std::string(((Message != nullptr) ? Message : "Do you want to continue this operation?")));

		} while (isContinue);
	}

    CurrencyRepository::OperationStates _FindByCode(const std::string Query, Currency& Result) {
		return m_RepositoryReference.FindByCode(Query, Result);
	}

    CurrencyRepository::OperationStates _FindByCountry(const std::string Query, Currency& Result) {
		return m_RepositoryReference.FindByCountry(Query, Result);
	}

    void _PerformFind(const User & CurrentUser, const char* NotFoundMessage = nullptr) {

			_ClearScreen();
			PrintHeader(CurrentUser);
			Currency currency("", 0, "", 0, Currency::ObjectMode::EmptyMode);

			_Message("Find By: [1] Code or [2] Country ? ");
			short Answer = static_cast<short>(Validator::returnValidatedNumber(1, 2));
			
			const bool ByCode = Answer == 1;

			_Message(ByCode ? "Please enter the currency code : " : "Please enter the country name : ");
			std::string Query = Validator::ReadString();

		 

			CurrencyRepository::OperationStates State = (Answer == 1) ? _FindByCode(Query,currency) : _FindByCountry(Query,currency);

			switch (State)
			{
			case CurrencyRepository::OperationStates::Successful:
			{
				_Message("Currency Found :-)\n");
				_PrintCurrency(currency);
				Logger::LogCurrency(CurrentUser.GetUsername(), Logger::Category::FindCurrency, Logger::Level::INFO, Query, 0, 0, ByCode ? "Code" : "Country");
				break;
			}
			case CurrencyRepository::OperationStates::NotFound:
			{
				_Message(std::string((NotFoundMessage != nullptr) ? NotFoundMessage : "Currency was not found :-(\n"));
				Logger::LogCurrency(CurrentUser.GetUsername(), Logger::Category::FindCurrency, Logger::Level::WARN, Query, 0, 0, ByCode ? "Code" : "Country");
				break;
			}
			default:
			{
				_Message("Something went wrong, please try again.\n");
				break;
			}
			}
		}


	

public:

	FindCurrencyScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessCurrencyServices().AccessRepository()) {};

	void Start(const User& CurrentUser) override {
		PerformMenu(CurrentUser);
		_GetBackToMenu("Press Enter to go back to Currency Exchange Menu");
	}

};

