#pragma once
#include "UI.h"
#include "Service.h"
#include "Date.h"
class Screen : public UI {
    // abstract class : cannot be instatated 

    private :

    Service& m_ServiceRef;

    protected : // only the subclasses under Screen ineherits these members internally and the objects of the subclasses cannot call these methods directly, only used in the implementation.

       
        void _ClearScreen() // fixed 
        { 
            system("cls"); 
        }
        virtual void UserIsAdminMsg(const std::string& process) {
            _ClearScreen();
            std::cout << "\t\t\t\t\t______________________________________";

            std::cout << "\n\n\t\t\t\t\t      You can't " << process + " the admin.";

            std::cout << "\n\t\t\t\t\t______________________________________\n\n";
        } 
        virtual void SelfEditMsg(const std::string& process) {
            _ClearScreen();
            std::cout << "\t\t__________________________________________________________________________________________________________________";

            std::cout << "\n\n\t\t\t\t Note : Your account has been " + process + ", please logout and sign in again.";

            std::cout << "\n\t\t__________________________________________________________________________________________________________________\n\n";
        }
        
        virtual void _GetBackToMenu(const char* Message = nullptr) 
        { 
            std::cout << '\n' + std::string( (((Message != nullptr) ? Message : "Press Enter to go back to Main Menu"))) + ".....\n";

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
        virtual void _Message(const std ::string &Message)
        {
            std::cout<<"\n" << Message;
        }
        
        virtual User Login() {
            std::cout << "Login Screen here\n";
            return User("", "", "", "", "", "", 0, Mode::EmptyMode);
        }

        virtual void NoAccessMsg(const char* ScreenName = nullptr, const char* SubTitle = nullptr) {

            std::cout << "\t\t\t\t\t______________________________________";

            std::cout << "\n\n\t\t\t\t          " << (((ScreenName != nullptr) ? ScreenName : "Access Denied, Please contact your Admin"));

            if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

            std::cout << "\n\t\t\t\t\t______________________________________\n\n";

        }
        virtual void ShowCurrentUser(const User &CurrentUser) {

            
            std::cout << "\t\t\t\t\tUser : " << CurrentUser.GetUsername();
            

        }
        virtual void ShowDate() {

            Date Today;
            std::cout << "\n\t\t\t\t\tDate : " << Today.DateToString() << '\n';


        }
        virtual void ShowUserAndDate(const User& CurrentUser) {
            ShowCurrentUser(CurrentUser);
            ShowDate();
        }
  

    public:

       Screen(Service& ref) : m_ServiceRef(ref) {}

        
};


