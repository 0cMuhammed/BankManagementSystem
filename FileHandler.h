#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"
#include "Parser.h";
class FileHandler : protected Client {
private :
	
	static bool _isNotToBeSaved(const Client& client)  noexcept {

		return	(client.getMode()  == Client::mode::EmptyMode || client.getMode() == Client::mode::DeleteMode);
	}

public :

	static std::vector<Client> LoadFile() {
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
	static void SaveFile(const Client& client) {

		std::fstream File;
		File.open("Clients.txt", std::ios::out | std::ios::app);

		if ( File.is_open() && ! _isNotToBeSaved(client) )
		{
			File << Parser::ObjectToLine(client) << '\n';
		}
		File.close();

	} // for one client
	static void SaveFile(const std::vector<Client>& Clients) {

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


	} // for the whole list
    
	

	
};

