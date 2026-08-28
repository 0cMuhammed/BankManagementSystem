#include <iostream>
#include "Service.h"
#include "UI.h"
#include "MainMenuScreen.h"

int main()
{
    Repository repo;
    Service service(repo);

    
    UI* _UI = new MainMenuScreen(service);

    _UI->Start();

    return 0;

}

