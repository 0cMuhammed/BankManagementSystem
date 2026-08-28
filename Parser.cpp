#include "Parser.h"
#include "Client.h"
#include "User.h"

std::string Parser::ObjectToLine(const Client& client, const std::string& delimiter) {
	return client.GetFirstName() + delimiter + client.GetLastName() + delimiter + client.GetEmail() + delimiter + client.GetPhoneNumber() + delimiter + client.getAccountNumber() + delimiter + client.getPinCode() + delimiter + std::to_string(client.getBalance()) + '\n';
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


	return User(std::move(Tokens[0]), std::move(Tokens[1]), std::move(Tokens[2]), std::move(Tokens[3]), std::move(Tokens[4]), std::move(Tokens[5]), static_cast<uint8_t>(stoi(Tokens[6])), User::ObjectMode::ExistingMode);

}
