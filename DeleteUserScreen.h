#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include "Validator.h"
#include "Screen.h"

class DeleteUserScreen : public Screen
{
private:

    UserRepository& m_RepositoryReference;

    void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Delete User Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";
    }

    void PerformMenu(User& CurrentUser,const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        do
        {
            _PerformDelete(CurrentUser);

            if (!CurrentUser.isEmpty()) 
            { 
                IsContinueOperation = Validator::GetConfirmation('\n' + std::string((((Message != nullptr) ? Message : "Do you want to continue this operation?")))); 
            }

            else 
            {
                IsContinueOperation = false;

            }

        } while (IsContinueOperation);

    }





    static bool _PerformConfirmation(const User& user, const char* Message = nullptr) {
        UserRepository::PrintUser(user);
        bool isConfirm = Validator::GetConfirmation('\n' + std::string ( (((Message != nullptr) ? Message : "Are you sure you want to delete this user?"))));
        return isConfirm;
    }
    void _PrintDeleteStatus(User &CurrentUser, const User& target) {

    
        if (m_RepositoryReference.IsAdmin(target))
        {
            UserIsAdminMsg("Delete");

            return;
        }

        switch (m_RepositoryReference.DeleteUser(CurrentUser,target))
        {

        case UserState::Failed: // for some reason....
        {
            std::cout << "Operation Failed, Try again Later...\n";
            break;

        }
        case UserState::Successful:
        {
            std::cout << "\nUser is deleted Successfully!\n";
            break;
            
        }
        case UserState::UserIsAdmin:
        {
            UserIsAdminMsg("Delete");
            break;
        }
        case UserState::SuccessfulSelfDelete:
        {
            SelfEditMsg("deleted");
            break;
        }

        default:
        {
            break;

        }

        }
    }

    void _Delete(User &CurrentUser,const std::string& Username, const std::string &Password) {

        User user = m_RepositoryReference.Find(Username);


        if (user.isEmpty())
        {
            std::cout << "\nUser is not found.\n";

        }
        else
        {
            ( _PerformConfirmation(user) ) ? _PrintDeleteStatus(CurrentUser, user) : _Message("Operation is cancelled");
        }


    }
    void _PerformDelete(User &CurrentUser) {

        _ClearScreen();
        PrintHeader();


        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::DeleteUser)) 
        {
            _Message("Please enter your username : ");
            std::string username = Validator::ReadString();

            _Message("Please enter your password : ");
            std::string password = Validator::ReadString();
     
            _Delete(CurrentUser,username, password);

        }
        else 
        {
            NoAccessMsg();
        }

    }




public:

    DeleteUserScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

    void Start(User &CurrentUser) override {
         PerformMenu(CurrentUser);
        _GetBackToMenu("Press Enter to go back to Manage Users Menu");
    }
};

