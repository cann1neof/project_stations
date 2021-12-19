#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>

#include "./classes/Classes.h"

#include "./classes/Path.cpp"
#include "./classes/Storage.cpp"
#include "./classes/BusManager.cpp"
#include "./classes/RouteManager.cpp"
#include "./classes/ManagerInstances.cpp"
#include "./classes/CFEReader.cpp"

using namespace std;

const unsigned int busUpdateDelay = 1000000;

int main() {
    srand(time(NULL));
    CFEReader reader("./example.cfe");
    ManagerInstances instances = reader.read();
    
    string str;
    while(1){
        cin >> str;
        if(str == "y"){
            system("clear");
            int from, to;
            cout << "Откуда и куда Вы хотите поехать? ";
            cin >> from >> to;
            instances.go(from, to);   
        }
        else{
            system("clear");
            instances.busManager->tick();
        }
    }
}