#pragma once
#include<vector>
#include<fstream>
#include "Parser.h";



class FileHandler {

private :
	static  bool _isNotToBeSaved(const Client& client) noexcept;
	

public :

	static std::vector<Client> LoadFile();
	static void SaveFile(const Client& client);
	static void SaveFile(const std::vector<Client>& Clients);
};

#include "Client.h"
inline  bool FileHandler::_isNotToBeSaved(const Client& client) noexcept {
	const Client::ObjectMode Mode = client.getMode();
	return	(Mode == Client::ObjectMode::EmptyMode || Mode == Client::ObjectMode::DeleteMode);
}
inline std::vector<Client>  FileHandler::LoadFile() {
	std::fstream file;
	std::vector<Client> Clients;
	file.open("Clients.txt", std::ios::in);

	if (file.is_open())
	{
		std::string dataline;

		while (!(std::getline(file, dataline).fail()))
		{
			Clients.emplace_back(Parser::LineToObject(dataline));
		}
		file.close();
	}

	return Clients;
}
inline  void FileHandler::SaveFile(const Client& client) {

	std::fstream File;
	File.open("Clients.txt", std::ios::out | std::ios::app);

	if (File.is_open() && !_isNotToBeSaved(client))
	{
		File << Parser::ObjectToLine(client) << '\n';
	}
	File.close();

} // for one client
inline  void FileHandler::SaveFile(const std::vector<Client>& Clients) {

	std::fstream File;
	File.open("Clients.txt", std::ios::out);
	if (File.is_open()) {
		for (const Client& client : Clients)
		{
			if (!_isNotToBeSaved(client))
			{
				File << Parser::ObjectToLine(client) << '\n';
			}

		}

		File.close();
	}


}
