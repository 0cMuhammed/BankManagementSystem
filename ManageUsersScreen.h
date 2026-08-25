#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include <string>
#include "User.h"
#include "Validator.h"


#include "ListUserScreen.h"
#include "AddUserScreen.h"
#include "DeleteUserScreen.h"
#include "UpdateUserScreen.h"
#include "FindUserScreen.h"

#include "UI.h"

class ManageUsersScreen : public MainMenuScreen
{
    enum MenuComponents { List = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Exit = 6};

    Service& m_ServicesRef;


    void _GetBackToMenu(const char* Message = nullptr) override {
        std::cout << '\n' + (((Message != nullptr) ? Message : "Press Enter to go back to Main Menu")); std::cout << ".....\n";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
    void _ClearScreen() override {
        system("cls");
    }
    void _Message(const char* Message = nullptr) override {
        std::cout << '\n' + Message;
    }
    void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {

        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Manage Users Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";

    }

    void _PrintLayout() {

        _ClearScreen();
        PrintHeader();

        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t\t\tManage Users Menue\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t[1] List Users.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[2] Add New User.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[3] Delete User.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[4] Update User.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[5] Find User.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[6] Main Menu.\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
    }
    static void _ExitMenu(bool& isInMainMenu, const char* message = "\nGetting Back to Main Menu...")
    {
        std::cout << message << "\n\n";

        isInMainMenu = false;

    }

    MenuComponents _NavigateUser(double from = 1, double to = 6)
    {
        _Message("Choose What do you want to do ? [1 to 6] : ");

        return  (MenuComponents)Validator::returnValidatedNumber(from, to);
    }

    void PerformMenu(const char* Message = nullptr) override {

        bool isInMainMenu = true;
        do
        {
            _PrintLayout();

            switch (_NavigateUser())
            {
            case MenuComponents::List:
            {
                ListUserScreen List(m_ServicesRef);
                List.Start();
                break;

            }
            case MenuComponents::Add:
            {
                AddUserScreen Add(m_ServicesRef);
                Add.Start();
                break;
            }
            case MenuComponents::Delete:
            {
                DeleteUserScreen Delete(m_ServicesRef);
                Delete.Start();
                break;
            }
            case MenuComponents::Update:
            {
                UpdateUserScreen Update(m_ServicesRef);
                Update.Start();
                break;
            }
            case MenuComponents::Find:
            {
                FindUserScreen Find(m_ServicesRef);
                Find.Start();
                break;
            }
            case MenuComponents::Exit:
            {
                _GetBackToMenu();
                _ExitMenu(isInMainMenu, "\nGetting Back to Main Menu....");
                break;

            }
             default: //for later enuchoice
            {
                break;
            }
            }

        } while (isInMainMenu);

    }

public:

    ManageUsersScreen(Service& Ref) : MainMenuScreen(Ref), m_ServicesRef(Ref) {};

    void Start() override {
        PerformMenu();
    }
};

