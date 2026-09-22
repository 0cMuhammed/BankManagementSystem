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
#include "UpdateScreen.h"
#include "FindScreen.h"
#include "TransactionsScreen.h"
#include "ManageUsersScreen.h"

#include "Screen.h"


class MainMenuScreen : public Screen
{

private :

	enum MainMenuComponents { List = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Transactions = 6, ManageUsers = 7, Logout = 8 };

    Session& m_Session;
	Service& m_ServicesRef;

    

  
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

			std::cout << "\n\n\t\t\t\t\t  \t\t" << (((ScreenName != nullptr) ? ScreenName : "  Main Menu"));
            
                if (SubTitle != nullptr) 
                { 
                    std::cout << "\n\t\t\t\t\t\t  " << SubTitle; 
                }

            std::cout << "\n\t\t\t\t\t______________________________________\n\n";

            ShowUserAndDate(m_Session.GetUser());
          
    }
    void _Menu(bool &isInMainMenu) {
        do
        {
            _MainMenuLayout();

            switch (_NavigateUser())
            {
            case MainMenuComponents::List:
            {
                ListScreen List(m_ServicesRef);
                List.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::Add:
            {
                AddScreen Add(m_ServicesRef);
                Add.Start(m_Session.GetUser());
                break;
            }
            case MainMenuComponents::Delete:
            {
                DeleteScreen Delete(m_ServicesRef);
                Delete.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::Update:
            {
                UpdateScreen Update(m_ServicesRef);
                Update.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::Find:
            {
                FindScreen Find(m_ServicesRef);
                Find.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::Transactions:
            {
                TransactionsScreen Transactions(m_ServicesRef, m_Session);
                Transactions.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::ManageUsers:
            {
                ManageUsersScreen ManageUsers(m_ServicesRef, m_Session);
                ManageUsers.Start(m_Session.GetUser());
                break;

            }
            case MainMenuComponents::Logout:
            {
                m_Session.Logout(isInMainMenu);
                break;

            }
            default: //for later enum choices
            {
                break;

            }



            }

        } while (isInMainMenu);
    }

    void PerformMainMenu(Session& CurrentUser, const char* Message = nullptr) override {

        bool isInMainMenu = true;

        if (CurrentUser.isActive()) 
        {
           

            _Menu(isInMainMenu);
        }
        else 
        {
            NoAccessMsg();
            CurrentUser.SetIsActive(false);
        }
       
        
    }
    

    

public : 

    MainMenuScreen(Service& Ref, Session &CurrentUser) : Screen(Ref), m_Session(CurrentUser), m_ServicesRef(Ref) {};


    void StartMainMenu(Session &CurrentUser) override {
        PerformMainMenu(CurrentUser);
    }

};

