#pragma once
#include <iostream>


#include "Repository.h"
#include "FileHandler.h"
#include "ClientServices.h"
#include "UserServices.h"
#include "Authenticator.h"

class Service
{

private :
	
	UserServices m_UserServices;
	ClientServices m_ClientsServices;
	Authenticator m_Auth;

public :
	//Repository outlives Service btw
	Service(Repository &Ref) : m_UserServices( Ref.AccessUserRepository() ), m_ClientsServices( Ref.AccessClientRepository() ), m_Auth(Ref.AccessUserRepository()) {}; 


	ClientServices& AccessClientServices()  noexcept { // mutates m_ClientServices state
		return m_ClientsServices;
	}
	
	UserServices& AccessUserServices()  noexcept { // mutates m_UserServices state
		return m_UserServices;
	}
	Authenticator& AccessAuthenticator() noexcept {
		return m_Auth; //mutates
	}
	const Authenticator& AccessAuthenticator() const noexcept {
		return m_Auth; // read only
	}
	const ClientServices& AccessClientServices() const noexcept { // read only
		return m_ClientsServices;
	}

	const UserServices& AccessUserServices() const  noexcept { // read only 
		return m_UserServices;
	}


};

