#pragma once
#include <iostream>
#include<vector>
#include<fstream>

#include "FileHandler.h"
#include "ClientRepository.h"

class Transactions
{
private :

	ClientRepository& m_RepositoryRef;

	void _Withdraw(Client& ToWithdrawFrom, double amount) {
		ToWithdrawFrom.setBalance(ToWithdrawFrom.getBalance() - amount);
	}
	void _Deposit(Client& ToDeposit, double amount) {
		ToDeposit.setBalance(ToDeposit.getBalance() + amount);
	}

public :
	Transactions(ClientRepository& Ref) : m_RepositoryRef(Ref) {};

	ClientRepository::OperationStates Withdraw(const std::string& AccountNumber, double amount) {

		Client toWithDrawFrom = m_RepositoryRef.Find(AccountNumber);

		if (toWithDrawFrom.isEmpty())
		{
			return ClientRepository::OperationStates::AccountNumberNotFound;
		}

		if (amount > toWithDrawFrom.getBalance())
		{
			return ClientRepository::OperationStates::InsufficentBalance;
		}



		_Withdraw(toWithDrawFrom, amount);
		m_RepositoryRef.UpdateClient(toWithDrawFrom);

		return ClientRepository::OperationStates::Successful;


	}
	ClientRepository::OperationStates Deposit(const std::string& AccountNumber, double amount) {

		Client toDeposit = m_RepositoryRef.Find(AccountNumber);

		if (toDeposit.isEmpty())
		{
			return ClientRepository::OperationStates::AccountNumberNotFound;
		}


		_Deposit(toDeposit, amount);
		m_RepositoryRef.UpdateClient(toDeposit);

		return ClientRepository::OperationStates::Successful;


	}

	ClientRepository::OperationStates Withdraw(Client& ExistingClient, double amount) {



		if (ExistingClient.isEmpty())
		{
			return ClientRepository::OperationStates::AccountNumberNotFound;
		}

		if (amount > ExistingClient.getBalance())
		{
			return ClientRepository::OperationStates::InsufficentBalance;
		}



		_Withdraw(ExistingClient, amount);
		m_RepositoryRef.UpdateClient(ExistingClient);

		return ClientRepository::OperationStates::Successful;


	}
	ClientRepository::OperationStates Deposit(Client& ExisitingClient, double amount) {


		if (ExisitingClient.isEmpty())
		{
			return ClientState::AccountNumberNotFound;
		}


		_Deposit(ExisitingClient, amount);
		m_RepositoryRef.UpdateClient(ExisitingClient);

		return ClientRepository::OperationStates::Successful;


	}


	ClientRepository::OperationStates Transfer(const std::string& FromAccNum, const std::string& ToAccNum, double amount) {

		Client From = m_RepositoryRef.Find(FromAccNum);

		if (From.isEmpty())
		{
			return ClientRepository::OperationStates::AccountNumberNotFound;
		}

		Client To = m_RepositoryRef.Find(ToAccNum);

		if (To.isEmpty())
		{
			return ClientRepository::OperationStates::AccountNumberNotFound;
		}

		if (amount > From.getBalance())
		{
			return ClientRepository::OperationStates::InsufficentBalance;
		}

		Withdraw(From, amount);
		Deposit(To, amount);

		m_RepositoryRef.UpdateClient(From);
		m_RepositoryRef.UpdateClient(To);

		return ClientRepository::OperationStates::Successful;
	}
	ClientRepository::OperationStates Transfer(Client& From, Client& To, double amount) {

		if (amount > From.getBalance())
		{
			return ClientRepository::OperationStates::InsufficentBalance;
		}

		Withdraw(From, amount);
		Deposit(To, amount);

		m_RepositoryRef.UpdateClient(From);
		m_RepositoryRef.UpdateClient(To);

		return ClientRepository::OperationStates::Successful;
	}
};

