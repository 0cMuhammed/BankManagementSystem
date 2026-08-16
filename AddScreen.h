#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Client.h"
#include "Repository.h"
#include "Validator.h"
#include "UI.h"

class AddScreen : public UI 
{
private :

    Repository& m_RepositoryReference;
    

   static void _Message(const std::string& Message) {
        std::cout << "\n" + Message + ": ";
    }
   static std::string _ReadAccountNumber() {
       _Message("Enter your account number");
       return Validator::ReadString();
   }
    static Client _ReadClient(Client& client)
    {
        _Message("Enter first name");
        client.setFirstName(Validator::ReadString());

        _Message("Enter last name");
        client.setLastName(Validator::ReadString());

        _Message("Enter Email");
        client.setEmail(Validator::ReadString());

        _Message("Enter Phone");
         client.setPhoneNumber(Validator::ReadString());

         _Message("Enter PinCode");
         client.setPinCode(Validator::ReadString());

         _Message("Enter Account Balance");
         client.setBalance(Validator::returnNumber("Invalid Number, Enter again"));

         return client;
    }
    static Client _ReadClient()
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
    
    void _PrintHeader(const std::string* ScreenName = nullptr, const std::string* SubTitle = nullptr) override {


        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (ScreenName != nullptr) ? *ScreenName : "Add New Client Screen";

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << *SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";
    }
    void _PerformMenu(const std::string* Message = nullptr) override {

        bool IsContinueOperation = true;

        do 
        { 
            _PrintHeader();
            
            Client New = _ReadClient();

            switch (m_RepositoryReference.AddClient(New)) 
            {

            case Repository::Successful : 
            {
                std::cout << "Account Saved Successfuly!\n";
                break;
            
            }
            case Repository::AccountNumberAlreadyExists: 
            {
                std::cout << "Account Number is Already Used, Choose another one.\n";
                break;

            }
            case Repository::Failed : 
            {
                std::cout << "Operation Failed, Try again Later...\n";
                break;

            }

            default :
            {
                break;

            }


               

            }


        } while()
    }
 	

};

