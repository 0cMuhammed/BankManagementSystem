#pragma once
#include <iostream>
#include<vector>
#include "Validator.h"
#include "ClientRepository.h"
#include "Screen.h"

class AddScreen : public Screen
{
private :

    ClientRepository& m_RepositoryReference;

    void PerformMenu(const User & CurrentUser, const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        while (IsContinueOperation)
        {
            if (!_PerformAdding(CurrentUser))                  // NEW: stop (instead of asking "continue?") when access is denied
            {
                _GetBackToMenu();
                return;
            }

            term::Blank();
            IsContinueOperation = term::Confirm((Message != nullptr) ? Message : "Add another client?");
        }

    }

    void _Add(const User& CurrentUser, Client& New, const std::string &AccountNumber = "Empty") {

        switch (m_RepositoryReference.AddClient(New))
        {

        case ClientRepository::OperationStates::Successful :
        {
            Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::AddClient, Logger::Level::INFO, "", New.getAccountNumber());
            _PrintClientCard(New);                             // NEW: replaces ClientRepository::PrintClient
            term::Success("Account saved successfully.");
            break;

        }
        case ClientRepository::OperationStates::AccountNumberAlreadyExists :
        {
            term::Error("Account number is already used.");
            break;

        }
        case ClientRepository::OperationStates::Failed : // for some reason....
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

    bool _PerformAdding(const User & CurrentUser) {

        _ClearScreen();

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::AddClient))
        {
            _Header(CurrentUser, "ADD CLIENT");                // NEW: replaces the PrintHeader override
            Client New = m_RepositoryReference.ReadClient();
            _Add(CurrentUser, New);
            return true;
        }
        else
        {
            Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Add Client");
            _Header(CurrentUser, "ADD CLIENT");
            NoAccessMsg();
            return false;
        }

    }

    public :

        AddScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessClientServices().AccessRepository()) {};

    void Start(const User &CurrentUser) override
    {
        PerformMenu(CurrentUser);
    }

};
