#pragma once
#include <iostream>
#include<vector>
#include <string>
#include "Validator.h"

#include "Session.h"
#include "ListUserScreen.h"
#include "AddUserScreen.h"
#include "DeleteUserScreen.h"
#include "UpdateUserScreen.h"
#include "FindUserScreen.h"
#include "Screen.h"

class ManageUsersScreen : public Screen
{
public :

    enum MenuComponents { List = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Exit = 6};

private :

    Session& m_Session;
    Service& m_ServicesRef;

    int m_LastIndex = 0;   // NEW: remembers the highlighted row

    void _PrintLayout() {

        _ClearScreen();
        term::Header("MANAGE USERS", "", m_Session.GetUser().GetUsername());   // NEW: replaces PrintHeader override + hand-printed menu box
        term::Section("MENU", std::string(term::g::Up) + term::g::Down + " move  " + term::g::Mid + "  Enter open  " + term::g::Mid + "  Esc back");
    }

    MenuComponents _NavigateUser()
    {
        // NEW: row order = enum order (row 1 = List ... row 6 = Exit)
        static const std::vector<term::MenuItem> items = {
            { "LIST USERS",   "every staff account" },
            { "ADD USER",     "create a staff account" },
            { "DELETE USER",  "remove a staff account" },
            { "UPDATE USER",  "edit details and permissions" },
            { "FIND USER",    "look up one account" },
            { "BACK",         "return to the main menu" }
        };

        int choice = term::Menu(items, m_LastIndex);

        if (choice < 0) return MenuComponents::Exit;           // Esc = go back

        m_LastIndex = choice;
        return static_cast<MenuComponents>(choice + 1);
    }

    void _Menu(bool &isInMainMenu) {

        do
        {
            _PrintLayout();

            const MenuComponents choice = _NavigateUser();

            if (choice != MenuComponents::Exit) _ClearScreen();   // every screen starts on a clean black page

            switch (choice)
            {
            case MenuComponents::List:
            {
                ListUserScreen List(m_ServicesRef);
                List.Start(m_Session.GetUser());
                break;

            }
            case MenuComponents::Add:
            {
                AddUserScreen Add(m_ServicesRef);
                Add.Start(m_Session.GetUser());
                break;
            }
            case MenuComponents::Delete:
            {
                DeleteUserScreen Delete(m_ServicesRef);
                Delete.Start(m_Session.GetUser());
                break;
            }
            case MenuComponents::Update:
            {
                UpdateUserScreen Update(m_ServicesRef);
                Update.Start(m_Session.GetUser());
                break;
            }
            case MenuComponents::Find:
            {
                FindUserScreen Find(m_ServicesRef);
                Find.Start(m_Session.GetUser());
                break;
            }
            case MenuComponents::Exit:
            {
                isInMainMenu = false;     // NEW: leave right away (the old "Getting Back..." text is gone)
                break;

            }
            default: //for later enum choices
            {
                break;
            }
            }

        } while (isInMainMenu);
    }

    void PerformMenu(const User& CurrentUser,const char* Message = nullptr) override {

        bool isInMainMenu = true;

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::ManageUsers))
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::ManageUsers, Logger::Level::INFO);
            _Menu(isInMainMenu);
        }
        else
        {
            _Header(CurrentUser, "MANAGE USERS");
            NoAccessMsg();
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Managing Users");
            _GetBackToMenu();     // NEW: only wait when there is a message to read (the menu itself no longer needs a wait)
        }

    }

public:

    ManageUsersScreen(Service& Ref, Session &CurrentUser) : Screen(Ref), m_Session(CurrentUser), m_ServicesRef(Ref) {};

    void Start(const User & CurrentUser) override
    {
        PerformMenu(CurrentUser);

    }
};
