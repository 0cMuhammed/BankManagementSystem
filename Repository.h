#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"
#include "FileHandler.h"
#include "Operations.h"

class Repository {

private :

	
	std::vector<Client> m_ClientList;
	

public :
	
	 Operations OperationsSection;

	 Repository() : m_ClientList(FileHandler::LoadFile()), OperationsSection(m_ClientList) {

	};

};




