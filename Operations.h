#pragma once
#include <iostream>
#include<vector>
#include<fstream>
#include "Client.h"


class Operations
{
public : 

	enum  OperationStates { NotConfirmed = 0, Failed = 1, Successful = 2, AccountNumberAlreadyExists = 3, AccountNumberNotFound = 4 };

private :
	
	std::vector<Client> m_List;

	void _UpdateVector() {
		m_List.clear();
		m_List = FileHandler::LoadFile();
	}

	void _AddInVector(const Client& client) {
		m_List.push_back(client);
	}

	 Client _GetEmptyObject() const noexcept  {
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

	 Client  _FindObject(const std::string& accountNumber, const char* pinCodeParameter = nullptr) const  {



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

		 Operations(std::vector<Client>& List) : m_List(List) {};


		 bool IsExists(const std::string& accountNumber, const char* pinCode = nullptr)  {

			Client client = _FindObject(accountNumber, pinCode);
			return (!client.isEmpty());
		}
		 Client Find(const std::string& accountNumber, const char* pinCode = nullptr)  {
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
		 OperationStates DeleteClient(const std::string& accountNumber) {

			if (!IsExists(accountNumber))
			{
				return OperationStates::AccountNumberNotFound;
			}

			if (_DeleteObject(accountNumber))
			{
				return OperationStates::Failed;

			}



			FileHandler::SaveFile(m_List);
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

			FileHandler::SaveFile(m_List);

			return OperationStates::Successful;

		}
	     OperationStates UpdateClient(Client& ExistingObject) {



			if (!_UpdateObject(ExistingObject))
			{
				return OperationStates::Failed;

			}

			FileHandler::SaveFile(m_List);

			return OperationStates::Successful;

		}
		 OperationStates DeleteClient(const Client& ExistingObject) {


			if (!_DeleteObject(ExistingObject))
			{
				return OperationStates::Failed;

			}



			FileHandler::SaveFile(m_List);
			_UpdateVector();

			return OperationStates::Successful;

		}



};

