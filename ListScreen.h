#pragma once
#include <iostream>
#include<vector>
#include "Client.h"
#include "ClientRepository.h"
#include<iomanip>
#include <limits>


#include "UI.h"

class ListScreen : public MainMenuScreen 
{ 

private:
   const ClientRepository& m_RepositoryReference;  // read only on repository dependency injection

    //universal
   void _ClearScreen() override {
       system("cls");
    }
   void _GetBackToMenu(const char* Message = nullptr) override {
       std::cout << '\n' + (((Message != nullptr) ? Message : "Press Enter to go back to Main Menu")); std::cout << ".....\n";

       std::cin.clear();
       std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
       std::cin.get();
   }
   void _Message(const char *Message = nullptr) override {
       std::cout << '\n' + Message;
   }
   
   void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {
       
        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  " << ( ( (ScreenName != nullptr) ? ScreenName : "Client List") );

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";

    }
   void PerformMenu(const char* Message = nullptr) override {
       if (m_RepositoryReference.GetList().size() == 0)
       {

           _Message("\t\t\t\tNo Clients Available In the System!");
           

       }
       else
       {
           const std::string SubTitle = "\t    (" + std::to_string(m_RepositoryReference.GetList().size()) + ") Client(s).";

           PrintHeader(nullptr, SubTitle.c_str());
           _PrintLayout();
           _PrintAll(m_RepositoryReference.GetList());

       }
   }

    //exclusive
    static void _PrintFormattedClient(const Client &client)
    {

        std::cout << std::setw(8) << std::left << "" << "| " << std::setw(15) << std::left << client.getAccountNumber();
        std::cout << "| " << std::setw(20) << std::left << client.GetFullName();
        std::cout << "| " << std::setw(12) << std::left << client.GetPhoneNumber();
        std::cout << "| " << std::setw(20) << std::left << client.GetEmail();
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

            _PrintFormattedClient(client);
            std::cout << std::endl;
        }
        _PrintLine();
        
    }

    
public :
   

    ListScreen(ClientServices& Ref) : MainMenuScreen(Ref), m_RepositoryReference(Ref.AccessRepository()) {};

    void Start() override {

      

        PerformMenu();
       _GetBackToMenu();

    }
    
};

