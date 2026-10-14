#pragma once
#include "Currency.h"
#include "FileHandler.h"
void Currency::_SaveExistingObject() {
	std::vector<Currency> currencies = FileHandler::LoadCurrencies();

		for (Currency& c : currencies)
		{
			if (c.GetCode() == this->GetCode())
		{
			c = *this;
			break;

		}

	}

	FileHandler::SaveCurrencies(currencies);
}
void Currency::_SaveNewObject() {
	this->m_mode = ObjectMode::ExistingMode;
	FileHandler::SaveCurrencies(*this);
}