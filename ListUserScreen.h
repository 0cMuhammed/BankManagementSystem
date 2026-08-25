#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include <limits>

#include "UserServices.h"
#include "UI.h"


class ListUserScreen : public ManageUsersScreen
{
private :
    const UserRepository& m_RepositoryReference; 

    void _ClearScreen() override {
        system("cls");
    }
    void _GetBackToMenu(const char* Message = nullptr) override {
        std::cout << '\n' + (((Message != nullptr) ? Message : "Press Enter to go back to Main Menu")); std::cout << ".....\n";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    void _Message(const char* Message = nullptr) override {
        std::cout << '\n' + Message;
    }

    void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {

        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  " << (((ScreenName != nullptr) ? ScreenName : "User List Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";

    }
    void PerformMenu(const char* Message = nullptr) override {
        if (m_RepositoryReference.GetList().size() == 0)
        {

            _Message("\t\t\t\tNo Users Available In the System!");


        }
        else
        {
            const std::string SubTitle = "\t    (" + std::to_string(m_RepositoryReference.GetList().size()) + ") User(s).";

            PrintHeader(nullptr, SubTitle.c_str());
            _PrintLayout();
            _PrintAll(m_RepositoryReference.GetList());

        }
    }

    //exclusive
    static void _PrintFormattedUser(const User& user)
    {

        std::cout << std::setw(8) << std::left << "" << "| " << std::setw(15) << std::left << user.GetUsername(); 
        std::cout << "| " << std::setw(20) << std::left << user.GetFullName();
        std::cout << "| " << std::setw(12) << std::left << user.GetPhoneNumber();
        std::cout << "| " << std::setw(20) << std::left << user.GetEmail();
        std::cout << "| " << std::setw(10) << std::left << user.GetPassword();
        std::cout << "| " << std::setw(12) << std::left << user.GetPermissions();

    }
    static void _PrintLine() {
        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_________________________________________\n" << std::endl;
    }
    static void _PrintLayout() {

        _PrintLine();
        std::cout << std::setw(8) << std::left << "" << "| " << std::left << std::setw(15) << "Username";
        std::cout << "| " << std::left << std::setw(20) << "Full Name";
        std::cout << "| " << std::left << std::setw(12) << "Phone";
        std::cout << "| " << std::left << std::setw(20) << "Email";
        std::cout << "| " << std::left << std::setw(10) << "Password";
        std::cout << "| " << std::left << std::setw(12) << "Permissions";
        _PrintLine();
    }
    static void _PrintAll(const std::vector<User>& Users) {
        for (const  User & u : Users)
        {

            _PrintFormattedUser(u);
            std::cout << std::endl;
        }
        _PrintLine();

    }


public:


    ListUserScreen(Service& Ref) : ManageUsersScreen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

    void Start() override {



        PerformMenu();
        _GetBackToMenu();

    }
};

