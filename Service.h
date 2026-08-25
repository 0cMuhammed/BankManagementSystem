#pragma once
#include <iostream>


#include "Repository.h"
#include "FileHandler.h"
#include "ClientServices.h"
#include "UserServices.h"


class Service
{

private :
	
	UserServices m_UserServices;
	ClientServices m_ClientsServices;

public :
	Service(Repository &Ref) : m_UserServices( Ref.AccessUserRepository() ), m_ClientsServices( Ref.AccessClientRepository() ) {};

	 const ClientServices& AccessClientServices()  noexcept {
		return m_ClientsServices;
	}

	const UserServices& AccessUserServices()  noexcept {
		return m_UserServices;
	}

};

