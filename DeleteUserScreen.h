#pragma once
#include <iostream>
#include<vector>
#include<iomanip>


#include "UserServices.h"
#include "Validator.h"


#include "UI.h"

class DeleteUserScreen : public ManageUsersScreen
{
private:
    UserRepository& m_RepositoryReference;

    //universal 
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

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Delete User Screen"));

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





    static bool _PerformConfirmation(const User& user, const char* Message = nullptr) {
        UserRepository::PrintUser(user);
        bool isConfirm = Validator::GetConfirmation('\n' + (((Message != nullptr) ? Message : "Are you sure you want to delete this user?")));
        return isConfirm;
    }
    void _PrintDeleteStatus(User& user) {

        switch (m_RepositoryReference.DeleteUser(user))
        {

        case UserState::Failed: // for some reason....
        {
            std::cout << "Operation Failed, Try again Later...\n";
            break;

        }
        case UserState::Successful:
        {
            std::cout << "user is deleted Successfully!\n";
            break;

        }

        default:
        {
            break;

        }

        }
    }

    void _Delete(const std::string& Username, const std::string &Password) {

        User user = m_RepositoryReference.Find(Username,Password.c_str());


        if (user.isEmpty())
        {
            std::cout << "User is not found.\n";

        }
        else
        {
            ( _PerformConfirmation(user) ) ? _PrintDeleteStatus(user) : _Message("Operation is cancelled");
        }

    }
    void _PerformDelete() {
        _ClearScreen();
        PrintHeader();

        _Message("Please enter your username");
        std::string username = Validator::ReadString();

        _Message("Please enter your password");
        std::string password = Validator::ReadString();

        _Delete(username,password);

    }




public:

    DeleteUserScreen(Service& Ref) : ManageUsersScreen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

    void Start() override {
        PerformMenu();
        _GetBackToMenu();
    }
};

