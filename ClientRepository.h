#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"
#include "Hasher.h"
#include "FileHandler.h"
#include"Validator.h"



class ClientRepository {

public :

	enum class OperationStates : uint8_t { NotConfirmed = 0, Failed = 1, Successful = 2, AccountNumberAlreadyExists = 3, AccountNumberNotFound = 4,  InsufficentBalance = 5};

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


	static void _Message(const std::string& Message) {
		term::Prompt(Message);   // NEW: prompts coming from the repository use the same style as the screens
	}


	std::string  _ReadAccountNumber() {

		_Message("ACCOUNT NUMBER");
		std::string AccountNumber = Validator::ReadString();

		while (IsExists(AccountNumber) || Validator::IsInvalid(AccountNumber)) {

			term::Error("Account number is already used or invalid.");
			_Message("ACCOUNT NUMBER");

			AccountNumber = Validator::ReadString();
		}

		return AccountNumber;
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


	 Client  _FindObject(const std::string& accountNumber, const char* pinCodeParameter = nullptr)  const {



		for (const Client& client : m_List)
		{
			bool accountNumberMatch = (client.getAccountNumber() == accountNumber);
			bool pinCodeMatch = (pinCodeParameter == nullptr) ? true : ( Hasher::VerifyUser( std::string(pinCodeParameter), client.getPinCode() ) ) ;

			if (accountNumberMatch && pinCodeMatch)
			{
				return client;

			}
		}


		return _GetEmptyObject();
	}


public :
	

	 ClientRepository(std::vector<Client> Clients) : m_List(std::move(Clients)) {};

	 const std::vector<Client>& GetList() const {
		 return m_List;
	 }


	 static Client ReadNewClient()
	 {
		 
		 _Message("ACCOUNT NUMBER");
		 std::string AccountNumber = Validator::ReadString();
		 
		 _Message("FIRST NAME");
		 std::string FirstName = Validator::ReadString();

		 _Message("LAST NAME");
		 std::string LastName = Validator::ReadString();

		 _Message("EMAIL");
		 std::string Email = Validator::ReadString();

		 std::string Phone = Validator::ReadPhoneNumber();

		 std::string PinCode = Hasher::GetHash(Validator::ReadPincode());

		 _Message("ACCOUNT BALANCE");
		 double Balance = Validator::returnNumber("Invalid Number, Enter again");

		 return Client(FirstName, LastName, Email, Phone, AccountNumber, PinCode, Balance, Client::ObjectMode::newMode);
	 }
	 static Client ReadClient(const std::string& ExistingAccountNumber)
	 {

		 _Message("FIRST NAME");
		 std::string FirstName = Validator::ReadString();

		 _Message("LAST NAME");
		 std::string LastName = Validator::ReadString();

		 _Message("EMAIL");
		 std::string Email = Validator::ReadString();

		
		 std::string Phone = Validator::ReadPhoneNumber();

		 std::string PinCode = Hasher::GetHash(Validator::ReadPincode());

		 _Message("ACCOUNT BALANCE");
		 double Balance = Validator::returnNumber("Invalid Number, Enter again");

		 return Client(FirstName, LastName, Email, Phone, ExistingAccountNumber, PinCode, Balance, Client::ObjectMode::newMode);
	 }
	 Client ReadClient() {

		 std::string AccountNumber = _ReadAccountNumber();

		 _Message("FIRST NAME");
		 std::string FirstName = Validator::ReadString();

		 _Message("LAST NAME");
		 std::string LastName = Validator::ReadString();

		 _Message("EMAIL");
		 std::string Email = Validator::ReadString();

		 
		 std::string Phone = Validator::ReadPhoneNumber();

		
		 std::string PinCode = Hasher::GetHash(Validator::ReadPincode());

		 _Message("ACCOUNT BALANCE");
		 double Balance = Validator::returnNumber("Invalid Number, Enter again");

		 return Client(FirstName, LastName, Email, Phone, AccountNumber, PinCode, Balance, Client::ObjectMode::newMode);
	 }

	static Client UpdateExistingClient(const Client &target, const std::string& ExistingAccountNumber) {

		

	
		std::string FirstName;

		std::string LastName;

		std::string email;

		std::string phone;
		
		std::string pincode;

	

		if (Validator::GetConfirmation("\nDo you want to change your First name ? : "))
		{


			_Message("NEW FIRST NAME");
			FirstName = Validator::ReadNames("First");
		}
		else
		{
			FirstName = target.GetFirstName();
		}
		if (Validator::GetConfirmation("\nDo you want to change your last name ? : "))
		{


			_Message("NEW LAST NAME");
			LastName = Validator::ReadNames("Last");
		}
		else
		{
			LastName = target.GetLastName();
		}
		if (Validator::GetConfirmation("\nDo you want to change your email ? : "))
		{


			_Message("NEW EMAIL");
			email = Validator::ReadEmails();
		}
		else
		{
			email = target.GetEmail();
		}

		if (Validator::GetConfirmation("\nDo you want to change your phone number ? : "))
		{


			_Message("NEW PHONE NUMBER");
			phone = Validator::ReadPhoneNumber();
		}
		else
		{
			phone = target.GetPhoneNumber();
		}


		if (Validator::GetConfirmation("\nDo you want to change your pin code? : "))
		{


			_Message("NEW PINCODE");
			pincode = Hasher::GetHash(Validator::ReadPincode());
		}
		else
		{
			pincode = target.getPinCode();
		}

	
		 return Client(FirstName, LastName, email, phone, ExistingAccountNumber, pincode, target.getBalance(), Client::ObjectMode::newMode);
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




