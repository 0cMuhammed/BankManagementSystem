#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Client.h"
#include "Repository.h"
#include "Validator.h"
#include "UI.h"

class UpdateScreen : public UI
{
private:
    Repository& m_RepositoryReference;

    static void _ClearScreen() {
        system("cls");
    }
    static void _PrintClient(const Client& client) {

        std::cout << "\nClient Card:";
        std::cout << "\n___________________";
        std::cout << "\nFirstName   : " << client.getFirstName();
        std::cout << "\nLastName    : " << client.getLastName();
        std::cout << "\nFull Name   : " << client.getFullName();
        std::cout << "\nEmail       : " << client.getEmail();
        std::cout << "\nPhone       : " << client.getPhoneNumber();
        std::cout << "\nAcc. Number : " << client.getAccountNumber();
        std::cout << "\nPassword    : " << client.getPinCode();
        std::cout << "\nBalance     : " << client.getBalance();
        std::cout << "\n___________________\n";


    }
    static Client _ReadClient(const std::string &ExistingAccountNumber)
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
    void PrintHeader(const std::string* ScreenName = nullptr, const std::string* SubTitle = nullptr) override {
        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? *ScreenName : "Delete Client Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << *SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";
    }

    static void _Message(const std::string& Message) {
        std::cout << "\n" + Message;
    }

    static bool _PerformConfirmation(const Client& client, const std::string* Message = nullptr) {
        _PrintClient(client);
        bool isConfirm = Validator::GetConfirmation("\n" + (((Message != nullptr) ? *Message : "Are you sure you want to update this client?")));
        return isConfirm;
    }

    void _PrintUpdateStatus(Client& client, const std::string &ExistingAccountNumber) {

        client = _ReadClient(ExistingAccountNumber);

        switch (m_RepositoryReference.UpdateClient(client))
        {

        case Repository::Failed: // for some reason....
        {
            std::cout << "Operation Failed, Try again Later...\n";
            break;

        }
        case Repository::Successful:
        {
            std::cout << "Account is Updated Successfully!\n";
            break;

        }

        default:
        {
            break;

        }

        }

    }

    void _Update(const std::string& AccountNumber) {

        Client client = m_RepositoryReference.Find(AccountNumber);


        if (client.isEmpty())
        {
            std::cout << "Account Number is not found.\n";

        }
        else
        {
            (_PerformConfirmation(client)) ? _PrintUpdateStatus(client,AccountNumber) : _Message("Operation is cancelled.");
        }

    }



    void _PerformUpdate() {
        _ClearScreen();
        PrintHeader();

        _Message("Please enter your account number");
        std::string AccountNumber = Validator::ReadString();

        _Update(AccountNumber);

    }
    void PerformMenu(const std::string* Message = nullptr) override {

        bool IsContinueOperation = true;

        do
        {


            _PerformUpdate();

            IsContinueOperation = Validator::GetConfirmation("\n" + (((Message != nullptr) ? *Message : "Do you want to continue this operation?")));

        } while (IsContinueOperation);

    }


public:

     UpdateScreen(Repository& Repo) : m_RepositoryReference(Repo) {};

    void Show() override {
        PerformMenu();
    }
};

