#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include <cstdint>


class Client;
class User;
class Currency;


class Parser  
{

public :

	static std::vector<std::string> TokensToVec(std::string line, const std::string& delimiter = "#//#")
	{
		std::string Word = "";
		size_t pos = 0;
		std::vector<std::string> vTokens;
		vTokens.reserve(7);

		while ((pos = line.find(delimiter)) != std::string::npos)
		{
			Word = line.substr(0, pos);

			if (Word != "")
				vTokens.push_back(std::move(Word));


			line.erase(0, pos + delimiter.length());
		}

		if (line != "")
			vTokens.push_back(std::move(line));


		return vTokens;

	}

	static uint32_t _StringToCode(std::string code) {

		std::string CurrencyCodes[] = {
			"USD", "AFN", "ALL", "DZD", "EUR", "AOA", "XCD", "ARS", "AMD",
			"AWG", "AUD", "AZN", "BSD", "BHD", "BDT", "BBD", "BYN", "BZD", "XOF",
			"BMD", "BTN", "BOB", "BAM", "BWP", "BRL", "BND", "BGN", "BIF", "KHR",
			"XAF", "CAD", "CVE", "KYD", "CLP", "CNY", "COP", "KMF", "CDF", "NZD",
			"CRC", "HRK", "CUP", "CZK", "DKK", "DJF", "DOP", "EGP", "ERN", "SZL",
			"ETB", "FJD", "XPF", "GMD", "GEL", "GHS", "GIP", "GTQ", "GNF", "GYD",
			"HTG", "HNL", "HKD", "HUF", "ISK", "INR", "IDR", "IRR", "IQD", "ILS",
			"JMD", "JPY", "JOD", "KZT", "KES", "KPW", "KRW", "KWD", "KGS", "LAK",
			"LBP", "LSL", "LRD", "LYD", "CHF", "MOP", "MGA", "MWK", "MYR", "MVR",
			"MRU", "MUR", "MXN", "MDL", "MNT", "MAD", "MZN", "MMK", "NAD", "NPR",
			"ANG", "NIO", "NGN", "MKD", "NOK", "OMR", "PKR", "PAB", "PGK", "PYG",
			"PEN", "PHP", "PLN", "QAR", "RON", "RUB", "RWF", "SHP", "WST", "STN",
			"SAR", "RSD", "SCR", "SLE", "SGD", "SBD", "SOS", "ZAR", "SSP", "LKR",
			"SDG", "SRD", "SEK", "SYP", "TJS", "TZS", "THB", "TOP", "TTD", "TND",
			"TRY", "TMT", "UGX", "UAH", "AED", "GBP", "UYU", "UZS", "VUV", "VES",
			"VND", "YER", "ZMW", "ZWL"
		};

		for (size_t i = 0; i < std::size(CurrencyCodes); i++) {
			if (CurrencyCodes[i] == code) {
				return static_cast<uint32_t>(i + 1);
			}
		}

		return static_cast<uint32_t>(0);
	}

	static std::string ObjectToLine(const Client& client, const std::string& delimiter = "#//#");
	static std::string ObjectToLine(const User& user, const std::string& delimiter = "#//#");


	static  Client LineToClient(std::string line);
	static  User LineToUser(std::string line);	
	static std::string LogtoLine(std::string line);
	static Currency LinetoCurrency(std::string line);
	static std::string CurrencyToLine(Currency &c, const std::string& delimiter = "#//#");

	static std::string BalanceToText(int Number)
	{

		if (Number == 0)
		{
			return "";
		}

		if (Number >= 1 && Number <= 19)
		{
			std::string arr[] = { "", "One","Two","Three","Four","Five","Six","Seven",
		"Eight","Nine","Ten","Eleven","Twelve","Thirteen","Fourteen",
		  "Fifteen","Sixteen","Seventeen","Eighteen","Nineteen" };

			return  arr[Number] + " ";

		}

		if (Number >= 20 && Number <= 99)
		{
			std::string arr[] = { "","","Twenty","Thirty","Forty","Fifty","Sixty","Seventy","Eighty","Ninety" };
			return  arr[Number / 10] + " " + BalanceToText(Number % 10);
		}

		if (Number >= 100 && Number <= 199)
		{
			return  "One Hundred " + BalanceToText(Number % 100);
		}

		if (Number >= 200 && Number <= 999)
		{
			return   BalanceToText(Number / 100) + "Hundreds " + BalanceToText(Number % 100);
		}

		if (Number >= 1000 && Number <= 1999)
		{
			return  "One Thousand " + BalanceToText(Number % 1000);
		}

		if (Number >= 2000 && Number <= 999999)
		{
			return   BalanceToText(Number / 1000) + "Thousands " + BalanceToText(Number % 1000);
		}

		if (Number >= 1000000 && Number <= 1999999)
		{
			return  "One Million " + BalanceToText(Number % 1000000);
		}

		if (Number >= 2000000 && Number <= 999999999)
		{
			return   BalanceToText(Number / 1000000) + "Millions " + BalanceToText(Number % 1000000);
		}

		if (Number >= 1000000000 && Number <= 1999999999)
		{
			return  "One Billion " + BalanceToText(Number % 1000000000);
		}
		else
		{
			return   BalanceToText(Number / 1000000000) + "Billions " + BalanceToText(Number % 1000000000);
		}

		// in UI
	}

};



