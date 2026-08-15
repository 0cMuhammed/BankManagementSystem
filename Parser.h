#pragma once
#include <iostream>
#include<vector>
#include<fstream>

class Client;

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

	static std::string ObjectToLine(const Client& client, const std::string& delimiter = "#//#");
	static  Client LineToObject(const std::string& line);

	// to UI
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

#include "Client.h"
inline std::string Parser::ObjectToLine(const Client& client, const std::string& delimiter) {
	return client.getFirstName() + delimiter + client.getLastName() + delimiter + client.getEmail() + delimiter + client.getPhoneNumber() + delimiter + client.getAccountNumber() + delimiter + client.getPinCode() + delimiter + std::to_string(client.getBalance());
}
inline  Client Parser::LineToObject(const std::string& line) {

	std::vector<std::string> Tokens;
	Tokens.reserve(7);
	Tokens = Parser::TokensToVec(line);

	if (Tokens.size() != 7)
		throw std::runtime_error("Malformed line: expected 7 fields, got " + std::to_string(Tokens.size()));


	return Client(Tokens[0], Tokens[1], Tokens[2], Tokens[3], Tokens[4], Tokens[5], stod(Tokens[6]), Client::ObjectMode::UpdateMode);

}


