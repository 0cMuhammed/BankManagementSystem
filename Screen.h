#pragma once
#include "UI.h"
#include "Service.h"

class Screen : public UI {
    // abstract class : cannot be instatated 

    private :

    Service& m_ServiceRef;

    protected : // only the subclasses under Screen ineherits these members internally and the objects of the subclasses cannot call these methods directly, only used in the implementation.

       
        void _ClearScreen() // fixed 
        { 
            system("cls"); 
        }
        virtual void _GetBackToMenu(const char* Message = nullptr) 
        { 
            std::cout << '\n' + std::string( (((Message != nullptr) ? Message : "Press Enter to go back to Main Menu"))) + ".....\n";

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
        virtual void _Message(const char* Message = nullptr) 
        {
            std::cout << '\n' + std::string(Message);
        }


    public:

       Screen(Service& ref) : m_ServiceRef(ref) {}

        
};


