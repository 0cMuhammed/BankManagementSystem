#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "User.h"
#include "FileHandler.h"
#include "UserRepository.h"

using State = UserRepository::OperationStates;

// Wrapper Class for future User features, link this with the screens, ClientServices and UserServices shall be merged into one Services Class

class UserServices
{
private :
	UserRepository& m_RepositoryRef;

public :

	UserServices(UserRepository& Repo) : m_RepositoryRef(Repo) {};

	UserRepository& AccessRepository() const noexcept {
		return m_RepositoryRef;
	}

};

