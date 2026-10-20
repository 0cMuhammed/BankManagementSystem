#pragma once
#include <iostream>
#include "CurrencyRepository.h"
#include "Validator.h"
#include "Logger.h"
#include "Screen.h"

class UpdateCurrencyRateScreen : public Screen
{
private:

	CurrencyRepository& m_RepositoryReference;

	void PrintHeader(const User& CurrentUser, const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Update Currency Rate Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";
		ShowUserAndDate(CurrentUser);
	}

	void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

		bool IsContinueOperation = true;

		do
		{
			_PerformUpdate(CurrentUser);

			IsContinueOperation = Validator::GetConfirmation('\n' + std::string(((Message != nullptr) ? Message : "Do you want to continue this operation?")));

		} while (IsContinueOperation);
	}

	// exclusive
	static bool _PerformConfirmation(Currency& currency, const char* Message = nullptr) {
		_PrintCurrency(currency);
		return Validator::GetConfirmation('\n' + std::string(((Message != nullptr) ? Message : "Are you sure you want to update the rate of this currency?")));
	}

	 double _ReadRate() {
		_Message("Enter the new rate : ");
		return Validator::returnNumber("Invalid rate, Please enter a number greater than 0 : ");
	}

	void _PrintUpdateStatus(const User& CurrentUser, Currency& currency) {

		const std::string Code = currency.GetCode();
		const double OldRate = currency.GetRate();
		const double NewRate = _ReadRate();

		switch (m_RepositoryReference.UpdateRate(Code, NewRate))
		{
		case CurrencyRepository::OperationStates::Failed: // for some reason....
		{
			std::cout << "\nOperation Failed, Try again Later...\n";
			break;
		}
		case CurrencyRepository::OperationStates::Successful:
		{
			Logger::LogCurrency(CurrentUser.GetUsername(), Logger::Category::UpdateCurrencyRate, Logger::Level::INFO, Code, OldRate, NewRate);

			std::cout << "\nCurrency rate is updated successfully!\n";

			m_RepositoryReference.FindByCode(Code, currency); // show new rate
			_PrintCurrency(currency);
			break;
		}
		default:
		{
			break;
		}
		}
	}

	void _Update(const User& CurrentUser, const std::string& CurrencyCode) {

		Currency currency("", 0, "", 0, Currency::ObjectMode::EmptyMode); // filled by the repository on success

		if (m_RepositoryReference.FindByCode(CurrencyCode, currency) != CurrencyRepository::OperationStates::Successful)
		{
			std::cout << "Currency is not found.\n";
		}
		else
		{
			(_PerformConfirmation(currency)) ? _PrintUpdateStatus(CurrentUser, currency) : _Message("Operation is cancelled.");
		}
	}

	void _PerformUpdate(const User& CurrentUser) {

		_ClearScreen();
		PrintHeader(CurrentUser);

		_Message("Please enter the currency code : ");
		std::string CurrencyCode = Validator::ReadString();

		_Update(CurrentUser, CurrencyCode);
	}

public:

	UpdateCurrencyRateScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessCurrencyServices().AccessRepository()) {};

	void Start(const User& CurrentUser) override {
		PerformMenu(CurrentUser);
		_GetBackToMenu("Press Enter to go back to Currency Exchange Menu");
	}
};

