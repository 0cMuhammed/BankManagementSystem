#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Person.h"
#include "FileHandler.h";
#include "Parser.h";

class Client : public Person {

protected :
	enum  mode { EmptyMode = 1, UpdateMode = 2, newMode = 3, DeleteMode = 4 };
	enum  saveStates { Failed = 1, Successful = 2, AccountNumberAlreadyExists = 3 };
	void setMode(const Client &client,const mode& Mode) {
		m_mode = Mode;
	}
	

private:
	

	std::string m_accountNumber = "";
	std::string m_pinCode = "";
	double m_balance = 0;
	mode m_mode = mode::EmptyMode;

	

	
	
	static Client _getNewObject(const std::string &accountNumber) noexcept {
		return  Client("", "", "", "", accountNumber, "", 0, mode::newMode);
	}
	static Client _getEmptyObject()  noexcept {
		return Client("", "", "", "", "", "", 0, mode::EmptyMode);
	}
	
	
	void _SaveExistingObject() {
		
		std::vector<Client> Clients = FileHandler::LoadFile();

		for (Client& client : Clients) 
		{
			if (client.m_accountNumber == m_accountNumber) 
			{
				client = *this;
				break;

			}

		}

		FileHandler::SaveFile(Clients);
		
	}

	void _SaveNewObject() {
		
       m_mode = mode::UpdateMode;
	   FileHandler::SaveFile(*this);
		

    }
		

	void _SetObject(const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newPinCode, double newBalance) {
		setFirstName(newFirstName);
		setLastName(newLastName);
		setEmail(newEmail);
		setPhoneNumber(newPhoneNumber);
		setPinCode(newPinCode);
		setBalance(newBalance);
	}

	
	void _MarkDelete (const std::string& accountNumber,std::vector<Client>& Clients) {

		for (Client& client : Clients) 
		{
			if (client.m_accountNumber == accountNumber) 
			{
				client.m_mode = mode::DeleteMode;
				break;

			}

		}

	}

	void _MakeEmpty() {
	   *this = _getEmptyObject(); 
	}

	// in repository
	void _DeleteObject(const std::string& accountNumber) {
		std::vector<Client> clients = FileHandler::LoadFile();

		_MarkDelete(accountNumber,clients);
		FileHandler::SaveFile(clients); //delete in file
		_MakeEmpty();       // null it out in memory
		
		
		
	}

	// in repository
	static Client _find(const std::string& accountNumber, const std::string *pinCodeParameter = nullptr) {


		std::fstream file;



		file.open("Clients.txt", std::ios::in);

		if (file.is_open())
		{
			std::string line;


			while (!(std::getline(file, line).fail()))
			{

				Client client = Parser::LineToObject(line);

				bool accountNumberMatch = (client.m_accountNumber == accountNumber);
				bool pinCodeMatch = (pinCodeParameter == nullptr) ? true : client.m_pinCode == *pinCodeParameter;

				if (accountNumberMatch && pinCodeMatch)
				{
					file.close();
					return client;

				}


			}
			file.close();


		}

		return _getEmptyObject();
	}  

	

public:
	
	
	Client(const std::string& firstName, const std::string& lastName, const std::string& email, const std::string& phoneNumber, const std::string& accountNumber, const std::string& pinCode, double balance, const mode& clientMode) : Person(firstName, lastName, email, phoneNumber), m_accountNumber(accountNumber), m_pinCode(pinCode), m_balance(balance), m_mode(clientMode) {};

	bool operator==(const Client& client) const {
		return (this == &client || ( getFirstName() == client.getFirstName() && getLastName() == client.getLastName() && getEmail() == client.getEmail() && getPhoneNumber() == client.getPhoneNumber() && m_accountNumber == client.m_accountNumber && m_pinCode == client.m_pinCode && m_balance == client.m_balance && m_mode == client.m_mode) );
	}
	bool operator !=(const Client& client) const {
		return !(*this == client);
	}
	
	bool isEmpty() const noexcept {
		return (m_mode == mode::EmptyMode);
	}

	const std::string& getAccountNumber() const noexcept {
		return m_accountNumber;
	}
	const std::string& getPinCode() const noexcept {
		return m_pinCode;
	}
	double getBalance() const noexcept {
		return m_balance;
	}
	const mode& getMode() const noexcept {
		return m_mode;
	}
	void setPinCode(const std::string& newPinCode) {
			m_pinCode = newPinCode; 
	}

	void setBalance(double newBalance) {
		m_balance = newBalance;
	}
	

	// in repository
	static Client search(const std::string& accountNumber, const std::string &pinCode) {
		return _find(accountNumber,&pinCode);
	}
	// in repository
	static Client search(const std::string& accountNumber) {
		return _find(accountNumber);
	}

	// in repository
	static bool isClientExists(const std::string& accountNumber, const std::string & pinCode) {
		Client client = search(accountNumber,pinCode);

		return (!client.isEmpty());
	}
	// in repository
	static bool isClientExists(const std::string& accountNumber) {
		Client client = search(accountNumber);

		return (!client.isEmpty());
	}



	static saveStates AddClient(const std::string& accountNumber, const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newPinCode, double newBalance)
	{
		if (isClientExists(accountNumber)) 
		{
			return saveStates::AccountNumberAlreadyExists;
		}
	   
		Client client = _getNewObject(accountNumber);
		client._SetObject(newFirstName, newLastName, newEmail, newPhoneNumber, newPinCode, newBalance);
		client.Save();

	
		
		return saveStates::Successful;
		
	}
	static bool DeleteClient(const std::string& accountNumber) {

		Client client = search(accountNumber);

		if (client.isEmpty())
		{
			return false;
		}

		
		
		return client.DeleteClient();
		

	}
	bool DeleteClient() {
		if (m_mode == mode::UpdateMode)
		{

			_DeleteObject(m_accountNumber);
			return true;
		}

		return false;

	}
    saveStates UpdateClient(const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newPinCode, double newBalance) {
		 
	
		if (m_mode == mode::UpdateMode )
	    {
			        _SetObject(newFirstName, newLastName, newEmail, newPhoneNumber, newPinCode, newBalance);
					_SaveExistingObject();
					return saveStates::Successful;
		}

		return saveStates::Failed;

		
	}

	void Save() {
		 // for any future save state if the object is not empty 
		switch (m_mode)
		{
		case mode::EmptyMode:
		{		
			return;
		}
		case mode::UpdateMode:
		{
			_SaveExistingObject();
			break;
		}
		case mode::newMode: 
		{
			_SaveNewObject();
			break;

		}
		default :
			return;
			
		}
	


	}

	void Print() {
		std::cout << "\nClient Card:";
		std::cout << "\n___________________";
		std::cout << "\nFirstName   : " << getFirstName();
		std::cout << "\nLastName    : " << getLastName();
		std::cout << "\nFull Name   : " << getFullName();
		std::cout << "\nEmail       : " << getEmail();
		std::cout << "\nPhone       : " << getPhoneNumber();
		std::cout << "\nAcc. Number : " << m_accountNumber;
		std::cout << "\nPassword    : " << m_pinCode;
		std::cout << "\nBalance     : " << m_balance;
		std::cout << "\n___________________\n";

	}



};