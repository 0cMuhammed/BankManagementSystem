#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Client.h"

#include "UI.h"
#include "Validator.h"
#include "ClientRepository.h"
#include "MainMenuScreen.h"

class AddScreen : public MainMenuScreen 
{
private :

    ClientRepository& m_RepositoryReference;


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
   

  
    
   

  
    void _Add(Client& New) {
        switch (m_RepositoryReference.AddClient(New))
        {

        case ClientRepository::Successful:
        {
            ClientRepository::PrintClient(New);
            std::cout << "Account is saved successfuly!\n";

            break;

        }
        case ClientRepository::AccountNumberAlreadyExists:
        {
            std::cout << "Account Number is Already Used.\n";
            break;

        }
        case ClientRepository::Failed: // for some reason....
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

        Client New = ClientRepository::ReadClient();
        _Add(New);
    }

   

    public :

        AddScreen(Service& Ref) : MainMenuScreen(Ref), m_RepositoryReference(Ref.AccessClientServices().AccessRepository()) {};

    void Start() override {
        PerformMenu();
        _GetBackToMenu();
    }
 	

};

