#include <iostream>
#include <bits/stdc++.h>
#include "./classes/Additional.cpp"
#include "./classes/Path.cpp"
#include "./classes/Router.cpp"
#include "./classes/CFEReader.cpp"

using namespace std;

int main() {
    CFEReader reader("./example.cfe");
    Router rt = reader.read();
    rt.go(0,5);
    rt.go(0,5);
    return 0;
}