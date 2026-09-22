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
    session.SetIsActive(true);

 
      while (true) {
          
       session = LoginScreen(service).Login();

      

       std::unique_ptr<UI> ui = std::make_unique<MainMenuScreen>(service, session);

       ui->StartMainMenu(session);

     } 
  
    

   
    return 0;

}

