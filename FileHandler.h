#pragma once
#include<vector>
#include<fstream>
#include "Parser.h";



class FileHandler {

private :
	static  bool _isNotToBeSaved(const Client& client) noexcept;
	static  bool _isNotToBeSaved(const User& user) noexcept;

	static constexpr const char * ClientsFile = "Clients.txt";
	static constexpr const char * UsersFile = "Users.txt";
	static constexpr const char*  delimiter = "#//#";

public :

	static std::vector<Client> LoadClients();
	static std::vector<User> LoadUsers();
	static void SaveClients(const Client& client);
	static void SaveClients(const std::vector<Client> &Clients);

	static void SaveUsers(const User& user);
	static void SaveUsers(const std::vector<User> &Users);

};




#include "Client.h"
#include "User.h"
inline  bool FileHandler::_isNotToBeSaved(const Client& client) noexcept {
	const Client::ObjectMode Mode = client.getMode();
	return	(Mode == Client::ObjectMode::EmptyMode || Mode == Client::ObjectMode::DeleteMode);
}
inline  bool FileHandler::_isNotToBeSaved(const User& user) noexcept {
	const User::ObjectMode Mode = user.GetMode();
	return	(Mode == User::ObjectMode::EmptyMode || Mode == User::ObjectMode::DeleteMode);
}

inline std::vector<Client>FileHandler::LoadClients() {
	std::fstream file;
	std::vector<Client> Clients;
	file.open(ClientsFile, std::ios::in);

	if (file.is_open())
	{
		std::string dataline = "";

		while (!(std::getline(file, dataline).fail()) && (!dataline.empty()))
		{
			Clients.emplace_back( Parser::LineToClient( std::move(dataline) ) ); // internally std::move(dataline) to to Tokens() if passed rvalue, if a lvalue is passed a copy would happen
		}
		file.close();
	}

	return Clients;
}
inline std::vector<User>  FileHandler::LoadUsers() {
	std::fstream file;
	std::vector<User> Users;
	file.open(UsersFile, std::ios::in);

	if (file.is_open())
	{
		std::string dataline = "";

		while (!(std::getline(file, dataline).fail()) && (!dataline.empty()))
		{
			Users.emplace_back(Parser::LineToUser(std::move(dataline))); // internally std::move(dataline) to to Tokens() if passed rvalue, if a lvalue is passed a copy would happen
		}
		file.close();
	}

	return Users;
}


inline  void FileHandler::SaveClients(const Client& client) {

	std::fstream File;
	File.open(ClientsFile, std::ios::out | std::ios::app);

	if (File.is_open() && !_isNotToBeSaved(client))
	{
		File << Parser::ObjectToLine(client,delimiter) << '\n';
	}
	File.close();

} // for one client

inline  void FileHandler::SaveClients(const std::vector<Client> &Clients) {

	std::fstream File;
	File.open(ClientsFile, std::ios::out);
	if (File.is_open()) {
		for (const Client  &c : Clients)
		{
			if (!_isNotToBeSaved(c))
			{
				File << Parser::ObjectToLine(c,delimiter) << '\n'; 
			}

		}

		File.close();
	}


}

inline  void FileHandler::SaveUsers(const User& user) {

	std::fstream File;
	File.open(UsersFile, std::ios::out | std::ios::app);

	if (File.is_open() && !_isNotToBeSaved(user))
	{
		File << Parser::ObjectToLine(user, delimiter) << '\n';
	}
	File.close();

} 

inline  void FileHandler::SaveUsers(const std::vector<User>& Users) {

	std::fstream File;
	File.open(UsersFile, std::ios::out);
	if (File.is_open()) {
		for (const User& c : Users)
		{
			if (!_isNotToBeSaved(c))
			{
				File << Parser::ObjectToLine(c,delimiter) << '\n';
			}

		}

		File.close();
	}


}

