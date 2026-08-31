#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Validator.h"

#include "Screen.h"




class AddUserScreen : public Screen
{
private :

    UserRepository & m_RepositoryReference;


    void _Message(const std::string & Message) override {
        std::cout << '\n' + Message + " : ";
    }

    void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {


        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Add New User Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";
    }
    void PerformMenu(const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        do {
            _PerformAdding();
            IsContinueOperation = Validator::GetConfirmation('\n' + std::string ( (((Message != nullptr) ? Message : "Do you want to continue this operation?"))));

        } while (IsContinueOperation);

    }







    void _Add(User& New) {
        switch (m_RepositoryReference.AddUser(New))
        {

        case UserState::Successful:
        {
            UserRepository::PrintUser(New);
            std::cout << "\nAccount is saved successfuly!\n";

            break;

        }
        case UserState::UserAlreadyExists:
        {
            std::cout << "\nUser Already Exists.\n";
            break;

        }
        case UserState::Failed: // for some reason....
        {
            std::cout << "\nOperation Failed, Try again Later...\n";
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

        User New = m_RepositoryReference.ReadUser();
        _Add(New);
    }



public:

    AddUserScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

    void Start() override {
        PerformMenu();
        _GetBackToMenu("Press Enter to go back to Manage Users Menu");
    }
};

