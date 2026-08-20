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
	
	 Operations m_Operations;

	 Repository() : m_ClientList(FileHandler::LoadFile()), m_Operations(m_ClientList) {

	};

	
	
	 const std::vector<Client>& GetClientList() const {
		return m_ClientList;
	}



	  
	
	   

	
};




