#include "Controller.h"
#include <iostream>
#include <thread>
#include <cmath>

int main()
{
    cout << "Starting program...\n";

    Controller controller = Controller();
    
    float radians = 0;
    while (true)
    {
        using namespace std::this_thread;
        sleep_for(chrono::milliseconds(16));
        radians += 2 * 3.14f / 60;

        controller.UpdatePosition(Vector3f(cos(radians), sin(radians), 1));
    }
}