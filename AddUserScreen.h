#pragma once
#include <iostream>
#include<vector>
#include<iomanip>

#include "Validator.h"
#include "UserRepository.h"
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

    void PerformMenu(const User & CurrentUser,const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        do {
            _PerformAdding(CurrentUser);
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
    void _PerformAdding(const User& CurrentUser) {

        _ClearScreen();
        PrintHeader();

      

        if (     Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::AddUser)    ) 
        {
            User New = m_RepositoryReference.ReadUser(static_cast<int32_t>(Authorizer::ReadPermissions(CurrentUser)));
            _Add(New);
        }
        else
        {
            NoAccessMsg();
        }

    }



public:

    AddUserScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

    void Start(const User & CurrentUser) override {
        PerformMenu(CurrentUser);
        _GetBackToMenu("Press Enter to go back to Manage Users Menu");
    }
};

