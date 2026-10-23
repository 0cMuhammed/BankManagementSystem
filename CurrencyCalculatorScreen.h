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

	void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

		bool IsContinueOperation = true;

		while (IsContinueOperation)
		{
			_PerformCalculation(CurrentUser);

			term::Blank();
			IsContinueOperation = term::Confirm((Message != nullptr) ? Message : "Make another calculation?");
		}
	}

	double _ReadAmount() {
		term::Prompt("AMOUNT");
		return Validator::returnNumber("Invalid amount. Please enter a number greater than 0.");
	}

	bool _ReadCurrency(const char* Label, Currency& Result) {
		term::Prompt(Label);
		std::string Code = Validator::ReadString();

		if (m_RepositoryReference.FindByCode(Code, Result) != CurrencyRepository::OperationStates::Successful)
		{
			term::Error("Currency was not found.");
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
			term::Error("Operation failed, try again later.");
			return;
		}

		Logger::LogCurrency(CurrentUser.GetUsername(), Logger::Category::CurrencyCalculator, Logger::Level::INFO, "", 0, 0,
			std::to_string(Amount) + " " + FromCode + " -> " + ToCode);

		term::Blank();
		term::Field("FROM", FromCode + "  " + term::Fit(From.GetName(), 36), term::col::Base);   // NEW: two short lines instead of two full currency cards
		term::Field("TO",   ToCode   + "  " + term::Fit(To.GetName(), 36),   term::col::Base);
		term::Blank();

		// NEW: the answer in one big line:  100.00 USD --> 447.35 MYR
		term::Print(std::string("  ") + term::col::White + term::col::Bold + term::Fixed(Amount, 2) + " " + FromCode
		            + term::col::Cyan + "  " + term::g::Rule + term::g::Play + "  "
		            + term::col::Green + term::col::Bold + term::Fixed(Price, 2) + " " + ToCode);
	}

	void _PerformCalculation(const User& CurrentUser) {

		_ClearScreen();
		_Header(CurrentUser, "CURRENCY CALCULATOR");

		Currency From("", 0, "", 0, Currency::ObjectMode::EmptyMode); // filled by the repository on success
		Currency To("", 0, "", 0, Currency::ObjectMode::EmptyMode);

		if (!_ReadCurrency("FROM CODE", From)) { return; }
		if (!_ReadCurrency("TO CODE", To)) { return; }

		double Amount = _ReadAmount();

		_PrintResults(CurrentUser, Amount, From, To);
	}

public:

	CurrencyCalculatorScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessCurrencyServices().AccessRepository()) {};

	void Start(const User& CurrentUser) override {

		PerformMenu(CurrentUser, "Make another calculation?");

	}
};
