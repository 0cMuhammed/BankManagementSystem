#pragma once
#include <iostream>

#include "CurrencyRepository.h"
#include "Validator.h"
#include "Logger.h"

#include "Screen.h"

class CurrencyCalculatorScreen : public Screen
{
private:

	CurrencyRepository& m_RepositoryReference;

	void PrintHeader(const User& CurrentUser, const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Currency Calculator Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";
		ShowUserAndDate(CurrentUser);
	}

	void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

		bool IsContinueOperation = true;

		do
		{
			_PerformCalculation(CurrentUser);

			IsContinueOperation = Validator::GetConfirmation('\n' + std::string(((Message != nullptr) ? Message : "Do you want to continue this operation?")));

		} while (IsContinueOperation);
	}



	double _ReadAmount() {
		_Message("Enter the amount to exchange : ");
		return Validator::returnNumber("Invalid amount, Please enter a number greater than 0 : ");
	}

	bool _ReadCurrency(const char* Message, Currency& Result) {
		_Message(Message);
		std::string Code = Validator::ReadString();

		if (m_RepositoryReference.FindByCode(Code, Result) != CurrencyRepository::OperationStates::Successful)
		{
			std::cout << "Currency is not found.\n";
			return false;
		}

		return true;
	}

	void _PrintResults(const User& CurrentUser, double Amount, Currency& From, Currency& To) {

		const std::string FromCode = From.GetCode();
		const std::string ToCode = To.GetCode();

		double Price = Amount; // CalculateExchangeRate converts it in place

		if (m_RepositoryReference.CalculateExchangeRate(FromCode, ToCode, Price) != CurrencyRepository::OperationStates::Successful)
		{
			std::cout << "\nOperation Failed, Try again Later...\n";
			return;
		}

		Logger::LogCurrency(CurrentUser.GetUsername(), Logger::Category::CurrencyCalculator, Logger::Level::INFO, "", 0, 0,
			std::to_string(Amount) + " " + FromCode + " -> " + ToCode);

		_Message("Convert From :");
		_PrintCurrency(From);

		_Message("Convert To :");
		_PrintCurrency(To);

		std::cout << "\n" << Amount << " " << FromCode << " = " << Price << " " << ToCode << "\n";
	}

	void _PerformCalculation(const User& CurrentUser) {

		_ClearScreen();
		PrintHeader(CurrentUser);

		Currency From("", 0, "", 0, Currency::ObjectMode::EmptyMode); // filled by the repository on success
		Currency To("", 0, "", 0, Currency::ObjectMode::EmptyMode);

		if (!_ReadCurrency("Please enter currency 1 code (convert from) : ", From)) { return; }
		if (!_ReadCurrency("Please enter currency 2 code (convert to) : ", To)) { return; }

		double Amount = _ReadAmount();

		_PrintResults(CurrentUser, Amount, From, To);
	}

public:

	CurrencyCalculatorScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessCurrencyServices().AccessRepository()) {};

	void Start(const User& CurrentUser) override {

		PerformMenu(CurrentUser, "Do you want to perform another calculation?");

		_GetBackToMenu("Press Enter to go back to Currency Exchange Menu");

	}
};

