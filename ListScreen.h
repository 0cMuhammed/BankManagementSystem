#pragma once
#include <iostream>
#include<vector>
#include "Client.h"
#include "Repository.h"
#include<iomanip>

#include "UI.h"

class ListScreen : public UI 
{ 

private:
    Repository& m_RepositoryReference; // a reference to any Repository object u pass in a constructor for better seperation of concerns

    void _PrintHeader(const std::string* ScreenName = nullptr, const std::string* SubTitle = nullptr) override {
        std::cout << "\t\t\t\t\t______________________________________";

         std::cout << "\n\n\t\t\t\t\t  " << (ScreenName == nullptr) ? "Client List" : *ScreenName;

         std::cout << "\n\t\t\t\t\t  " << (SubTitle == nullptr)  ? "" : *SubTitle;

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";

    }
    void _PerformMenu(const std::string* Message = nullptr) override {
        std::cout <<  (Message == nullptr) ? "\nPress Enter to go back to Main Menu.....\n" : "\n" + *Message + "....." + "\n";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    static void _PrintClient(const Client &client)
    {

        std::cout << std::setw(8) << std::left << "" << "| " << std::setw(15) << std::left << client.getAccountNumber();
        std::cout << "| " << std::setw(20) << std::left << client.getFullName();
        std::cout << "| " << std::setw(12) << std::left << client.getPhoneNumber();
        std::cout << "| " << std::setw(20) << std::left << client.getEmail();
        std::cout << "| " << std::setw(10) << std::left << client.getPinCode();
        std::cout << "| " << std::setw(12) << std::left << client.getBalance();

    }
    static void _PrintLine() {
        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_________________________________________\n" << std::endl;
    }
    static void _PrintLayout() {

        _PrintLine();
        std::cout << std::setw(8) << std::left << "" << "| " << std::left << std::setw(15) << "Account Number";
        std::cout << "| " << std::left << std::setw(20) << "Client Name";
        std::cout << "| " << std::left << std::setw(12) << "Phone";
        std::cout << "| " << std::left << std::setw(20) << "Email";
        std::cout << "| " << std::left << std::setw(10) << "Pin Code";
        std::cout << "| " << std::left << std::setw(12) << "Balance";
        _PrintLine();
    }
   
   
    static void _PrintAll(const std::vector<Client> &AllClients) {
        for (const Client& client : AllClients)
        {

            _PrintClient(client);
            std::cout << std::endl;
        }
        _PrintLine();
        
    }

public :
    ListScreen(Repository& Repo) : m_RepositoryReference(Repo) {};

    void Show() override {

      

       if (m_RepositoryReference.GetClientList().size() != 0) 
       {

        const std::string SubTitle = "\t    (" + std::to_string(m_RepositoryReference.GetClientList().size()) + ") Client(s).";

        _PrintHeader(nullptr,&SubTitle);
        _PrintLayout();
        _PrintAll(m_RepositoryReference.GetClientList());


       }
       else 
       {
               std::cout << "\t\t\t\tNo Clients Available In the System!";
       }
       _PerformMenu();

    }
    
};

