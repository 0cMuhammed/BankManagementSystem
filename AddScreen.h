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
   

  
    
   

  
    void _Add(Client& New) {
        switch (m_RepositoryReference.operations.AddClient(New))
        {

        case Operations::Successful:
        {
            Operations::PrintClient(New);
            std::cout << "Account is saved successfuly!\n";

            break;

        }
        case Operations::AccountNumberAlreadyExists:
        {
            std::cout << "Account Number is Already Used.\n";
            break;

        }
        case Operations::Failed: // for some reason....
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

        Client New = Operations::ReadClient();
        _Add(New);
    }

   

    public :
        AddScreen(Repository& Repo) : MainMenuScreen(Repo), m_RepositoryReference(Repo) {};

    void Show() override {
        PerformMenu();
        _GetBackToMenu();
    }
 	

};

