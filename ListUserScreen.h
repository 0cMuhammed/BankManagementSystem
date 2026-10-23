#pragma once
#include <iostream>
#include<vector>
#include <limits>
#include "Session.h"
#include "Authorizer.h"
#include "Screen.h"

class ListUserScreen : public Screen
{
private :

    const UserRepository& m_RepositoryReference;

    static constexpr int W = 94;   // NEW: width of this table (see the column widths below: 2 + 14+2+18+2+12+2+24+2+12 = 90, plus 2 margin each side)

    // NEW: column titles + thin line (widths must match _PrintRow)
    static void _PrintLayout() {
        term::Print(std::string(term::col::Muted) + "  " + term::Pad("USERNAME", 14) + "  " + term::Pad("NAME", 18) + "  " + term::Pad("PHONE", 12)
                    + "  " + term::Pad("EMAIL", 24) + "  " + term::PadLeft("PERMISSIONS", 12), W);
        term::Rule(W);
    }

    // NEW: one user = one row. The logged-in user's own row is highlighted in magenta.
    static void _PrintRow(const User& user, const std::string& CurrentUsername) {
        const bool isMe = (user.GetUsername() == CurrentUsername);
        term::Print(std::string("  ") + (isMe ? term::col::Magenta : term::col::Cyan) + term::Pad(term::Fit(user.GetUsername(), 14), 14)
                    + term::col::Base + "  " + term::Pad(term::Fit(user.GetFullName(), 18), 18)
                    + term::col::Muted + "  " + term::Pad(user.GetPhoneNumber(), 12)
                    + "  " + term::Pad(term::Fit(user.GetEmail(), 24), 24)
                    + term::col::Amber + "  " + term::PadLeft(std::to_string(user.GetPermissions()), 12), W);
    }

    void _ShowList(const User &CurrentUser) {

        const std::vector<User>& users = m_RepositoryReference.GetList();

        _ClearScreen();

        if (users.empty())
        {
            _Header(CurrentUser, "USERS", "0 RECORDS", W);
            term::Warn("No users available in the system.", W);
            return;
        }

        _Header(CurrentUser, "USERS", std::to_string(users.size()) + " RECORDS", W);
        _PrintLayout();

        Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::ShowUserList, Logger::Level::INFO);   // (was inside _PrintAll)

        for (const User& u : users)
            _PrintRow(u, CurrentUser.GetUsername());

        term::Rule(W);
    }

    void PerformMenu(const User &CurrentUser, const char* Message = nullptr) override {

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::ShowUserList))
        {

            _ShowList(CurrentUser);
        }
        else
        {
            Logger::LogUser(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::INFO, "List Users");
            _Header(CurrentUser, "USERS");
            NoAccessMsg();
        }

    }

public:

    ListUserScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessUserServices().AccessRepository()) {};

    void Start(const User &CurrentUser) override {

        PerformMenu(CurrentUser);
        _GetBackToMenu("Press any key to go back to the Manage Users menu");

    }
};
