#include "Client.h"
#include "FileHandler.h"

void Client::_SaveExistingObject() {
    std::vector<Client> Clients = FileHandler::LoadClients();

    for (Client& client : Clients)
    {
        if (client.getAccountNumber() == this->m_accountNumber)
        {
            client = *this;
            break;
        }
    }

    FileHandler::SaveClients(Clients);
}

void Client::_SaveNewObject() {
   this->m_mode = ObjectMode::ExistingMode;
    FileHandler::SaveClients(*this);
}