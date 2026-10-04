#pragma once
#include <iostream>
#include<vector>
#include<fstream>

#include "FileHandler.h"
#include "ClientRepository.h"
#include "Transactions.h"

using ClientState = ClientRepository::OperationStates;

class ClientServices 
{

private :

	
	ClientRepository& m_RepositoryRef;
	Transactions m_TransactionsRef;



public:

	ClientServices(ClientRepository& Repo) : m_RepositoryRef(Repo), m_TransactionsRef(Repo) {};

	const ClientRepository & AccessRepository() const noexcept {
		return m_RepositoryRef; // read only
	}

    ClientRepository& AccessRepository() noexcept {
		return m_RepositoryRef; // mutuable
	}
    Transactions &AccessTransactions() noexcept {
		return m_TransactionsRef; // mutuable, no read-only since transactions always changes clients balance
	}
	

	double GetTotalBalances() const {

		double total = 0;


		for (const Client& client : m_RepositoryRef.GetList())
		{
			total += client.getBalance();
		}

		return total;

	}


};


