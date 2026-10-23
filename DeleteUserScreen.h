#pragma once
#include <iostream>
#include<vector>
#include "Validator.h"
#include "Screen.h"

class DeleteUserScreen : public Screen
{
private:

    UserRepository& m_RepositoryReference;

    void PerformMenu(User& CurrentUser,const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        while (IsContinueOperation)
        {
            if (!_PerformDelete(CurrentUser))
            {
                _GetBackToMenu("Press any key to go back to the Manage Users menu");
                return;
            }

            if (CurrentUser.isEmpty())   // the user deleted THEIR OWN account: show the message, then leave
            {
                _GetBackToMenu("Press any key to go back to the Manage Users menu");
                return;
            }

            term::Blank();
            IsContinueOperation = term::Confirm((Message != nullptr) ? Message : "Delete another user?");
        }

    }

    static bool _PerformConfirmation(const User& user, const char* Message = nullptr) {
        _PrintUserCard(user);
        return term::Confirm((Message != nullptr) ? Message : "Are you sure you want to delete this user?");
    }
    void _PrintDeleteStatus(User &CurrentUser, const User& target) {

        const std::string Actor = CurrentUser.GetUsername();

        if (m_RepositoryReference.IsAdmin(target))
        {
            Logger::LogUser(Actor, Logger::Category::DeleteUser, Logger::Level::WARN, "", target.GetUsername());
            UserIsAdminMsg("Delete");

            return;
        }

        switch (m_RepositoryReference.DeleteUser(CurrentUser,target))
        {

        case UserState::Failed: // for some reason....
        {
            term::Error("Operation failed, try again later.");
            break;

        }
        case UserState::Successful:
        {
            Logger::LogUser(Actor, Logger::Category::DeleteUser, Logger::Level::INFO, "", target.GetUsername());
            term::Success("User deleted successfully.");
            break;

        }
        case UserState::UserIsAdmin:
        {
            UserIsAdminMsg("Delete");
            break;
        }
        case UserState::SuccessfulSelfDelete:
        {
            Logger::LogUser(Actor, Logger::Category::DeleteUser, Logger::Level::INFO, "", target.GetUsername());
            SelfEditMsg("deleted");
            break;
        }

        default:
        {
            break;

        }

        }
    }

    // NOTE: the old version also asked for a password here and then never used it, so the prompt was removed.
    void _Delete(User &CurrentUser,const std::string& Username) {

        User user = m_RepositoryReference.Find(Username);

        if (user.isEmpty())
        {
            term::Error("User was not found.");

        }
        else
        {
            ( _PerformConfirmation(user) ) ? _PrintDeleteStatus(CurrentUser, user) : term::Warn("Operation cancelled.");
        }

    }
    bool _PerformDelete(User &CurrentUser) {

        _ClearScreen();
        _Header(CurrentUser, "DELETE USER");

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::DeleteUser))
        {
            term::Prompt("USERNAME");
            std::string username = Validator::ReadString();

            _Delete(CurrentUser, username);
            return true;
        }
        else
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Delete User");
            NoAccessMsg();
            return false;
        }

    }

public:

    DeleteUserScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

    void Start(User &CurrentUser) override {
         PerformMenu(CurrentUser);
    }
};
