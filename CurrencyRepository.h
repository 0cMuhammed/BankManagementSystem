#pragma once
#include <iostream>
#include <vector>
#include "FileHandler.h"
#include "Currency.h"

class CurrencyRepository
{
public :
	enum  OperationStates { Failed = 1, Successful = 2, NotFound = 4};
	
private :

	std::vector<Currency> m_List;

	void _UpdateVector() {
		m_List.clear();
		m_List = FileHandler::LoadCurrencies();
	}

	void _AddInVector(const Currency& currency) {
		m_List.push_back(currency);
	}
	bool IgnoreCase(const std::string& str1, const std::string& str2)
	{
		if (str1.length() != str2.length())
			return false;

		for (int i = 0; i < str1.length(); i++) 
		{
			if (tolower(str1[i]) != tolower(str2[i]))
				return false;
		}

		return true;
	}



	static Currency _GetEmptyObject() noexcept {
		return Currency("", 0, "", 0, Currency::ObjectMode::EmptyMode);
	}
	

	Currency _FindObjectByCode(const std::string& code) {

		for (Currency &currency : m_List)
		{
			if (IgnoreCase(currency.GetCode(), code))
			{
				return currency;

			}
		}


		return _GetEmptyObject();
	}

	Currency _FindObjectByCountry(const std::string& Country) {

		for (Currency& currency : m_List)
		{
			if (IgnoreCase(currency.GetCountry(), Country))
			{
				return currency;

			}
		}


		return _GetEmptyObject();
	}

	bool _UpdateRate(const std::string&Code, double NewRate) {

		for (Currency& currency : m_List)
		{
			if (IgnoreCase(currency.GetCode(), Code))
			{
				currency.SetRate(NewRate);                // modifes in vector
				FileHandler::SaveCurrencies(m_List);     // save in file 
				return true;

			}
		}

		return false;
	}

public :

	CurrencyRepository(std::vector<Currency> list) : m_List(std::move(list)) {};

	const std::vector<Currency>& GetList() const {
		return m_List; // read only 
	}
	std::vector<Currency>& GetMutableList()  noexcept {
		return m_List;
	}

	OperationStates FindByCode(const std::string& Code) {
		Currency c = _FindObjectByCode(Code);

		return (c.isEmpty()) ? OperationStates::NotFound : OperationStates::Successful;
	}
	OperationStates FindByCountry(const std::string& Country) {
		Currency c = _FindObjectByCountry(Country);

		return (c.isEmpty()) ? OperationStates::NotFound : OperationStates::Successful;
	}

	OperationStates CalculateExchangeRate(const std::string& FromCode, const std::string& ToCode, double &Price) {

		Currency From = _FindObjectByCode(FromCode);
		Currency To = _FindObjectByCode(ToCode);
		
		

		if (From.isEmpty() || To.isEmpty() || From.GetRate() == 0)
		{
			return OperationStates::Failed;
		}
			
	
			

			double p = Price / From.GetRate();
			Price = p * To.GetRate();

			return OperationStates::Successful;
	}
			
	OperationStates UpdateRate(const std::string& Code, double newRate) {

		Currency toUpdate = _FindObjectByCode(Code);

		if (!toUpdate.isEmpty()) 
		{
			return (_UpdateRate(Code, newRate)) ? OperationStates::Successful : OperationStates::Failed;
		}

		return OperationStates::Failed;
	}





};

