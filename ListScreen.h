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


    void _PrintHeader(const std::string* ScreenName = nullptr, const std::string* SubTitle = nullptr) override {
        std::cout << "\t\t\t\t\t______________________________________";

        (ScreenName == nullptr) ? std::cout << "\n\n\t\t\t\t\t  " << "Client List" : std::cout << "\n\n\t\t\t\t\t  " << *ScreenName;
        if (SubTitle != nullptr)  std::cout << "\n\t\t\t\t\t  " << *SubTitle;

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";

    }
    void _PerformMenu(const std::string* Message = nullptr) override {
        (Message == nullptr) ? std::cout << "\nPress Any Key to go back to Main Menu.....\n" : std::cout << "\n" << *Message << "....." << "\n";
        system("pause>0");
    }

    static void _PrintClient(const Client &Client)
    {

        std::cout << std::setw(8) << std::left << "" << "| " << std::setw(15) << std::left << Client.getAccountNumber();
        std::cout << "| " << std::setw(20) << std::left << Client.getFullName();
        std::cout << "| " << std::setw(12) << std::left << Client.getPhoneNumber();
        std::cout << "| " << std::setw(20) << std::left << Client.getEmail();
        std::cout << "| " << std::setw(10) << std::left << Client.getPinCode();
        std::cout << "| " << std::setw(12) << std::left << Client.getBalance();

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
        std::cout << std::setw(8) << std::left << "" << "\n\t_______________________________________________________";
        std::cout << "_________________________________________\n" << std::endl;

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
    ListScreen() = default;
    void Show() override {

       Repository AllClients;

       if (AllClients.GetClientList().size() != 0) 
       {
        const std::string SubTitle = "\t    (" + std::to_string(AllClients.GetClientList().size()) + ") Client(s).";

        _PrintHeader(nullptr,&SubTitle);
        _PrintLayout();
        _PrintAll(AllClients.GetClientList());

       }
       else 
       {
               std::cout << "\t\t\t\tNo Clients Available In the System!";
       }
       _PerformMenu();

    }
    
};

