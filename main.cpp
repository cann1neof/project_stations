#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>

#include "./classes/Additional.cpp"
#include "./classes/Path.cpp"
#include "./classes/BusManager.cpp"
#include "./classes/Router.cpp"
#include "./classes/CFEReader.cpp"

using namespace std;

const unsigned int busUpdateDelay = 1000000;

int main() {
    srand(time(NULL));
    CFEReader reader("./example.cfe");
    Router rt = reader.read();
    BusManager bm = reader.getBusManager();
    // Path* p = rt.go(0, 5);
    // vector<int> v = bm.getLanesFromPath(p);
    // for(auto each: v){
    //     cout << each << " ";
    // }
    // cout << endl;
    while(true){
        bm.tick();
        usleep(busUpdateDelay);
    }
    
}