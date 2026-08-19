#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"
#include "FileHandler.h"
#include "Operations.h"

class BankOperations 
{
private :

	std::vector<Client>& m_List; // dependency injection for m_ClientList in Repository.h, BankOperations() -> Operations() --> m_ClientList in Repository

	void _Withdraw(double amount, Client &client) {
		client.setBalance(client.getBalance() - amount);
	}
	void _Deposit(double amount, Client& ToDeposit) {
		ToDeposit.setBalance(ToDeposit.getBalance() + amount);
	}
};

