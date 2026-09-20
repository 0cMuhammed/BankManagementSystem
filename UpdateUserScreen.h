#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include "Validator.h"

#include "Screen.h"
#include "Session.h"

class UpdateUserScreen : public Screen
{
private:

    UserRepository& m_RepositoryReference;


     void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Update User Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";
    }

   

    void PerformMenu(User &CurrentUser, const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        do
        {
            _PerformUpdate(CurrentUser);

            if (!CurrentUser.isEmpty()) 
            { 
                IsContinueOperation = Validator::GetConfirmation('\n' + std::string(((Message != nullptr) ? Message : "Do you want to continue this operation?")));
            } 
            else 
            {
                IsContinueOperation = false;
            }

        } while (IsContinueOperation);

    }



    static bool _PerformConfirmation(const User& user, const char* Message = nullptr) {

        UserRepository::PrintUser(user);

        bool isConfirm = Validator::GetConfirmation('\n' + std::string ((((Message != nullptr) ? Message : "Are you sure you want to update this user?"))));

        return isConfirm;

    }


    void _PrintUpdateStatus(User& CurrentUser, User& target, const std::string& ExistingUsername) {

        if (m_RepositoryReference.IsAdmin(target))
        {
            UserIsAdminMsg("Update");

            return;
        }
      


        User TargetCopy = target;

     

        target = m_RepositoryReference.UpdateExistingUser(TargetCopy,ExistingUsername, static_cast<int32_t>(Authorizer::ReadPermissions(CurrentUser) ) );

        switch (m_RepositoryReference.UpdateUser(CurrentUser,TargetCopy, target))
        {

        case UserState::Failed: // for some reason....
        {
            std::cout << "\nOperation Failed, Try again Later...\n";
            break;

        }
        case UserState::Successful :
        {
            std::cout << "\nUser is Updated Successfully!\n";
            break;

        }
        case UserState::UserIsAdmin : 
        {
            UserIsAdminMsg("Update");
            break;
        }
        case UserState::SuccessfulSelfUpdate:
        {
            SelfEditMsg("Updated");
            break;
        }

        default:
        {
            break;

        }

        }

    }

    void _Update(User& CurrentUser,const std::string& Username, const std::string &Password) {

        User user = m_RepositoryReference.Find(Username);


        if (user.isEmpty())
        {
            std::cout << "User is not found.\n";

        }
        else
        {
            (_PerformConfirmation(user)) ? _PrintUpdateStatus(CurrentUser, user, Username) : _Message("\nOperation is cancelled.\n");
        }

    }

    void _PerformUpdate(User& CurrentUser) {

        _ClearScreen();
        PrintHeader();


        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::UpdateUser))
        {
            _Message("Please enter your username : ");
            std::string Username = Validator::ReadString();

            _Message("Please enter your password : ");
            std::string Password = Validator::ReadString();

            _Update(CurrentUser, Username, Password);
        }
        else 
        {
            NoAccessMsg();
        }


    }



public:

    UpdateUserScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

    void Start(User & CurrentUser) override {
        PerformMenu(CurrentUser);
        _GetBackToMenu("Press Enter to go back to Manage Users Menu");
    }
};

