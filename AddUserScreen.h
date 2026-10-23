#pragma once
#include <iostream>
#include<vector>

#include "Validator.h"
#include "UserRepository.h"
#include "Screen.h"

class AddUserScreen : public Screen
{
private :

    UserRepository & m_RepositoryReference;

    void PerformMenu(const User & CurrentUser,const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        while (IsContinueOperation)
        {
            if (!_PerformAdding(CurrentUser))
            {
                _GetBackToMenu("Press any key to go back to the Manage Users menu");
                return;
            }

            term::Blank();
            IsContinueOperation = term::Confirm((Message != nullptr) ? Message : "Add another user?");
        }

    }

    void _Add(const User& CurrentUser, User& New) {

        switch (m_RepositoryReference.AddUser(New))
        {

        case UserState::Successful:
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::AddUser, Logger::Level::INFO, "", New.GetUsername());
            _PrintUserCard(New);                               // NEW: replaces UserRepository::PrintUser
            term::Success("User saved successfully.");

            break;

        }
        case UserState::UserAlreadyExists:
        {
            term::Error("User already exists.");
            break;

        }
        case UserState::Failed: // for some reason....
        {
            term::Error("Operation failed, try again later.");
            break;

        }

        default:
        {
            break;

        }
        }
    }
    bool _PerformAdding(const User& CurrentUser) {

        _ClearScreen();
        _Header(CurrentUser, "ADD USER");

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::AddUser))
        {
            User New = m_RepositoryReference.ReadUser(static_cast<int32_t>(Authorizer::ReadPermissions(CurrentUser)));
            _Add(CurrentUser, New);
            return true;
        }
        else
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Add User");
            NoAccessMsg();
            return false;
        }

    }

public:

    AddUserScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

    void Start(const User & CurrentUser) override {
        PerformMenu(CurrentUser);
    }
};
