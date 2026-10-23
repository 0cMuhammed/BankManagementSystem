#include <iostream>
#include "Repository.h"
#include "Service.h"
#include "Session.h"
#include "LoginScreen.h"
#include "MainMenuScreen.h"
#include "FileHandler.h"
#include "Terminal.h"   // NEW
#include <vector>

int main()
{
    term::Init();   // NEW: UTF-8 output + color support + black window. Must run BEFORE anything is printed.


    Repository repository;
    Service service(repository);

    Session session;
    session.SetIsActive(true);

 
      while (true) {
          
       session = LoginScreen(service).Login();
       std::unique_ptr<UI> ui = std::make_unique<MainMenuScreen>(service, session);

       ui->StartMainMenu(session);

     } 
    
 
  

   
    return 0;

}

