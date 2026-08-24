#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"
#include "FileHandler.h"
#include"Validator.h"

class ClientRepository {

public :
	enum  OperationStates { NotConfirmed = 0, Failed = 1, Successful = 2, AccountNumberAlreadyExists = 3, AccountNumberNotFound = 4,  InsufficentBalance = 5};

private :

	std::vector<Client> m_List;
	

	void _UpdateVector() {
		m_List.clear();
		m_List = FileHandler::LoadClients();
	}

	void _AddInVector(const Client& client) {
		m_List.push_back(client);
	}

	Client _GetEmptyObject() const noexcept {
		return Client("", "", "", "", "", "", 0, Client::ObjectMode::EmptyMode);
	}

	Client  _GetNewObject(const std::string& accountNumber) const noexcept {
		return  Client("", "", "", "", accountNumber, "", 0, Client::ObjectMode::newMode);
	}

	void _MakeEmpty(Client& client) {
		if (client.getMode() == Client::ObjectMode::DeleteMode)
		{
			client = _GetEmptyObject();
		}

	}

	static bool _IsModifiable(const Client& client, const std::string& accountNumber) {
		return client.getAccountNumber() == accountNumber && client.getMode() == Client::ObjectMode::ExistingMode;
	}

	static void _Message(const char* Message) {
		std::cout << '\n' + Message;
	}

	bool _DeleteObject(const std::string& accountNumber) {


		for (Client& c : m_List)
		{
			if (_IsModifiable(c, accountNumber))
			{
				c.setMode(Client::ObjectMode::DeleteMode);
				_MakeEmpty(c);
				return true;
			}

		}


		return false;

	}
	bool _DeleteObject(const Client& client) {


		for (Client& c : m_List)
		{
			if (_IsModifiable(c, client.getAccountNumber()))
			{
				c.setMode(Client::ObjectMode::DeleteMode);
				_MakeEmpty(c);
				return true;
			}

		}


		return false;

	}
	bool _UpdateObject(const std::string& accountNumber, const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newPinCode, double newBalance) {

		for (Client& c : m_List)
		{
			if (_IsModifiable(c, accountNumber))
			{
				c.SetObject(newFirstName, newLastName, newEmail, newPhoneNumber, newPinCode, newBalance);
				c.Save();
				return true;
			}

		}

		return false;

	}
	bool _UpdateObject(Client& client) {

		for (Client& c : m_List)
		{
			if (_IsModifiable(c, client.getAccountNumber()))
			{
				c = std::move(client);
				c.Save();
				return true;
			}

		}

		return false;

	}


	Client  _FindObject(const std::string& accountNumber, const char* pinCodeParameter = nullptr) const {



		for (const Client& client : m_List)
		{
			bool accountNumberMatch = (client.getAccountNumber() == accountNumber);
			bool pinCodeMatch = (pinCodeParameter == nullptr) ? true : (client.getPinCode().c_str()) == pinCodeParameter;

			if (accountNumberMatch && pinCodeMatch)
			{
				return client;

			}
		}


		return _GetEmptyObject();
	}


public :
	

	 ClientRepository() : m_List(FileHandler::LoadClients()) {};

	 const std::vector<Client>& GetList() const {
		 return m_List;
	 }

	 static void PrintClient(const Client& client) {

		 std::cout << "\nClient Card:";
		 std::cout << "\n___________________";
		 std::cout << "\nFirstName   : " << client.GetFirstName();
		 std::cout << "\nLastName    : " << client.GetLastName();
		 std::cout << "\nFull Name   : " << client.GetFullName();
		 std::cout << "\nEmail       : " << client.GetEmail();
		 std::cout << "\nPhone       : " << client.GetPhoneNumber();
		 std::cout << "\nAcc. Number : " << client.getAccountNumber();
		 std::cout << "\nPassword    : " << client.getPinCode();
		 std::cout << "\nBalance     : " << client.getBalance();
		 std::cout << "\n___________________\n";


	 }
	 static Client ReadClient()
	 {
		 _Message("Enter your account number");
		 std::string AccountNumber = Validator::ReadString();

		 _Message("Enter first name");
		 std::string FirstName = Validator::ReadString();

		 _Message("Enter last name");
		 std::string LastName = Validator::ReadString();

		 _Message("Enter Email");
		 std::string Email = Validator::ReadString();

		 _Message("Enter Phone");
		 std::string Phone = Validator::ReadString();

		 _Message("Enter PinCode");
		 std::string PinCode = Validator::ReadString();

		 _Message("Enter Account Balance");
		 double Balance = Validator::returnNumber("Invalid Number, Enter again");

		 return Client(FirstName, LastName, Email, Phone, AccountNumber, PinCode, Balance, Client::ObjectMode::newMode);
	 }
	 static Client ReadClient(const std::string& ExistingAccountNumber)
	 {

		 _Message("Enter first name");
		 std::string FirstName = Validator::ReadString();

		 _Message("Enter last name");
		 std::string LastName = Validator::ReadString();

		 _Message("Enter Email");
		 std::string Email = Validator::ReadString();

		 _Message("Enter Phone");
		 std::string Phone = Validator::ReadString();

		 _Message("Enter PinCode");
		 std::string PinCode = Validator::ReadString();

		 _Message("Enter Account Balance");
		 double Balance = Validator::returnNumber("Invalid Number, Enter again");

		 return Client(FirstName, LastName, Email, Phone, ExistingAccountNumber, PinCode, Balance, Client::ObjectMode::newMode);
	 }


	 bool IsExists(const std::string& accountNumber, const char* pinCode = nullptr) {

		 Client client = _FindObject(accountNumber, pinCode);
		 return (!client.isEmpty());
	 }

	
	 Client Find(const std::string& accountNumber, const char* pinCode = nullptr) {
		 return _FindObject(accountNumber, pinCode);
	 }

	 OperationStates AddClient(Client& FilledObject)
	 {
		 if (IsExists(FilledObject.getAccountNumber()))
		 {
			 return OperationStates::AccountNumberAlreadyExists;
		 }


		 FilledObject.setMode(Client::ObjectMode::newMode);
		 FilledObject.Save();
		 _AddInVector(FilledObject);



		 return OperationStates::Successful;

	 }

	 OperationStates DeleteClient(const Client& ExistingObject) {


		 if (!_DeleteObject(ExistingObject))
		 {
			 return OperationStates::Failed;

		 }



		 FileHandler::SaveClients(m_List);
		 _UpdateVector();

		 return OperationStates::Successful;

	 }
	 OperationStates DeleteClient(const std::string& accountNumber) {

		 if (!IsExists(accountNumber))
		 {
			 return OperationStates::AccountNumberNotFound;
		 }

		 if (_DeleteObject(accountNumber))
		 {
			 return OperationStates::Failed;

		 }



		 FileHandler::SaveClients(m_List);
		 _UpdateVector();

		 return OperationStates::Successful;

	 }

	 OperationStates UpdateClient(const std::string& accountNumber, const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newPinCode, double newBalance) {

		 if (!IsExists(accountNumber))
		 {
			 return OperationStates::AccountNumberNotFound;
		 }

		 if (!_UpdateObject(accountNumber, newFirstName, newLastName, newEmail, newPhoneNumber, newPinCode, newBalance))
		 {
			 return OperationStates::Failed;

		 }

		 FileHandler::SaveClients(m_List);

		 return OperationStates::Successful;

	 }
	 OperationStates UpdateClient(Client& ExistingObject) {



		 if (!_UpdateObject(ExistingObject))
		 {
			 return OperationStates::Failed;

		 }

		 FileHandler::SaveClients(m_List);

		 return OperationStates::Successful;

	 }


};




