#pragma once
#include<vector>
#include<fstream>
#include "Person.h"

class FileHandler;

class Client : public Person {

public :


	enum  ObjectMode { EmptyMode = 1, UpdateMode = 2, newMode = 3, DeleteMode = 4 };

private:
	
	
	std::string m_accountNumber = "";
	std::string m_pinCode = "";
	double m_balance = 0;
    ObjectMode m_mode = ObjectMode::EmptyMode;
	
	
	//using FileHandler
	void _SaveExistingObject();
	void _SaveNewObject(); 


public:

	
	Client(const std::string& firstName, const std::string& lastName, const std::string& email, const std::string& phoneNumber, const std::string& accountNumber, const std::string& pinCode, double balance, const ObjectMode &Mode) : Person(firstName, lastName, email, phoneNumber), m_accountNumber(accountNumber), m_pinCode(pinCode) , m_balance(balance), m_mode(Mode){};
	

	bool operator==(const Client& client) const {
		return (this == &client || ( getFirstName() == client.getFirstName() && getLastName() == client.getLastName() && getEmail() == client.getEmail() && getPhoneNumber() == client.getPhoneNumber() && m_accountNumber == client.m_accountNumber && m_pinCode == client.m_pinCode && m_balance == client.m_balance && m_mode == client.m_mode) );
	}
	bool operator !=(const Client& client) const {
		return !(*this == client);
	}
	
	bool isEmpty() const noexcept {
		return (m_mode == ObjectMode::EmptyMode);
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

	void setPinCode(const std::string& newPinCode) {
			m_pinCode = newPinCode; 
	}
	void setBalance(double newBalance) {
		m_balance = newBalance;
	}
	
	void setMode(ObjectMode mode) {
		m_mode = mode;
	}
	ObjectMode getMode() const noexcept {
		return m_mode;
	}
	
	void SetObject(const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newPinCode, double newBalance) {
		setFirstName(newFirstName);
		setLastName(newLastName);
		setEmail(newEmail);
		setPhoneNumber(newPhoneNumber);
		setPinCode(newPinCode);
		setBalance(newBalance);
	}
	
	void Save() {
		 // for any future save state if the object is not empty, apply strategy pattern here because the way of how its saved can be changed according the mode of the object in a different class
		switch (m_mode)
		{
		case ObjectMode::EmptyMode:
		{		
			return;
		}
		case ObjectMode::UpdateMode:
		{
			_SaveExistingObject();
			break;
		}
		case ObjectMode::newMode:
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

#include "FileHandler.h"
inline void Client::_SaveExistingObject() {
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
inline void Client::_SaveNewObject() {
	m_mode = ObjectMode::UpdateMode;
	FileHandler::SaveFile(*this);
}



