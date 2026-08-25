#pragma once
#include <iostream>
#include<vector>
#include<fstream>

#include "FileHandler.h"
#include "ClientRepository.h"

using ClientState = ClientRepository::OperationStates;

class ClientServices 
{

	
private :

	
	ClientRepository& m_RepositoryRef;

	void _Withdraw(Client &ToWithdrawFrom, double amount) {
		ToWithdrawFrom.setBalance(ToWithdrawFrom.getBalance() - amount);
	}
	void _Deposit(Client& ToDeposit, double amount) {
		ToDeposit.setBalance(ToDeposit.getBalance() + amount);
	}


public:

	ClientServices(ClientRepository& Repo) : m_RepositoryRef(Repo) {};

	const ClientRepository & AccessRepository() const noexcept {
		return m_RepositoryRef; // read only
	}
    ClientRepository& AccessRepository() noexcept {
		return m_RepositoryRef; // mutuable
	}

	double GetTotalBalances() const {

		double total = 0;


		for (const Client& client : m_RepositoryRef.GetList())
		{
			total += client.getBalance();
		}

		return total;

	}

	ClientState Withdraw(const std::string& AccountNumber, double amount) {

		Client toWithDrawFrom = m_RepositoryRef.Find(AccountNumber);

		if (  toWithDrawFrom.isEmpty()  )
		{
			return ClientState::AccountNumberNotFound;
		}

		if (amount > toWithDrawFrom.getBalance()) 
		{
			return ClientState::InsufficentBalance;
		}
		
		 
		
	    _Withdraw(toWithDrawFrom, amount);
		 m_RepositoryRef.UpdateClient(toWithDrawFrom);

	     return ClientState::Successful;
		

	}
	ClientState Deposit(const std::string& AccountNumber, double amount) {

		Client toDeposit  = m_RepositoryRef.Find(AccountNumber);

		if (toDeposit.isEmpty())
		{
			return ClientState::AccountNumberNotFound;
		}


		_Deposit(toDeposit, amount);
		m_RepositoryRef.UpdateClient(toDeposit);

		return ClientState::Successful;


	}

	ClientState Withdraw(Client& ExistingClient, double amount) {



		if (ExistingClient.isEmpty())
		{
			return ClientState::AccountNumberNotFound;
		}

		if (amount > ExistingClient.getBalance())
		{
			return ClientState::InsufficentBalance;
		}



		_Withdraw(ExistingClient, amount);
		m_RepositoryRef.UpdateClient(ExistingClient);

		return ClientState::Successful;


	}
	ClientState Deposit(Client&  ExisitingClient, double amount) {


		if (ExisitingClient.isEmpty())
		{
			return ClientState::AccountNumberNotFound;
		}


		_Deposit(ExisitingClient, amount);
		m_RepositoryRef.UpdateClient(ExisitingClient);

		return ClientState::Successful;


	}


};


