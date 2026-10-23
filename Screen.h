#pragma once
#include "UI.h"
#include "Service.h"
#include "Logger.h"
#include "Date.h"
#include "Terminal.h"   // NEW: the rendering layer (colors, header, menu, money...)


class Screen : public UI {
    // abstract class : cannot be instatated 

    private :

    Service& m_ServiceRef;

    protected : // only the subclasses under Screen ineherits these members internally and the objects of the subclasses cannot call these methods directly, only used in the implementation.

       
        void _ClearScreen() // NEW: no more system("cls") (slow, flickers, flashes white). term::Clear() paints the whole window black instantly.
        { 
            term::Clear(); 
        }

        // NEW: ONE header for every screen. Screens call this instead of writing their own PrintHeader override with hard-coded tabs.
        // title  = big word at the left (e.g. "CLIENTS"), status = optional quiet text at the right end of the header (e.g. "5 RECORDS"), empty = nothing, w = width of the screen's content area
        void _Header(const User& CurrentUser, const std::string& title, const std::string& status = "", int w = term::kW)
        {
            term::Header(title, status, CurrentUser.GetUsername(), w);
        }
        virtual void UserIsAdminMsg(const std::string& process) {
            _ClearScreen();
            term::Blank();
            term::Warn("You can't " + process + " the admin.");   // NEW: one colored line instead of 4 lines of tabs and underscores
        } 
        virtual void SelfEditMsg(const std::string& process) {
            _ClearScreen();
            term::Blank();
            term::Warn("Your account has been " + process + ". Please log out and sign in again.");
        }
        
        virtual void _GetBackToMenu(const char* Message = nullptr) 
        { 
            std::cin.clear(); // keep this: clears any "bad input" state left on cin by an earlier invalid number
            term::WaitKey((Message != nullptr) ? Message : "Press any key to go back to the Main Menu"); // NEW: waits for ONE key press (the old cin.ignore + cin.get trick needed a leftover newline in the buffer, which the arrow-key menu no longer leaves behind)
        }
        virtual User Login() {
            std::cout << "Login Screen here\n";
            return User("", "", "", "", "", "", 0, Mode::EmptyMode);
        }

        virtual void NoAccessMsg(const char* ScreenName = nullptr, const char* SubTitle = nullptr) {
            term::Error((ScreenName != nullptr) ? ScreenName : "Access denied. Please contact your admin."); // NEW: red cross + message
            if (SubTitle != nullptr) term::Info(SubTitle);                                                   // optional quiet second line
        }
        // NEW: the three "cards" now live here (UI code) instead of inside the repositories (data code).
        // Each card is a few  LABEL   value  lines made with term::Field.
        static void _PrintClientCard(const Client& c)
        {
            term::Blank();
            term::Field("ACCOUNT", c.getAccountNumber(), term::col::Cyan);
            term::Field("NAME",    c.GetFullName(), term::col::White);
            term::Field("PHONE",   c.GetPhoneNumber());
            term::Field("EMAIL",   c.GetEmail());
            term::Field("BALANCE", term::Money(c.getBalance()), term::col::Green);
            term::Blank();
        }

        // NEW: receipt for ONE-account money operations (deposit / withdraw).
        // Names and account numbers arrive as plain text captured BEFORE the operation, because Transactions::Deposit/Withdraw
        // end with UpdateClient(), which moves the data out of the Client object (its name is empty afterwards).
        // newColor: Green when money arrives (deposit), Amber when money leaves (withdraw).
        static void _PrintMoneyReceipt(const std::string& title, const std::string& name, const std::string& acc,
                                       double oldBalance, double newBalance, double amount, const char* newColor)
        {
            term::Blank();
            term::Success(title);
            term::Blank();
            term::Field("ACCOUNT", acc, term::col::Cyan);
            term::Field("NAME", name, term::col::White);
            term::Print(std::string(term::col::Muted) + "  " + term::Pad("BALANCE", 12)
                        + term::col::Muted + term::Money(oldBalance)                                   // old balance, grey
                        + term::col::Cyan + " " + term::g::Rule + term::g::Play + " "                  // the arrow
                        + newColor + term::Money(newBalance));                                         // new balance
            term::Field("AMOUNT", term::Money(amount), term::col::White);
        }

        static void _PrintUserCard(const User& u)
        {
            term::Blank();
            term::Field("USERNAME",    u.GetUsername(), term::col::Cyan);
            term::Field("NAME",        u.GetFullName(), term::col::White);
            term::Field("PHONE",       u.GetPhoneNumber());
            term::Field("EMAIL",       u.GetEmail());
            term::Field("PERMISSIONS", std::to_string(u.GetPermissions()), term::col::Amber);
            term::Blank();
        }

        static void _PrintCurrency(Currency &c)
        {
            term::Blank();
            term::Field("COUNTRY",   c.GetCountry(), term::col::White);
            term::Field("CODE",      c.GetCode(), term::col::Cyan);
            term::Field("NAME",      c.GetName());
            term::Field("RATE (1$)", term::Fixed(c.GetRate(), 4), term::col::Green);
            term::Blank();
        }
    public:

       Screen(Service& ref) : m_ServiceRef(ref) {}

        
};


