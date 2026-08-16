#include <iostream>
#include "Repository.h"
#include "UI.h"
int main()
{
    int a = 5;
    int& ref = a;

    ref = 2;
    std::cout << ref;

  
    return 0;

}

