#pragma once
#include "Global.h"
#include "Parser.h"
#include "Client.h"
#include "User.h"
#include "Currency.h"



std::string Parser::ObjectToLine(const Client& client, const std::string& delimiter) {

	return client.GetFirstName() + delimiter + client.GetLastName() + delimiter + client.GetEmail() + delimiter + client.GetPhoneNumber() + delimiter + client.getAccountNumber() + delimiter + client.getPinCode() + delimiter + std::to_string(client.getBalance());
}
std::string Parser::ObjectToLine(const User& user, const std::string& delimiter) {
	return user.GetFirstName() + delimiter + user.GetLastName() + delimiter + user.GetEmail() + delimiter + user.GetPhoneNumber() + delimiter + user.GetUsername() + delimiter + user.GetPassword() + delimiter + std::to_string(user.GetPermissions());
}
Client Parser::LineToClient(std::string line) {

	std::vector<std::string> Tokens;
	Tokens.reserve(7);

	Tokens = Parser::TokensToVec(std::move(line));

	if (Tokens.size() != 7)
		throw std::runtime_error("Malformed line: expected 7 fields, got " + std::to_string(Tokens.size()));


	return Client(std::move(Tokens[0]), std::move(Tokens[1]), std::move(Tokens[2]), std::move(Tokens[3]), std::move(Tokens[4]), std::move(Tokens[5]), stod(Tokens[6]), Client::ObjectMode::ExistingMode);

}
User Parser::LineToUser(std::string line) {

	std::vector<std::string> Tokens;
	Tokens.reserve(7);
	Tokens = Parser::TokensToVec(std::move(line));

	if (Tokens.size() != 7)
		throw std::runtime_error("Malformed line: expected 7 fields, got " + std::to_string(Tokens.size()));


	return User(std::move(Tokens[0]), std::move(Tokens[1]), std::move(Tokens[2]), std::move(Tokens[3]), std::move(Tokens[4]), std::move(Tokens[5]), static_cast<int32_t>(stoi(Tokens[6])), User::ObjectMode::ExistingMode);

}
Currency Parser::LinetoCurrency(std::string line) {

	std::vector<std::string> Tokens;
	Tokens.reserve(4);

	Tokens = Parser::TokensToVec(std::move(line));

	if (Tokens.size() != 4)
		throw std::runtime_error("Malformed line: expected 4 fields, got " + std::to_string(Tokens.size()));


	return Currency(std::move(Tokens[0]), _StringToCode(Tokens[1]), std::move(Tokens[2]), std::stod(Tokens[3]), Currency::ObjectMode::ExistingMode);
}
std::string Parser::CurrencyToLine( Currency& c, const std::string& delimiter = "#//#") {
	return c.GetCountry() + delimiter + c.GetCode() + delimiter + c.GetName() + delimiter + std::to_string(c.GetRate());
}



std::string Parser::LogtoLine(std::string line) {

	std::vector<std::string> Tokens;
	Tokens.reserve(5);

	Tokens = Parser::TokensToVec(std::move(line), LOG_DELIMITER);

	if (Tokens.size() < 3)
		throw std::runtime_error("Malformed line: expected at least 3 fields, got " + std::to_string(Tokens.size()));

	if (Tokens[0].find(" / ") == std::string::npos)
		throw std::runtime_error("Malformed line: the level and date field is not valid");

	std::string str = Tokens[0];

	for (size_t i = 1; i < Tokens.size(); i++) 
	{
		str += DELIMITER;
		str += Tokens[i];
	}

	return str;
}

