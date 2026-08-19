#pragma once
#include<vector>
#include<fstream>
#include "Person.h"

class FileHandler;

class Client : public Person {

public :


	enum  ObjectMode { EmptyMode = 1, ExistingMode = 2, newMode = 3, DeleteMode = 4 };

private:
	
	
	std::string m_accountNumber = "";
	std::string m_pinCode = "";
	double m_balance = 0;
    ObjectMode m_mode = ObjectMode::EmptyMode;
	
	
	//using FileHandler
	void _SaveExistingObject();
	void _SaveNewObject(); 


public:

	
	Client(std::string firstName, std::string lastName, std::string email, std::string phoneNumber, std::string accountNumber, std::string pinCode, double balance, ObjectMode Mode) : Person(std::move(firstName), std::move(lastName),std::move( email), std::move(phoneNumber)), m_accountNumber(std::move(accountNumber)), m_pinCode(std::move(pinCode)), m_balance(balance), m_mode(Mode) {};
	

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
		case ObjectMode::ExistingMode:
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
	m_mode = ObjectMode::ExistingMode;
	FileHandler::SaveFile(*this);
}



