#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"

class Repository {

private :

	
	 std::vector<Client> m_ClientList;

	static Client _GetEmptyObject()  noexcept {
		return Client("", "", "", "", "", "", 0, Client::ObjectMode::EmptyMode);
	}
	static Client _GetNewObject(const std::string& accountNumber) noexcept {
		return  Client("", "", "", "", accountNumber, "", 0, Client::ObjectMode::newMode);
	}
	
	static void _MakeEmpty(Client& client) {
		if (client.getMode() == Client::ObjectMode::DeleteMode)
		{
			client = _GetEmptyObject();
	    }
		
	}
	static bool _IsModifiable(const std::string &accountNumber, const Client& client) {
		return client.getAccountNumber() == accountNumber && client.getMode() == Client::ObjectMode::UpdateMode;
	}
	bool _DeleteObject(const std::string& accountNumber) {

		for (Client& client : m_ClientList)
		{
			if (_IsModifiable(accountNumber,client))
			{   
				client.setMode(Client::ObjectMode::DeleteMode);
				_MakeEmpty(client);
				return true;
			}

		}

		return false;

	}
	bool _UpdateObject(const std::string& accountNumber, const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newPinCode, double newBalance) {

		for (Client& client : m_ClientList)
		{
			if (_IsModifiable(accountNumber, client)) // a client has to be in update mode which is an the mode of an object when its already there when its not empty or new
			{
				client.SetObject(newFirstName, newLastName, newEmail, newPhoneNumber, newPinCode, newBalance);
				client.Save();
				return true;
			}

		}

		return false;

	}

	Client _FindObject(const std::string& accountNumber, const std::string* pinCodeParameter = nullptr) {

	

		for (const Client& client : m_ClientList) 
		{
			bool accountNumberMatch = (client.getAccountNumber() == accountNumber);
			bool pinCodeMatch = (pinCodeParameter == nullptr) ? true : client.getPinCode() == *pinCodeParameter;

			if (accountNumberMatch && pinCodeMatch)
			{
				return client;

			}
		}

		return _GetEmptyObject();
	}


	void _UpdateVector() {
		m_ClientList.clear();
		m_ClientList = FileHandler::LoadFile();
	}

public :
	
	enum  OperationStates { Failed = 1, Successful = 2, AccountNumberAlreadyExists = 3, AccountNumberNotFound = 4};



	 Repository() : m_ClientList( FileHandler::LoadFile() ) {
	                                                              //rule of zero std::vector manages itself
	 }
	
	  long long GetTotalBalance() {

		long long total = 0;
		

		for (const Client& client : m_ClientList)
		{
			total += client.getBalance();
		}

		return total;

	}

	  std::vector<Client> GetClientList() {
		 return m_ClientList;
	 }
	

	   Client Find(const std::string& accountNumber, const std::string* pinCode = nullptr) {
		     return _FindObject(accountNumber, pinCode);
	    }
	
	   bool IsExists(const std::string &accountNumber, const std::string* pinCode = nullptr) {
		 
		   Client client = _FindObject(accountNumber, pinCode);;
		   return (!client.isEmpty());
	  }

	   
	   
	   OperationStates AddClient(const std::string& accountNumber, const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newPinCode, double newBalance)
	 {
		 if ( IsExists(accountNumber) )
		 {
			 return OperationStates::AccountNumberAlreadyExists;
		 }

		 Client client = _GetNewObject(accountNumber);
		 client.SetObject(newFirstName, newLastName, newEmail, newPhoneNumber, newPinCode, newBalance);
		 client.Save();



		 return OperationStates::Successful;

	 }

	   OperationStates DeleteClient(const std::string& accountNumber) {
		 
		 if ( ! IsExists(accountNumber) )
		 {
			 return OperationStates::AccountNumberNotFound;
		 }

		 if ( ! _DeleteObject(accountNumber) ) 
		 {
			 return OperationStates::Failed;

		 }
		

		 FileHandler::SaveFile(m_ClientList);
		 _UpdateVector();
		 
		 return OperationStates::Successful;

	 }

	   OperationStates UpdateClient(const std::string& accountNumber, const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newPinCode, double newBalance) {
        
		   if (!IsExists(accountNumber))
		   {
			   return OperationStates::AccountNumberNotFound;
		   }

		   if (! _UpdateObject(accountNumber,newFirstName,newLastName,newEmail,newPhoneNumber,newPinCode,newBalance) )
		   {
			   return OperationStates::Failed;

		   }

		   FileHandler::SaveFile(m_ClientList);

		   return OperationStates::Successful;

	   }
	

	
};




