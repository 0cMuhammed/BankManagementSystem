#pragma once
#include "Person.h"
#include <iostream>
#include<vector>
#include<fstream>
class Client : public Person {

private:

	enum mode {EmptyMode = 1, UpdateMode = 2, newMode = 3, DeleteMode = 4};
	enum saveStates {Failed = 1, Successful = 2, AccountNumberAlreadyExists = 3};

	std::string m_accountNumber = ""; //read only
	std::string m_pinCode = "";
	double m_balance = 0;
	mode m_mode = mode::EmptyMode;

	bool _isNotToBeSaved() const noexcept {
		return	m_mode == mode::EmptyMode || m_mode == mode::DeleteMode;
	}
	static std::vector<std::string> _splitTokensToVec(std::string line, const std::string& delimiter = "#//#")
	{
		std::string Word = "";
		size_t pos = 0;
		std::vector<std::string> vTokens;
		vTokens.reserve(7);

		while ( ( pos = line.find(delimiter) ) != std::string::npos )
		{
			Word = line.substr(0, pos);

			if (Word != "")
				vTokens.push_back(std::move(Word));


			line.erase ( 0, pos + delimiter.length() );
		}

		if (line != "")
			vTokens.push_back(std::move(line));


		return vTokens;

	}

	static Client _getLineToObject(const std::string& line) {

		std::vector<std::string> Tokens;
		Tokens.reserve(7);
		Tokens = _splitTokensToVec(line);

		if (Tokens.size() != 7)
			throw std::runtime_error("Malformed line: expected 7 fields, got " + std::to_string(Tokens.size()));


		return Client(Tokens[0], Tokens[1], Tokens[2], Tokens[3], Tokens[4], Tokens[5], stod(Tokens[6]), mode::UpdateMode);
		
	}

	static Client _getEmptyObject() noexcept {
		return Client("", "", "", "", "", "", 0, mode::EmptyMode);
	}
	static Client _getNewObject(const std::string &accountNumber) noexcept {
		return  Client("", "", "", "", accountNumber, "", 0, mode::newMode);
	}
   
	std::string _getObjectToLine(const Client& client,const std::string& delimiter = "#//#") const {
		return client.getFirstName() + delimiter + client.getLastName() + delimiter + client.getEmail() + delimiter + client.getPhoneNumber() + delimiter + client.m_accountNumber + delimiter + client.m_pinCode + delimiter + std::to_string(client.m_balance);
	}


	static std::string NumberToText(int Number)
	{

		if (Number == 0)
		{
			return "";
		}

		if (Number >= 1 && Number <= 19)
		{
			std::string arr[] = { "", "One","Two","Three","Four","Five","Six","Seven",
		"Eight","Nine","Ten","Eleven","Twelve","Thirteen","Fourteen",
		  "Fifteen","Sixteen","Seventeen","Eighteen","Nineteen" };

			return  arr[Number] + " ";

		}

		if (Number >= 20 && Number <= 99)
		{
			std::string arr[] = { "","","Twenty","Thirty","Forty","Fifty","Sixty","Seventy","Eighty","Ninety" };
			return  arr[Number / 10] + " " + NumberToText(Number % 10);
		}

		if (Number >= 100 && Number <= 199)
		{
			return  "One Hundred " + NumberToText(Number % 100);
		}

		if (Number >= 200 && Number <= 999)
		{
			return   NumberToText(Number / 100) + "Hundreds " + NumberToText(Number % 100);
		}

		if (Number >= 1000 && Number <= 1999)
		{
			return  "One Thousand " + NumberToText(Number % 1000);
		}

		if (Number >= 2000 && Number <= 999999)
		{
			return   NumberToText(Number / 1000) + "Thousands " + NumberToText(Number % 1000);
		}

		if (Number >= 1000000 && Number <= 1999999)
		{
			return  "One Million " + NumberToText(Number % 1000000);
		}

		if (Number >= 2000000 && Number <= 999999999)
		{
			return   NumberToText(Number / 1000000) + "Millions " + NumberToText(Number % 1000000);
		}

		if (Number >= 1000000000 && Number <= 1999999999)
		{
			return  "One Billion " + NumberToText(Number % 1000000000);
		}
		else
		{
			return   NumberToText(Number / 1000000000) + "Billions " + NumberToText(Number % 1000000000);
		}


	}

	static std::vector<Client> _loadFile() {
		std::fstream file;
		std::vector<Client> Clients;
		file.open("Clients.txt", std::ios::in);

		if (file.is_open()) 
		{
			std::string dataline;
			
			while ( !(std::getline(file, dataline).fail())) 
			{
				Clients.emplace_back(_getLineToObject(dataline));
			}
			file.close();
		}

		return Clients;
	}
	static long long getTotalBalance() {

		long long total = 0;
		std::vector<Client> allClients = _loadFile();

		for (const Client& client : allClients) 
		{
			total += client.m_balance;
		}

		return total;

	}

	 void _saveFile(const std::vector<Client>& Clients) {

		std::fstream File;
		File.open("Clients.txt", std::ios::out);
		if (File.is_open()) {
			for (const Client& client : Clients)
			{
				if ( ! client._isNotToBeSaved() )
					File << _getObjectToLine(client) << '\n';
			}

			File.close();
		}


	}
	 void _saveFile(const Client & client) {

		 std::fstream File;
		 File.open("Clients.txt", std::ios::out | std::ios::app);

		 if (File.is_open()) 
		 {
		      File << _getObjectToLine(client) << '\n';
		 }
		 File.close();
			
     }
	 void _UpdateVector(std::vector<Client>& Clients) {
		 Clients.clear();
		 Clients = _loadFile();
	 }
	 
	void _SaveExistingObject() {
		// use this after .update()
		std::vector<Client> Clients = _loadFile();

		for (Client& client : Clients) 
		{
			if (client.m_accountNumber == m_accountNumber) 
			{
				client = *this;
				break;

			}

		}

		_saveFile(Clients);
		
	}

	void _SaveNewObject() {
		
       m_mode = mode::UpdateMode;
	   _saveFile(*this);
		

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

	

	void _DeleteObject(const std::string& accountNumber) {
		std::vector<Client> clients = _loadFile();

		_MarkDelete(accountNumber,clients);
		_saveFile(clients); //delete in file
		_MakeEmpty();       // null it out in memory
		
		
		
	}

	static Client _find(const std::string& accountNumber, const std::string *pinCodeParameter = nullptr) {


		std::fstream file;



		file.open("Clients.txt", std::ios::in);

		if (file.is_open())
		{
			std::string line;


			while (!(std::getline(file, line).fail()))
			{

				Client client = _getLineToObject(line);
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

	void setPinCode(const std::string& newPinCode) {
		
		
			m_pinCode = newPinCode; 
		
	}

	void setBalance(double newBalance) {
		m_balance = newBalance;
	}

	static Client search(const std::string& accountNumber, const std::string &pinCode) {
		return _find(accountNumber,&pinCode);
	}
	static Client search(const std::string& accountNumber) {
		return _find(accountNumber);
	}

	static bool isClientExists(const std::string& accountNumber, const std::string & pinCode) {
		Client client = search(accountNumber,pinCode);

		return (!client.isEmpty());
	}
	static bool isClientExists(const std::string& accountNumber) {
		Client client = search(accountNumber);

		return (!client.isEmpty());
	}

	static std::vector<Client> getClientList() {
		return _loadFile();
	}

	static saveStates AddClient(const std::string& accountNumber, const std::string& newFirstName, const std::string& newLastName, const std::string& newEmail, const std::string& newPhoneNumber, const std::string& newPinCode, double newBalance)
	{
		if (isClientExists(accountNumber)) 
		{
			return saveStates::AccountNumberAlreadyExists;
		}
	   
		Client client = _getNewObject(accountNumber);
		client._SetObject(newFirstName, newLastName, newEmail, newPhoneNumber, newPinCode, newBalance);
		client.save();

	
		
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

	void save() {
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
			
		}
	


	}

	void print()
	{
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