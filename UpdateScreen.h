#pragma once
#include <iostream>
#include<vector>

#include "Client.h"
#include "Validator.h"
#include "ClientRepository.h"
#include "Screen.h"
#include "Authorizer.h"

class UpdateScreen : public Screen
{
private:
    ClientRepository& m_RepositoryReference;

    void PerformMenu(const User &CurrentUser, const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        while (IsContinueOperation)
        {
            if (!_PerformUpdate(CurrentUser))
            {
                _GetBackToMenu();
                return;
            }

            term::Blank();
            IsContinueOperation = term::Confirm((Message != nullptr) ? Message : "Update another client?");
        }

    }

    //exclusive
    static bool _PerformConfirmation(const Client& client, const char* Message = nullptr) {
        _PrintClientCard(client);                                // NEW: replaces ClientRepository::PrintClient
        return term::Confirm((Message != nullptr) ? Message : "Do you want to update this client?");
    }
    void _PrintUpdateStatus(const User& CurrentUser, Client& client, const std::string &ExistingAccountNumber) {

        client = ClientRepository::UpdateExistingClient(client,ExistingAccountNumber);

        switch (m_RepositoryReference.UpdateClient(client))
        {

        case ClientRepository::OperationStates::Failed: // for some reason....
        {
            term::Error("Operation failed, try again later.");
            break;

        }
        case ClientRepository::OperationStates::Successful:
        {
            Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::UpdateClient, Logger::Level::INFO, "", ExistingAccountNumber);
            term::Blank();
            term::Success("Account updated successfully.");
            break;

        }

        default:
        {
            break;

        }

        }

    }
    void _Update(const User& CurrentUser, const std::string& AccountNumber) {

        Client client = m_RepositoryReference.Find(AccountNumber);

        if (client.isEmpty())
        {
            term::Error("Account number was not found.");

        }
        else
        {
            (_PerformConfirmation(client)) ? _PrintUpdateStatus(CurrentUser, client, AccountNumber) : term::Warn("Operation cancelled.");
        }

    }
    bool _PerformUpdate(const User & CurrentUser) {

        _ClearScreen();

        if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::UpdateClient))
        {
            _Header(CurrentUser, "UPDATE CLIENT");

            term::Prompt("ACCOUNT NUMBER");
            std::string AccountNumber = Validator::ReadString();

            _Update(CurrentUser, AccountNumber);
            return true;
        }
        else
        {
            Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "Update Client");
            _Header(CurrentUser, "UPDATE CLIENT");
            NoAccessMsg();
            return false;
        }

    }

public:

    UpdateScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessClientServices().AccessRepository()) {};

    void Start(const User & CurrentUser) override {
        PerformMenu(CurrentUser);
    }
};
