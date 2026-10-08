#include<fstream>
#include "FileHandler.h"
#include "Client.h"
#include "User.h"
#include "Parser.h"
#include "Global.h"
#include <filesystem>

 bool FileHandler::_isNotToBeSaved(const Client& client) noexcept {
	const Client::ObjectMode Mode = client.getMode();
	return	(Mode == Client::ObjectMode::EmptyMode || Mode == Client::ObjectMode::DeleteMode);
}
 bool FileHandler::_isNotToBeSaved(const User& user) noexcept {
	const User::ObjectMode Mode = user.GetMode();
	return	(Mode == User::ObjectMode::EmptyMode || Mode == User::ObjectMode::DeleteMode);
}

 std::vector<Client>FileHandler::LoadClients() {

	std::fstream file;
	std::vector<Client> Clients;

	file.open(CLIENTS_FILE, std::ios::in);

	if (file.is_open())
	{
		std::string dataline = "";

		while ( (!std::getline(file, dataline).fail()) && (!dataline.empty())  )
		{
			Clients.emplace_back(Parser::LineToClient(std::move(dataline))); // internally std::move(dataline) to to Tokens() if passed rvalue, if a lvalue is passed a copy would happen
		}

		file.close();
	}

	return Clients;
}
std::vector<User>FileHandler::LoadUsers() {
	std::fstream file;
	std::vector<User> Users;

	file.open(USERS_FILE, std::ios::in);

	if (file.is_open())
	{
		std::string dataline = "";

		while (!(std::getline(file, dataline).fail()) && (!dataline.empty()))
		{
			Users.emplace_back(Parser::LineToUser(std::move(dataline))); // internally std::move(dataline) to to Tokens() if passed rvalue, if a lvalue is passed a copy would happen
		}

		file.close();
	}
	else 
	{
		std::cout<< "Failed to open Users.txt.\n";
		std::cout << "Path : " << std::filesystem::current_path() << "\n";
	}

	return Users;
}

std::vector<std::string> FileHandler::LoadLogs() {
	std::fstream file;
	std::vector<std::string> logs;

	file.open(LOGGER_FILE, std::ios::in);

	if (file.is_open())
	{
		std::string dataline = "";

		while (!(std::getline(file, dataline).fail()) && (!dataline.empty()))
		{
			try
			{
				logs.emplace_back(Parser::LogtoLine(std::move(dataline)));
			}
			catch (const std::runtime_error&) 
			{
				continue;
			}
		}

		file.close();
	}
	

	return logs;
}
std::vector<std::string> FileHandler::LoadAuditLogs() {
	std::fstream file;
	std::vector<std::string> logs;

	file.open(LOGGER_TRANSFERS_FILE, std::ios::in);

	if (file.is_open())
	{
		std::string dataline = "";

		while (!(std::getline(file, dataline).fail()) && (!dataline.empty()))
		{
			try
			{
				logs.emplace_back(Parser::LogtoLine(std::move(dataline)));
			}
			catch (const std::runtime_error&)
			{
				continue;
			}
		}

		file.close();
	}


	return logs;
}
void FileHandler::SaveLog(const std::string& message) {

	std::fstream File;

	File.open(LOGGER_FILE, std::ios::out | std::ios::app);

	if (File.is_open())
	{

		File << message << '\n';
	}
	File.close();

}

void FileHandler::SaveLogTransfer(const std::string& message) {
	std::fstream File;

	File.open(LOGGER_TRANSFERS_FILE, std::ios::out | std::ios::app);

	if (File.is_open())
	{
		File << message << '\n';
	}
	File.close();
}
  void FileHandler::SaveClients(const Client& client) {

	std::fstream File;
	File.open(CLIENTS_FILE, std::ios::out | std::ios::app);

	if (File.is_open() && !_isNotToBeSaved(client))
	{
		File << Parser::ObjectToLine(client, DELIMITER) << '\n';
	}
	File.close();

} // for one client

  void FileHandler::SaveClients(const std::vector<Client>& Clients) {

	std::fstream File;
	File.open(CLIENTS_FILE, std::ios::out);
	if (File.is_open()) {
		for (const Client& c : Clients)
		{
			if (!_isNotToBeSaved(c))
			{
				File << Parser::ObjectToLine(c, DELIMITER) << '\n';
			}

		}

		File.close();
	}


}

 void FileHandler::SaveUsers(const User& user) {

	std::fstream File;
	File.open(USERS_FILE, std::ios::out | std::ios::app);

	if (File.is_open() && !_isNotToBeSaved(user))
	{
		File << Parser::ObjectToLine(user, DELIMITER) << '\n';
	}
	File.close();

}

 void FileHandler::SaveUsers(const std::vector<User>& Users) {

	std::fstream File;
	File.open(USERS_FILE, std::ios::out);
	if (File.is_open()) {
		for (const User& c : Users)
		{
			if (!_isNotToBeSaved(c))
			{
				File << Parser::ObjectToLine(c, DELIMITER) << '\n';
			}

		}

		File.close();
	}


}
