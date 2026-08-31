#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Client.h"
#include "Validator.h"
#include "ClientRepository.h"
#include "Screen.h"


class UpdateScreen : public Screen
{
private:
    ClientRepository& m_RepositoryReference;


    void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Update Client Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";
    }
    void PerformMenu(const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        do
        {


            _PerformUpdate();

            IsContinueOperation = Validator::GetConfirmation('\n' + std::string( ( (Message != nullptr) ? Message : "Do you want to continue this operation?") ));

        } while (IsContinueOperation);

    }
   

 

    //exclusive
    static bool _PerformConfirmation(const Client& client, const char* Message = nullptr) {
        ClientRepository::PrintClient(client);
        bool isConfirm = Validator::GetConfirmation('\n' + std::string ((((Message != nullptr) ? Message : "Are you sure you want to update this client?"))));
        return isConfirm;
    }
    void _PrintUpdateStatus(Client& client, const std::string &ExistingAccountNumber) {

        client = ClientRepository::ReadClient(ExistingAccountNumber);

        switch (m_RepositoryReference.UpdateClient(client))
        {

        case ClientRepository::OperationStates::Failed: // for some reason....
        {
            std::cout << "\nOperation Failed, Try again Later...\n";
            break;

        }
        case ClientRepository::OperationStates::Successful:
        {
            std::cout << "\nAccount is Updated Successfully!\n";
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

        _Message("Please enter your account number : ");
        std::string AccountNumber = Validator::ReadString();

        _Update(AccountNumber);

    }
   


public:

    UpdateScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessClientServices().AccessRepository()) {};

    void Start() override {
        PerformMenu();
        _GetBackToMenu();
    }
};

