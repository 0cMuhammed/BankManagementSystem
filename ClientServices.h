#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"
#include "FileHandler.h"
#include "ClientRepository.h"

using State = ClientRepository::OperationStates;

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
	ClientRepository & AccessRepository() const noexcept {
		return m_RepositoryRef;
	}

	double GetTotalBalances() const {

		double total = 0;


		for (const Client& client : m_RepositoryRef.GetList())
		{
			total += client.getBalance();
		}

		return total;

	}

	State Withdraw(const std::string& AccountNumber, double amount) {

		Client toWithDrawFrom = m_RepositoryRef.Find(AccountNumber);

		if (  toWithDrawFrom.isEmpty()  )
		{
			return State::AccountNumberNotFound;
		}

		if (amount > toWithDrawFrom.getBalance()) 
		{
			return State::InsufficentBalance;
		}
		
		 
		
	    _Withdraw(toWithDrawFrom, amount);
		 m_RepositoryRef.UpdateClient(toWithDrawFrom);

	     return State::Successful;
		

	}
	State Deposit(const std::string& AccountNumber, double amount) {

		Client toDeposit  = m_RepositoryRef.Find(AccountNumber);

		if (toDeposit.isEmpty())
		{
			return State::AccountNumberNotFound;
		}


		_Deposit(toDeposit, amount);
		m_RepositoryRef.UpdateClient(toDeposit);

		return State::Successful;


	}

	State Withdraw(Client& ExistingClient, double amount) {



		if (ExistingClient.isEmpty())
		{
			return State::AccountNumberNotFound;
		}

		if (amount > ExistingClient.getBalance())
		{
			return State::InsufficentBalance;
		}



		_Withdraw(ExistingClient, amount);
		m_RepositoryRef.UpdateClient(ExistingClient);

		return State::Successful;


	}
	State Deposit(Client&  ExisitingClient, double amount) {


		if (ExisitingClient.isEmpty())
		{
			return State::AccountNumberNotFound;
		}


		_Deposit(ExisitingClient, amount);
		m_RepositoryRef.UpdateClient(ExisitingClient);

		return State::Successful;


	}


};


