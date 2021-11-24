#include <iostream>
#include <string>
#include <fstream>
#include <vector>


class CFEReader{
    private:
        std::string path;
        std::ifstream fin;
        int stationAnchor;
        int routeAnchor;
        int changeAnchor;
        int endAnchor;
        Router router;

        int search(std::string aim){
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

        void createStations(){
            std::string str;
            int i = 0;
            bool flag = routeAnchor > stationAnchor;
            open();
            
            while(getline(fin, str)){
                if(flag){
                    if(i > stationAnchor && i < routeAnchor){
                        StationNode st = parseStationNode(str);
                        router.addStation(st);

                    } else if(i >= routeAnchor){ 
                        break; 
                    }
                } else {
                    if(i > stationAnchor && i < endAnchor){
                        StationNode st = parseStationNode(str);
                        router.addStation(st);

                    } else if(i >= endAnchor){ 
                        break; 
                    }
                }

                i++;
                str = "";
            }
            
            close();
        }

        void createRoutes(){
            std::string str;
            int i = 0;
            bool flag = routeAnchor > stationAnchor;
            open();
            
            while(getline(fin, str)){
                if(flag){
                    if(i > routeAnchor && i < endAnchor){
                        LaneNode rt = parseLaneNode(str);
                        router.addLaneNode(rt);
                    } else if(i >= endAnchor){ 
                        break; 
                    }
                } else {
                    if(i > routeAnchor && i < stationAnchor){
                        LaneNode rt = parseLaneNode(str);
                        router.addLaneNode(rt);
                    } else if(i >= stationAnchor){ 
                        break; 
                    }
                }

                i++;
                str = "";
            }
            
            close();
        }

        StationNode parseStationNode(std::string str){
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

            return StationNode(id, aboard, name);
        }

        LaneNode parseLaneNode(std::string str){
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

            return LaneNode(from, to, time);
        }

        void open(){
            fin.open(path);
        }

        void close(){
            fin.close();
        }

    public:
        CFEReader(std::string _path){
            path = _path;
            stationAnchor = search("STATIONLIST");
            routeAnchor = search("ROUTELIST");
            endAnchor = search("END");

            int stationAmount = (stationAnchor < routeAnchor ? routeAnchor : endAnchor) - stationAnchor - 1;

            router = Router(stationAmount);
        }

        Router read(){
            createStations();
            createRoutes();
            router.configurateRoutes();

            return router;
        }
        
};