#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Person.h"
#include "FileHandler.h";
#include "Parser.h";
#include "Client.h"

class Repository  {

private :
	std::vector<Client> m_ClientList;
	

public :
	 Repository() : m_ClientList( FileHandler::LoadFile() ) {
	 //rule of zero, vector structure manages itself
	}
	

	 long long getTotalBalance() {

		long long total = 0;
		

		for (const Client& client : m_ClientList)
		{
			total += client.getBalance();
		}

		return total;

	}
	 std::vector<Client> GetClientList() {
		 return m_ClientList;
	 }
	 void UpdateVector() {
		 m_ClientList.clear();
		 Repository();
	 }

	


};

