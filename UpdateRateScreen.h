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

	void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

		bool IsContinueOperation = true;

		while (IsContinueOperation)
		{
			_PerformUpdate(CurrentUser);

			term::Blank();
			IsContinueOperation = term::Confirm((Message != nullptr) ? Message : "Update another rate?");
		}
	}

	// exclusive
	static bool _PerformConfirmation(Currency& currency, const char* Message = nullptr) {
		_PrintCurrency(currency);
		return term::Confirm((Message != nullptr) ? Message : "Do you want to update the rate of this currency?");
	}

	 double _ReadRate() {
		term::Prompt("NEW RATE");
		return Validator::returnNumber("Invalid rate. Please enter a number greater than 0.");
	}

	void _PrintUpdateStatus(const User& CurrentUser, Currency& currency) {

		const std::string Code = currency.GetCode();
		const double OldRate = currency.GetRate();
		const double NewRate = _ReadRate();

		switch (m_RepositoryReference.UpdateRate(Code, NewRate))
		{
		case CurrencyRepository::OperationStates::Failed: // for some reason....
		{
			term::Error("Operation failed, try again later.");
			break;
		}
		case CurrencyRepository::OperationStates::Successful:
		{
			Logger::LogCurrency(CurrentUser.GetUsername(), Logger::Category::UpdateCurrencyRate, Logger::Level::INFO, Code, OldRate, NewRate);

			term::Blank();
			term::Success("Currency rate updated successfully.");

			m_RepositoryReference.FindByCode(Code, currency); // show new rate
			_PrintCurrency(currency);
			term::Print(std::string(term::col::Muted) + "  " + term::Pad("CHANGE", 12) + term::col::Muted + term::Fixed(OldRate, 4)   // NEW: old --> new, like the money receipts
			            + term::col::Cyan + " " + term::g::Rule + term::g::Play + " " + term::col::Green + term::Fixed(NewRate, 4));
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
			term::Error("Currency was not found.");
		}
		else
		{
			(_PerformConfirmation(currency)) ? _PrintUpdateStatus(CurrentUser, currency) : term::Warn("Operation cancelled.");
		}
	}

	void _PerformUpdate(const User& CurrentUser) {

		_ClearScreen();
		_Header(CurrentUser, "UPDATE RATE");

		term::Prompt("CURRENCY CODE");
		std::string CurrencyCode = Validator::ReadString();

		_Update(CurrentUser, CurrencyCode);
	}

public:

	UpdateCurrencyRateScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessCurrencyServices().AccessRepository()) {};

	void Start(const User& CurrentUser) override {
		PerformMenu(CurrentUser);
	}
};
