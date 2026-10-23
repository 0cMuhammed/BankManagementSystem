#pragma once
#include <iostream>
#include<vector>
#include "Validator.h"

#include "Screen.h"
#include "Session.h"

class UpdateUserScreen : public Screen
{
private:

    UserRepository& m_RepositoryReference;

    void PerformMenu(User &CurrentUser, const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        while (IsContinueOperation)
        {
            if (!_PerformUpdate(CurrentUser))
            {
                _GetBackToMenu("Press any key to go back to the Manage Users menu");
                return;
            }

            if (CurrentUser.isEmpty())   // the user edited THEIR OWN account: show the message, then leave (same as the old flow)
            {
                _GetBackToMenu("Press any key to go back to the Manage Users menu");
                return;
            }

            term::Blank();
            IsContinueOperation = term::Confirm((Message != nullptr) ? Message : "Update another user?");
        }

    }

    static bool _PerformConfirmation(const User& user, const char* Message = nullptr) {

        _PrintUserCard(user);                                  // NEW: replaces UserRepository::PrintUser

        return term::Confirm((Message != nullptr) ? Message : "Do you want to update this user?");

    }

    void _PrintUpdateStatus(User& CurrentUser, User& target, const std::string& ExistingUsername) {

        const std::string Actor = CurrentUser.GetUsername();

        if (m_RepositoryReference.IsAdmin(target))
        {
            Logger::LogUser(Actor, Logger::Category::UpdateUser, Logger::Level::WARN, "", ExistingUsername);
            UserIsAdminMsg("Update");

            return;
        }

        User TargetCopy = target;

        target = m_RepositoryReference.UpdateExistingUser(TargetCopy,ExistingUsername, static_cast<int32_t>(Authorizer::ReadPermissions(CurrentUser) ) );

        switch (m_RepositoryReference.UpdateUser(CurrentUser,TargetCopy, target))
        {

        case UserState::Failed: // for some reason....
        {
            term::Error("Operation failed, try again later.");
            break;

        }
        case UserState::Successful :
        {
            Logger::LogUser(Actor, Logger::Category::UpdateUser, Logger::Level::INFO, "", ExistingUsername);
            term::Blank();
            term::Success("User updated successfully.");
            break;

        }
        case UserState::UserIsAdmin :
        {
            UserIsAdminMsg("Update");
            break;
        }
        case UserState::SuccessfulSelfUpdate:
        {
            Logger::LogUser(Actor, Logger::Category::UpdateUser, Logger::Level::INFO, "", ExistingUsername);
            SelfEditMsg("Updated");
            break;
        }

        default:
        {
            break;

        }

        }

    }

    // NOTE: the old version also asked for a password here and then never used it, so the prompt was removed.
    void _Update(User& CurrentUser,const std::string& Username) {

        User user = m_RepositoryReference.Find(Username);

        if (user.isEmpty())
        {
            term::Error("User was not found.");

        }
        else
        {
            (_PerformConfirmation(user)) ? _PrintUpdateStatus(CurrentUser, user, Username) : term::Warn("Operation cancelled.");
        }

    }

    bool _PerformUpdate(User& CurrentUser) {

        _ClearScreen();
        _Header(CurrentUser, "UPDATE USER");

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::UpdateUser))
        {
            term::Prompt("USERNAME");
            std::string Username = Validator::ReadString();

            _Update(CurrentUser, Username);
            return true;
        }
        else
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Update User");
            NoAccessMsg();
            return false;
        }

    }

public:

    UpdateUserScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

    void Start(User & CurrentUser) override {
        PerformMenu(CurrentUser);
    }
};
