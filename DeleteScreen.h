#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Client.h"
#include "Repository.h"
#include "Validator.h"
#include "UI.h"

class DeleteScreen : public UI
{
private :
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
	void PrintHeader(const std::string* ScreenName = nullptr, const std::string* SubTitle = nullptr) override {
		std::cout << "\t\t\t\t\t______________________________________";

		std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? *ScreenName : "Delete Client Screen"));

		if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << *SubTitle; }

		std::cout << "\n\t\t\t\t\t______________________________________\n\n";
	}
    static void _Message(const std::string& Message) {
        std::cout << "\n" + Message;
    }
    static bool _PerformConfirmation(const Client &client, const std::string* Message = nullptr) {
        _PrintClient(client);
        bool isConfirm =  Validator::GetConfirmation("\n" + (((Message != nullptr) ? *Message : "Are you sure you want to delete this client?")));
        return isConfirm;
    }
    void _PrintDeleteStatus(Client& client) {
        switch (m_RepositoryReference.DeleteClient(client))
        {

        case Repository::Failed: // for some reason....
        {
            std::cout << "Operation Failed, Try again Later...\n";
            break;

        }
        case Repository::Successful:
        {
            std::cout << "Account is deleted Successfully!\n";
            break;

        }

        default:
        {
            break;

        }

        }
    }

    void _Delete(const std::string& AccountNumber) {

        Client client = m_RepositoryReference.Find(AccountNumber);
         

        if (client.isEmpty())
        {
            std::cout << "Account Number is not found.\n";

        }
        else 
        {
            (_PerformConfirmation(client)) ? _PrintDeleteStatus(client) : _Message("Operation is cancelled");
        }

    }
    void _PerformDelete() {
        _ClearScreen();
        PrintHeader();

        _Message("Please enter your account number");
        std::string AccountNumber = Validator::ReadString();

        _Delete(AccountNumber);
            
    }
	void PerformMenu(const std::string* Message = nullptr) override {

		bool IsContinueOperation = true;

		do 
        {
            
			
            _PerformDelete();

            IsContinueOperation = Validator::GetConfirmation("\n" + (((Message != nullptr) ? *Message : "Do you want to continue this operation?")));

		} while (IsContinueOperation);

	}


    public :

         DeleteScreen(Repository& Repo) : m_RepositoryReference(Repo) {};

        void Show() override {
            PerformMenu();
        }

};

