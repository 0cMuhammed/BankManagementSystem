#pragma once
#include <iostream>
#include<vector>
#include<iomanip>
#include <string>
#include "Client.h"
#include "Repository.h"
#include "Validator.h"

#include "ListScreen.h"
#include "AddScreen.h"
#include "DeleteScreen.h"
#include "UpdateScreen.h"`
#include"FindScreen.h"`

#include "UI.h"

class MainMenuScreen : public UI 
{
public :
    // abstarct functions for classes under MainMenuScreen
    virtual void _GetBackToMenu(const char* Message = nullptr) = 0;
    virtual void _ClearScreen() = 0;
    virtual void _Message(const char *Message = nullptr) = 0;
private :

	enum MainMenuComponents { List = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Transactions = 6, ManageUser = 7, Logout = 8 };
	Repository& m_RepositoryReference;

    // exclusive
    static void _ClearMainScreen() {
        system("cls");
    }
    static void _ExitMenu(bool& isInMainMenu, const char* message = "\nLogging Out...")
    {
        std::cout << message << "\n\n";
        
        isInMainMenu = false;
        
    }
    static void _MainMenuMessage(const char*Message) {
        std::cout << '\n' + Message;
    }
     void _MainMenuLayout()
    {

        _ClearMainScreen();
        PrintHeader();
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t\t\tMain Menue\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";
        std::cout << std::setw(37) << std::left << "" << "\t[1] Show Client List.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[2] Add New Client.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[3] Delete Client.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[4] Update Client Info.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[5] Find Client.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[6] Transactions.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[7] Manage Users.\n";
        std::cout << std::setw(37) << std::left << "" << "\t[8] Logout.\n";
        std::cout << std::setw(37) << std::left << "" << "===========================================\n";


    }
	MainMenuComponents _NavigateUser(double from = 1, double to = 8)
	{
         _MainMenuMessage("Choose What do you want to do ? [1 to 8] : ");

		return  (MainMenuComponents) Validator::returnValidatedNumber(from, to);
	}
    

    //universal
   
	void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) {
		
			std::cout << "\t\t\t\t\t______________________________________";

			std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Main Menu"));
            
                if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

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
                ListScreen List(m_RepositoryReference);
                List.Show();
                break;
              
            }
            case MainMenuComponents::Add: 
            {
                AddScreen Add(m_RepositoryReference);
                Add.Show();
                break;
            }
            case MainMenuComponents::Delete: 
            {
                DeleteScreen Delete(m_RepositoryReference);
                Delete.Show();
                break;

            }
            case MainMenuComponents::Update: 
            {
                UpdateScreen Update(m_RepositoryReference);
                Update.Show();
                break;

            }
            case MainMenuComponents::Find: 
            {
                FindScreen Find(m_RepositoryReference);
                Find.Show();
                break;

            } 
            case MainMenuComponents::Logout: 
            {
                _ExitMenu(isInMainMenu);
                break;

            }
            default: //for later enuchoice
            {
                break;

            }
                                       


            }

        } while (isInMainMenu);
        
    }
    

    

public : 

    MainMenuScreen(Repository& Repo) : m_RepositoryReference(Repo) {};


    void Show() override {
        PerformMenu();
    }

};

