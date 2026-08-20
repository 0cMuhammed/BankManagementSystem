#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Client.h"
#include "Validator.h"
#include "Operations.h"
#include "UI.h"


class UpdateScreen : public MainMenuScreen
{
private:
    Repository& m_RepositoryReference;

    //universial
    void _ClearScreen() override {
        system("cls");
    }
    void _Message(const char* Message) override {
        std::cout << '\n' + Message;
    }
    void _GetBackToMenu(const char* Message = nullptr) override {
        std::cout << '\n' + (((Message != nullptr) ? Message : "Press Enter to go back to Main Menu")); std::cout << ".....\n";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Delete Client Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";
    }
    void PerformMenu(const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        do
        {


            _PerformUpdate();

            IsContinueOperation = Validator::GetConfirmation('\n' + ( (Message != nullptr) ? Message : "Do you want to continue this operation?") );

        } while (IsContinueOperation);

    }
   

 

    //exclusive
    static bool _PerformConfirmation(const Client& client, const char* Message = nullptr) {
        Operations::PrintClient(client);
        bool isConfirm = Validator::GetConfirmation('\n' + (((Message != nullptr) ? Message : "Are you sure you want to update this client?")));
        return isConfirm;
    }
    void _PrintUpdateStatus(Client& client, const std::string &ExistingAccountNumber) {

        client = Operations::ReadClient(ExistingAccountNumber);

        switch (m_RepositoryReference.OperationsSection.UpdateClient(client))
        {

        case Operations::Failed: // for some reason....
        {
            std::cout << "Operation Failed, Try again Later...\n";
            break;

        }
        case Operations::Successful:
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

        Client client = m_RepositoryReference.OperationsSection.Find(AccountNumber);


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
   


public:

    UpdateScreen(Repository& Repo) : MainMenuScreen(Repo), m_RepositoryReference(Repo) {};

    void Start() override {
        PerformMenu();
        _GetBackToMenu();
    }
};

