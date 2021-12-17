#include <iostream>
#include <string>
#include <fstream>
#include <vector>

std::pair<int, int> CFEReader::parseBusList(std::string str){
    std::string idSub = str.substr(str.find("#")+1, str.find(" ") - (str.find("#") + 1));
    
    int amountStart = str.find("amount=\"") + 8;
    int amountEnd = str.substr(amountStart, str.length()).find("\"");
    std::string amountSub = str.substr(amountStart,  amountEnd);
    
    int id = std::atoi(idSub.c_str());
    int amount = std::atoi(amountSub.c_str());

    return std::make_pair(amount, id);
}

int CFEReader::search(std::string aim){
    open();
    if(fin){
        std::string str;
        int i = 0;
        while(getline(fin, str)){
            if(str == aim){
                close();
                return i;
            }
            i++;
            str = "";
        }
        close();
        return -2;
    }
    close();
    return -1;
}

void CFEReader::createBuses(){
    std::string str;
    int i = 0;
    open();
    
    while(getline(fin, str)){
        if(i > busAnchor && i < endAnchor){
            std::pair<int, int> kek = parseBusList(str);
            busManager->addNBus(kek);
        } else if(i >= endAnchor){ 
            break; 
        }
        i++;
        str = "";
    }
    
    close();
}

void CFEReader::createStations(){
    std::string str;
    int i = 0;
    bool flag = routeAnchor > stationAnchor;
    open();
    
    while(getline(fin, str)){
        if(flag){
            if(i > stationAnchor && i < routeAnchor){
                StationNode* st = parseStationNode(str);
                routeManager->addStation(st);
            } else if(i >= routeAnchor){ 
                break; 
            }
        } else {
            if(i > stationAnchor && i < endAnchor){
                StationNode* st = parseStationNode(str);
                routeManager->addStation(st);
            } else if(i >= endAnchor){ 
                break; 
            }
        }

        i++;
        str = "";
    }
    
    close();
}

void CFEReader::createRoutes(){
    std::string str;
    int i = 0;
    bool flag = routeAnchor > stationAnchor;
    open();
    
    while(getline(fin, str)){
        if(flag){
            if(i > routeAnchor && i < endAnchor){
                LaneNode rt = parseLaneNode(str);
                routeManager->addLaneNode(rt);
            } else if(i >= endAnchor){ 
                break; 
            }
        } else {
            if(i > routeAnchor && i < stationAnchor){
                LaneNode rt = parseLaneNode(str);
                routeManager->addLaneNode(rt);
            } else if(i >= stationAnchor){ 
                break; 
            }
        }

        i++;
        str = "";
    }
    
    close();
}



StationNode* CFEReader::parseStationNode(std::string str){
    std::string idSub = str.substr(str.find("#") + 1, str.find(" "));
    
    int aboardStart = str.find("aboard=\"") + 8;
    int aboardEnd = str.substr(aboardStart, str.length()).find("\"");
    std::string aboardSub = str.substr(aboardStart,  aboardEnd);

    int nameStart = str.find("name=\"") + 6;
    int nameEnd = str.substr(nameStart, str.length()).find("\"");
    std::string nameSub = str.substr( nameStart,  nameEnd);
            
    
    int id = std::atoi(idSub.c_str());
    int aboard = std::atoi(aboardSub.c_str());
    std::string name = nameSub;

    return new StationNode(id, aboard, name);
}

LaneNode CFEReader::parseLaneNode(std::string str){
    std::string idSub = str.substr(str.find("#") + 1, str.find(" "));
    
    int fromStart = str.find("from=\"") + 6;
    int fromEnd = str.substr(fromStart, str.length()).find("\"");
    std::string fromSub = str.substr(fromStart,  fromEnd);
    
    int toStart = str.find("to=\"") + 4;
    int toEnd = str.substr(toStart, str.length()).find("\"");
    std::string toSub = str.substr(toStart,  toEnd);

    int timeStart = str.find("time=\"") + 6;
    int timeEnd = str.substr(timeStart, str.length()).find("\"");
    std::string timeSub = str.substr(timeStart,  timeEnd);

    int from = atoi(fromSub.c_str());
    int to = atoi(toSub.c_str());
    int time = atoi(timeSub.c_str());
    int laneId = atoi(idSub.c_str());

    return LaneNode(from, to, time, laneId);
}

void CFEReader::open(){
    fin.open(path);
}

void CFEReader::close(){
    fin.close();
}

ManagerInstances CFEReader::read(){
    createStations();
    createRoutes();
    routeManager->configurateRoutes();
    createBuses();
    
    return ManagerInstances(storage, busManager, routeManager);
}
