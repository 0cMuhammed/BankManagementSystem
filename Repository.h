#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "ClientRepository.h"
#include "UserRepository.h"

#include "FileHandler.h"
#include"Validator.h"

class Repository
{
private :

	UserRepository   m_UserRepo;
	ClientRepository m_ClientRepo;

public :
	Repository() : m_UserRepo(FileHandler::LoadUsers()), m_ClientRepo(FileHandler::LoadClients()) {};
	
	//read only access
	const ClientRepository & AccessClientRepository() const  noexcept {
		return m_ClientRepo;
	}
	const UserRepository &AccessUserRepository()  const noexcept {
		return m_UserRepo;
	}

	//mutuable accessors
	ClientRepository& AccessClientRepository()  noexcept {
		return m_ClientRepo;
	}
	UserRepository& AccessUserRepository()  noexcept {
		return m_UserRepo;
	}

};

