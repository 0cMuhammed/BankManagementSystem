#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include "Validator.h"

#include "Screen.h"


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
    void PerformMenu(const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        do
        {
            _PerformUpdate();

            IsContinueOperation = Validator::GetConfirmation('\n' + std::string ( ((Message != nullptr) ? Message : "Do you want to continue this operation?")));

        } while (IsContinueOperation);

    }




    //exclusive
    static bool _PerformConfirmation(const User& user, const char* Message = nullptr) {
        UserRepository::PrintUser(user);
        bool isConfirm = Validator::GetConfirmation('\n' + std::string ((((Message != nullptr) ? Message : "Are you sure you want to update this user?"))));
        return isConfirm;
    }
    void _PrintUpdateStatus(User& target,  const std::string& ExistingUsername, const User &CurrentUser) {

        target = UserRepository::ReadUser(CurrentUser,ExistingUsername);

        switch (m_RepositoryReference.UpdateUser(target))
        {

        case UserState::Failed: // for some reason....
        {
            std::cout << "\nOperation Failed, Try again Later...\n";
            break;

        }
        case UserState::Successful:
        {
            std::cout << "\nUser is Updated Successfully!\n";
            break;

        }

        default:
        {
            break;

        }

        }

    }

    void _Update(const std::string& Username, const std::string &Password, const User& CurrentUser) {

        User user = m_RepositoryReference.Find(Username,Password.c_str());


        if (user.isEmpty())
        {
            std::cout << "User is not found.\n";

        }
        else
        {
            (_PerformConfirmation(user)) ? _PrintUpdateStatus(user, Username,CurrentUser) : _Message("\nOperation is cancelled.\n");
        }

    }

    void _PerformUpdate() {
        _ClearScreen();
        PrintHeader();

        _Message("Please enter your username : ");
        std::string Username = Validator::ReadString();

        _Message("Please enter your password : ");
        std::string Password = Validator::ReadString();

        _Update(Username,Password);

    }



public:

    UpdateUserScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

    void Start() override {
        PerformMenu();
        _GetBackToMenu("Press Enter to go back to Manage Users Menu");
    }
};

