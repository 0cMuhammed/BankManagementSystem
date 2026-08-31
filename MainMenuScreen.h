#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include <string>
#include "Validator.h"
#include "Service.h"


#include "ListScreen.h"
#include "AddScreen.h"
#include "DeleteScreen.h"
#include "UpdateScreen.h"`
#include "FindScreen.h"
#include "TransactionsScreen.h"
#include "ManageUsersScreen.h"

#include "Screen.h"

class MainMenuScreen : public Screen
{

private :

	enum MainMenuComponents { List = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Transactions = 6, ManageUsers = 7, Logout = 8 };

	Service& m_ServicesRef;

    
    static void _ExitMenu(bool& isInMainMenu, const char* message = "\nLogging Out...")
    {
        std::cout << message << "\n\n";
        
        isInMainMenu = false;
        
    }
  
  

     void _MainMenuLayout()
    {

        _ClearScreen();
        PrintHeader();
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t\t\tMain Menue\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t[1] Show Client List.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[2] Add New Client.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[3] Delete Client.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[4] Update Client.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[5] Find Client.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[6] Transactions.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[7] Manage Users.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[8] Logout.\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";


    }
	 MainMenuComponents _NavigateUser(double from = 1, double to = 8)
	{
         _Message("Choose What do you want to do ? [1 to 8] : ");

		return  (MainMenuComponents) Validator::returnValidatedNumber(from, to);
	}
    

    
	void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) {
		
			std::cout << "\t\t\t\t\t______________________________________";

			std::cout << "\n\n\t\t\t\t\t  \t\t" << (((ScreenName != nullptr) ? ScreenName : "Main Menu"));
            
                if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t\t  " << SubTitle; }

            std::cout << "\n\t\t\t\t\t______________________________________\n\n";

    }
    void PerformMenu(const char* Message = nullptr) override {

        bool isInMainMenu = true;
        do
        {
            _MainMenuLayout();
            
            switch (_NavigateUser())
            {
            case MainMenuComponents::List: 
            {
                ListScreen List(m_ServicesRef);
                List.Start();
                break;
              
            }
            case MainMenuComponents::Add: 
            {
                AddScreen Add(m_ServicesRef);
                Add.Start();
                break;
            }
            case MainMenuComponents::Delete: 
            {
                DeleteScreen Delete(m_ServicesRef);
                Delete.Start();
                break;

            }
            case MainMenuComponents::Update: 
            {
                UpdateScreen Update(m_ServicesRef);
                Update.Start();
                break;

            }
            case MainMenuComponents::Find: 
            {
                FindScreen Find(m_ServicesRef);
                Find.Start();
                break;

            } 
            case MainMenuComponents::Transactions:
            {
                TransactionsScreen Transactions(m_ServicesRef);
                Transactions.Start();
                break;

            }
            case MainMenuComponents::ManageUsers:
            {
                ManageUsersScreen ManageUsers(m_ServicesRef);
                ManageUsers.Start();
                break;

            }
            case MainMenuComponents::Logout: 
            {
                _ExitMenu(isInMainMenu);
                break;

            }
            default: //for later enum choices
            {
                break;

            }
                                       


            }

        } while (isInMainMenu);
        
    }
    

    

public : 

    MainMenuScreen(Service& Ref) : Screen(Ref), m_ServicesRef(Ref) {};


    void Start() override {
        PerformMenu();
    }

};

