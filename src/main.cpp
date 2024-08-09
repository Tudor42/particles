#include "application.h" 
#include <iostream>
int main(void)
{
    Application::getInstance()->run();

    glfwTerminate();

    return 0;
}
