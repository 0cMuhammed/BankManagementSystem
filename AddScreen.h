#pragma once
#include <iostream>
#include<vector>
#include<iomanip>



#include "Validator.h"
#include "ClientRepository.h"
#include "Screen.h"

class AddScreen : public Screen
{
private :

    ClientRepository& m_RepositoryReference;




    void PrintHeader(const char* ScreenName = nullptr, const char* SubTitle = nullptr) override {


        std::cout << "\t\t\t\t\t______________________________________";

        std::cout << "\n\n\t\t\t\t\t  \t  " << (((ScreenName != nullptr) ? ScreenName : "Add New Client Screen"));

        if (SubTitle != nullptr) { std::cout << "\n\t\t\t\t\t  " << SubTitle; }

        std::cout << "\n\t\t\t\t\t______________________________________\n\n";
    }
    void PerformMenu(const char* Message = nullptr) override {

        bool IsContinueOperation = true;

        do {
            _PerformAdding();
            IsContinueOperation = Validator::GetConfirmation('\n' + std::string( (((Message != nullptr) ? Message : "Do you want to continue this operation?")) ));

        } while (IsContinueOperation);

    }
   

  
    
   

  
    void _Add(Client& New, const std::string &AccountNumber = "Empty") {


        switch (m_RepositoryReference.AddClient(New))
        {

        case ClientRepository::OperationStates::Successful :
        {
            ClientRepository::PrintClient(New);
            std::cout << "Account is saved successfuly!\n";

            break;

        }
        case ClientRepository::OperationStates::AccountNumberAlreadyExists :
        {
            std::cout << "Account Number is Already Used.\n";
            break;

        }
        case ClientRepository::OperationStates::Failed : // for some reason....
        { 
            std::cout << "Operation Failed, Try again Later...\n";
            break;

        }

        default:
        {
            break;

        }
        }
    }
    void _PerformAdding() {

        _ClearScreen();
        PrintHeader();
       

        Client New = m_RepositoryReference.ReadClient();
        
        _Add(New);
    }

   

    public :

        AddScreen(Service& Ref) : Screen(Ref), m_RepositoryReference(Ref.AccessClientServices().AccessRepository()) {};

    void Start() override {
        PerformMenu();
        _GetBackToMenu();
    }
 	

};

