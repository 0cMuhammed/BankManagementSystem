#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"
#include "FileHandler.h"


class BankOperations 
{
private :
	

	Operations& m_OpearationsReference; // dependency injection for Operations to accsess Update Methods, BankOperations() -> Operations() --> m_ClientList in Repository

	 

	void _Withdraw(Client &ToWithdrawFrom, double amount) {
		ToWithdrawFrom.setBalance(ToWithdrawFrom.getBalance() - amount);
	}
	void _Deposit(Client& ToDeposit, double amount) {
		ToDeposit.setBalance(ToDeposit.getBalance() + amount);
	}

public:
	BankOperations(Operations& Repo) : m_OpearationsReference(Repo) {};

	Operations::OperationStates Withdraw(const std::string& AccountNumber, double amount) {

		Client toWithDrawFrom = m_OpearationsReference.Find(AccountNumber);

		if (toWithDrawFrom.isEmpty())
		{
			return Operations::OperationStates::AccountNumberNotFound;
		}

		if (amount > toWithDrawFrom.getBalance()) 
		{
			return Operations::OperationStates::InsufficentBalance;
		}
		
		 
		
	    _Withdraw(toWithDrawFrom, amount);
		 m_OpearationsReference.UpdateClient(toWithDrawFrom);

	     return Operations::OperationStates::Successful;
		

	}
	Operations::OperationStates Deposit(const std::string& AccountNumber, double amount) {

		Client toDeposit  = m_OpearationsReference.Find(AccountNumber);

		if (toDeposit.isEmpty())
		{
			return Operations::OperationStates::AccountNumberNotFound;
		}


		_Deposit(toDeposit, amount);
		m_OpearationsReference.UpdateClient(toDeposit);

		return Operations::OperationStates::Successful;


	}


};


