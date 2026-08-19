#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Client.h"
#include "Repository.h"
#include "Validator.h"
#include "UI.h"
#include "MainMenuScreen.h"

class AddScreen : public MainMenuScreen 
{
private :

    Repository& m_RepositoryReference;


    //universal
    void _ClearScreen() override {
        system("cls");
    }

    void _Message(const char* Message) override {
        std::cout << '\n' + Message + ': ';
    }
     void _GetBackToMenu(const char* Message = nullptr) override {
        std::cout << '\n' + (((Message != nullptr) ? Message : "Press Enter to go back to Main Menu")); std::cout << ".....\n";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }


    void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {


        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Add New Client Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";
    }
    void PerformMenu(const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        do {
            _PerformAdding();
            IsContinueOperation = Validator::GetConfirmation('\n' + (((Message != nullptr) ? Message : "Do you want to continue this operation?")));

        } while (IsContinueOperation);

    }
   

    //exclusive to Operations Class to be added later
   static void _PrintClient(const Client &client) {
      
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
   Client _ReadClient()
    {
        _Message("Enter your account number");
        std::string AccountNumber = Validator::ReadString();

        _Message("Enter first name");
        std::string FirstName = Validator::ReadString();

        _Message("Enter last name");
        std::string LastName = Validator::ReadString();

        _Message("Enter Email");
        std::string Email  = Validator::ReadString();

        _Message("Enter Phone");
        std::string Phone = Validator::ReadString();

        _Message("Enter PinCode");
       std::string PinCode = Validator::ReadString();

        _Message("Enter Account Balance");
        double Balance = Validator::returnNumber("Invalid Number, Enter again");

        return Client(FirstName, LastName, Email, Phone, AccountNumber, PinCode, Balance, Client::ObjectMode::newMode);
    }
    
   

  
    void _Add(Client& New) {
        switch (m_RepositoryReference.AddClient(New))
        {

        case Repository::Successful:
        {
            _PrintClient(New);
            std::cout << "Account is saved successfuly!\n";

            break;

        }
        case Repository::AccountNumberAlreadyExists:
        {
            std::cout << "Account Number is Already Used.\n";
            break;

        }
        case Repository::Failed: // for some reason....
        {
            std::cout << "Operation Failed, Try again Later...\n";
            break;

        }

        default:
        {
            break;

        }
        }
    }
    void _PerformAdding() {

        _ClearScreen();
        PrintHeader();

        Client New = _ReadClient();
        _Add(New);
    }

   

    public :
        AddScreen(Repository& Repo) : MainMenuScreen(Repo), m_RepositoryReference(Repo) {};

    void Show() override {
        PerformMenu();
        _GetBackToMenu();
    }
 	

};

