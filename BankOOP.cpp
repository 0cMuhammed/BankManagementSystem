#include <iostream>
#include "Repository.h"
#include "Service.h"
#include "Session.h"
#include "LoginScreen.h"
#include "MainMenuScreen.h"


int main()
{

    Repository repository;
    Service service(repository);

    Session session;
 
  
   do {

       session = LoginScreen(service).Login();

       std::unique_ptr<UI> ui = std::make_unique<MainMenuScreen>(service, session);

       ui->StartMainMenu(session);

   } while (!session.isActive());

  
    

   
    return 0;

}

