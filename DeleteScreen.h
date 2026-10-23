#pragma once
#include <iostream>
#include<vector>
#include "ClientRepository.h"
#include "Validator.h"
#include "Screen.h"

class DeleteScreen : public Screen
{

private :

    ClientRepository& m_RepositoryReference;

    void PerformMenu(const User &CurrentUser, const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        while (IsContinueOperation)
        {
            if (!_PerformDelete(CurrentUser))
            {
                _GetBackToMenu();
                return;
            }

            term::Blank();
            IsContinueOperation = term::Confirm((Message != nullptr) ? Message : "Delete another client?");
        }

    }

    static bool _PerformConfirmation(const Client &client, const char* Message = nullptr) {
        _PrintClientCard(client);
        return term::Confirm((Message != nullptr) ? Message : "Are you sure you want to delete this client?");
    }

    void _PrintDeleteStatus(const User& CurrentUser, Client& client) {

        switch (m_RepositoryReference.DeleteClient(client))
        {

        case ClientRepository::OperationStates::Failed : // for some reason....
        {
            term::Error("Operation failed, try again later.");
            break;

        }
        case ClientRepository::OperationStates::Successful:
        {
            Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::DeleteClient, Logger::Level::INFO, "", client.getAccountNumber());
            term::Success("Account deleted successfully.");
            break;

        }

        default:
        {
            break;

        }

        }
    }
    void _Delete(const User& CurrentUser, const std::string& AccountNumber) {

        Client client = m_RepositoryReference.Find(AccountNumber);

        if (client.isEmpty())
        {
            term::Error("Account number was not found.");

        }
        else
        {
            (_PerformConfirmation(client)) ? _PrintDeleteStatus(CurrentUser, client) : term::Warn("Operation cancelled.");
        }

    }
    bool _PerformDelete(const User &CurrentUser) {

        _ClearScreen();

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::DeleteClient))
        {
            _Header(CurrentUser, "DELETE CLIENT");

            term::Prompt("ACCOUNT NUMBER");
            std::string AccountNumber = Validator::ReadString();

            _Delete(CurrentUser, AccountNumber);
            return true;
        }
        else
        {
            Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Delete Client");
            _Header(CurrentUser, "DELETE CLIENT");
            NoAccessMsg();
            return false;
        }

    }

    public :

        DeleteScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessClientServices().AccessRepository()) {};

        void Start(const User &CurrentUser) override {
            PerformMenu(CurrentUser);
        }

};
