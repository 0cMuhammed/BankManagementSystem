#pragma once
#include <iostream>
#include<vector>
#include<fstream>

#include "FileHandler.h"
#include "UserRepository.h"


// Wrapper Class for future User features, link this with the screens, ClientServices and UserServices shall be merged into one Services Class
using UserState = UserRepository::OperationStates;

class UserServices
{
private :
	UserRepository& m_RepositoryRef;

public :

	UserServices(UserRepository& Repo) : m_RepositoryRef(Repo) {};


	const UserRepository& AccessRepository()  const noexcept {
		return m_RepositoryRef; // read only 
	}
	UserRepository& AccessRepository()  noexcept {
		return m_RepositoryRef; // mutable
	}
};

