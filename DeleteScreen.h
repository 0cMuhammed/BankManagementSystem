#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Client.h"
#include "Repository.h"
#include "Validator.h"
#include "Operations.h"

#include "UI.h"


class DeleteScreen : public MainMenuScreen
{
private :
    Repository& m_RepositoryReference;

    //universal 
    void _ClearScreen() override{
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


            _PerformDelete();

            IsContinueOperation = Validator::GetConfirmation('\n' + (((Message != nullptr) ? Message : "Do you want to continue this operation?")));

        } while (IsContinueOperation);

    }
   



   
    static bool _PerformConfirmation(const Client &client, const char* Message = nullptr) {
        Operations::PrintClient(client);
        bool isConfirm =  Validator::GetConfirmation('\n' + (((Message != nullptr) ? Message : "Are you sure you want to delete this client?")));
        return isConfirm;
    }
    void _PrintDeleteStatus(Client& client) {
 
        switch (m_RepositoryReference.OperationsSection.DeleteClient(client))
        {

        case Operations::Failed : // for some reason....
        {
            std::cout << "Operation Failed, Try again Later...\n";
            break;

        }
        case Operations::Successful:
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

        Client client = m_RepositoryReference.OperationsSection.Find(AccountNumber);
         

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

	


    public :

         DeleteScreen(Repository& Repo) : MainMenuScreen(Repo), m_RepositoryReference(Repo) {};

        void Start() override {
            PerformMenu();
            _GetBackToMenu();
        }

};

