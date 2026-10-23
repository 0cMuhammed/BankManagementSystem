#pragma once
#include <iostream>
#include<vector>
#include "Client.h"
#include "ClientRepository.h"
#include "Screen.h"
#include<iomanip>
#include <limits>

class ListScreen : public Screen
{ 

private:

   const ClientRepository& m_RepositoryReference;  // read only on repository dependency injection

   static constexpr int W = 100;  // NEW: this screen is wider than the default (66) because the table has 6 columns. All term:: calls below receive W so everything is centered as one block.

   // NEW: prints the column titles (grey) and a thin line under them.
   // Column widths MUST match _PrintRow below: 8 + 18 + 12 + 24 + 10 + 14 = 86, plus 5 gaps of 2 spaces = 96, plus 2 spaces on the left = 98.
   static void _PrintLayout() {
       term::Print(std::string(term::col::Muted) + "  " + term::Pad("ACCOUNT", 8) + "  " + term::Pad("NAME", 18) + "  " + term::Pad("PHONE", 12)
                   + "  " + term::Pad("EMAIL", 24) + "  " + term::Pad("SHARE", 10) + "  " + term::PadLeft("BALANCE", 14), W);
       term::Rule(W);                                           // thin cyan line
   }

   // NEW: prints ONE client as ONE row. Replaces the old _PrintFormattedClient + std::setw lines.
   // "total" is the sum of all balances; it is used to draw the SHARE bar (how big this client's balance is compared to the whole bank).
   static void _PrintRow(const Client& client, double total) {
       const double balance = client.getBalance();               // this client's money
       const double share = (total > 0) ? balance / total : 0;   // 0.0 .. 1.0 (guard against dividing by zero when the bank is empty)
       const char* barColor = (share > 0.5) ? term::col::Magenta : term::col::Cyan; // a client holding more than half of everything gets the highlight color

       term::Print(std::string("  ") + term::col::Cyan + term::Pad(client.getAccountNumber(), 8) // account number in cyan
                   + term::col::Base + "  " + term::Pad(term::Fit(client.GetFullName(), 18), 18)   // name, cut with "..." if longer than 18 so columns never shift
                   + term::col::Muted + "  " + term::Pad(client.GetPhoneNumber(), 12)              // phone in grey
                   + "  " + term::Pad(term::Fit(client.GetEmail(), 24), 24)                        // email, cut at 24
                   + "  " + term::Bar(share, 10, barColor)                                         // the share bar (10 blocks wide)
                   + term::col::Base + "  " + term::PadLeft(term::Money(balance), 14), W);         // balance, right-aligned: $12,450.00
   }

   void _List(const User &CurrentUser) 
   {
       const std::vector<Client>& clients = m_RepositoryReference.GetList(); // read once, use many times (the old code called GetList() 3 times)

       if (clients.empty())
       {
           _Header(CurrentUser, "CLIENTS", "0 RECORDS", W);     // header still shown so the screen looks consistent
           term::Warn("No clients available in the system.", W);// NEW: colored line instead of _Message with 4 tabs
           return;                                              // nothing else to print
       }

       double total = 0;                                        // sum of all balances, needed for the share bars
       for (const Client& c : clients) total += c.getBalance();

       _Header(CurrentUser, "CLIENTS", std::to_string(clients.size()) + " RECORDS", W); // replaces PrintHeader override + SubTitle string
       _PrintLayout();                                          // column titles

       for (const Client& client : clients)                     // one row per client
           _PrintRow(client, total);

       term::Rule(W);                                           // closing line
       term::Print(std::string(term::col::Muted) + "  TOTAL" + term::col::Cyan + term::PadLeft(term::Money(total), W - 4 - 5), W); // "TOTAL" on the left, sum on the right edge (W - 4 margins - 5 letters)
   }

   void PerformMenu(const User& CurrentUser, const char* Message = nullptr) override {

       if (Authorizer::HasAccess(CurrentUser, Authorizer::Permissions::ShowClientList))
       {
           Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::ShowClientList, Logger::Level::INFO);
           _List(CurrentUser);
       }
       else
       {
           Logger::LogClient(CurrentUser.GetUsername(), Logger::Category::NoAccsess, Logger::Level::WARN, "List Clients");
           _Header(CurrentUser, "CLIENTS");           // NEW: header first, so the denial message has a screen around it
           NoAccessMsg();
       }

   }

    
public :
   

    ListScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessClientServices().AccessRepository()) {};

    void Start(const User &CurrentUser) override {

      

        PerformMenu(CurrentUser);
       _GetBackToMenu();

    }
    
};
