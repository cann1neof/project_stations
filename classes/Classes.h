#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#ifndef _Classes_h_
#define _Classes_h_

struct LaneNode{
    int laneId;
    int from;
    int to;
    int time;
    
    LaneNode(int _from, int _to, int _time, int _laneId){
        laneId = _laneId;
        from = _from;
        to = _to;
        time = _time;
    }
};

struct StationNode{
    int id;
    int aboard;
    std::string name;
    
    StationNode(int _id, int _aboard, std::string _name){
        id = _id;
        aboard = _aboard;
        name = _name;
    }
};

struct BusState{
    int vec;
    int distance;
    int aboard;
    int station;
    int laneId;
    int stationIndex;
    int prevStation;
    bool onStation;

    BusState(int _vec, int _distance, int _aboard, int _station, int _laneId, int _stationIndex, int _prevStation, bool _onStation){
        vec = _vec;
        distance = _distance;
        aboard = _aboard;
        station = _station;
        laneId = _laneId;
        stationIndex = _stationIndex;
        prevStation = _prevStation;
        onStation = _onStation;
    }
};

class Bus{
    private: 
        int vec;
        int distance;
        int aboard;
        int station;
        int laneId;
        int stationIndex;
        int prevStation;
        bool onStation;

    public:
        Bus(int _station, int _stationIndex, int _vec, int _laneId, int _distance, int _aboard);
        Bus(BusState* state);
        int get_laneId() const { return laneId; }
        int get_station() const { return station; }
        int get_stationIndex() const { return stationIndex; }
        bool get_onStation() const { return onStation; }
        int get_vec() const { return vec; }
        int get_aboard() const { return aboard; }
        int get_distance() const { return distance; }
        int get_prevStation() const { return prevStation; }
        BusState* get_state();
        int tick(bool verbose);
        void setNewStation(int _station, int _stationIndex, int _distance, int _aboard);
        void switchvec();
        
        friend std::ostream& operator<<(std::ostream& strm, const Bus *b){
            strm << "#";
            strm << b->get_laneId();
            strm << " station: ";
            strm << b->get_station();
            strm << " vec: ";
            strm << b->get_vec();
            strm << " dist: ";
            strm << b->get_distance();
            strm << " aboard: ";
            strm << b->get_aboard();
            strm << "\n";

            return strm;
        }

};

class Path{
    private:
        StationNode* curr;
        Path* next;
    
    public:
        Path(StationNode* _curr){
            curr = _curr;
            next = nullptr;
        }
        
        void setNext(Path* _next);
        Path* getNext() const;
        
        StationNode* getCurrentStation(){
            return curr;
        }

        std::string toString() const;
        
        friend std::ostream& operator<<(std::ostream& strm, const Path& p){
            strm << p.toString();
            return strm;
        }
        friend std::ostream& operator<<(std::ostream& strm, const Path* p){
            strm << p->toString();
            return strm;
        }
        
        std::vector<int> getStationIds();
        int getTotalAboardTime();
        int getPathLength();
        StationNode* getStartNode();
};

class Storage{
    private:
        std::vector<Bus*> depot;
        std::vector<std::vector<int>> matrix;
        std::vector<std::vector<int>> aims;
        std::vector<StationNode*> stations;
        std::map<int, std::vector<int>> lanes;
        std::map<std::pair<int, int>, Path*> PathCache;
        std::vector<std::pair<int, int>> transferList;

        int laneAmount;
        int size;

        void setNextStation(Bus* b);
    public:
        // setters
        void setSize(int _size);
        void setPathCache(std::pair<int, int> key, Path* val);
        void setMatrixElement(int from, int to, int val);
        void setAimsVector(int i, std::vector<int> d);
        
        void addStation(StationNode* st);
        void addStationToLane(LaneNode st);
        void addTransfer(LaneNode st);
        void addBus(Bus* b);

        // getters
        int getSize();
        int getMatrixElement(int from, int to);
        StationNode* getStation(int i);
        std::vector<std::vector<int>> getAims();
        std::map<std::pair<int, int>, Path*> getPathCache();
        int getLaneSize(int laneId);
        int getStationIdFromLane(int laneId, int i);
        int getDepotSize();
        Bus* getBus(int i);
        int getLaneIdFromStation(int stationId);
        std::vector<int> getLane(int laneId);
        std::vector<std::pair<int, int>> getTransferList();
        Bus* getBusState(int i, int stateTick);
};

class BusManager{
    private:
        Storage* storage;

        void setNextStation(Bus* b);
        void addBus(int laneId, int i);
        
        int getDistanceToStation(Bus* b, int target);
        int getDistanceToStation(Bus* b, StationNode* target);
        Bus* getClosestBus(int from);

    public:        
        BusManager(Storage* _storage){
            storage = _storage;
        }
        std::vector<int> getLanesFromPath(Path* p);
        std::vector<std::pair<int, int>> getTransfers(Path* p);

        void addNBus(int laneId, int amount);
        void addNBus(std::pair<int, int> parsedBus);
        void tick();
        void go(int from, int to);
        std::pair<int, Bus*> findClosest(int from, int stateTick, bool verbose);
        std::pair<int, Bus*> findClosest(StationNode* from, int stateTick, bool verbose);

        int getDepotSize();
        Bus* getBus(int i);
        int getRouteTime(Path* part, int stateTick);
        int getNRouteTime(std::vector<Path*> totalPath);
};

class RouteManager{
    private:
        Storage* storage;

        std::vector<int> getNeighbours(int target);
        Path* generatePath(int from, int to, std::vector<int> d);
        std::vector<int> dijkstra(int s, int f);
        Path* getPath(int from, int to);

    public:
        RouteManager(Storage* _storage, int _size){
            storage = _storage;
            storage->setSize(_size);
        }

        Path* go(int from, int to);
        void addStation(StationNode* st);
        void addLaneNode(LaneNode rt);
        void configurateRoutes(); 
        Path* generateSubPath(int from, std::pair<int, int> transfer, bool rev);
};

class ManagerInstances{
    public:
        Storage* storage;
        BusManager *busManager;
        RouteManager *routeManager;

        ManagerInstances(Storage *storage, BusManager* _bm, RouteManager* _rt){
            busManager = _bm;
            routeManager = _rt;
        }

        void go(int from, int to);

};

class CFEReader{
    private:
        std::string path;
        std::ifstream fin;
        int stationAnchor;
        int routeAnchor;
        int changeAnchor;
        int busAnchor;
        int endAnchor;
        RouteManager* routeManager;
        BusManager* busManager;
        Storage* storage;

        std::pair<int, int> parseBusList(std::string str);
        int search(std::string aim);
        void createBuses();
        void createStations();
        void createRoutes();
        StationNode* parseStationNode(std::string str);
        LaneNode parseLaneNode(std::string str);
        void open();
        void close();
    public:
        CFEReader(std::string _path){
            storage = new Storage();
            path = _path;
            stationAnchor = search("STATIONLIST");
            routeAnchor = search("ROUTELIST");
            busAnchor = search("BUSLIST");
            endAnchor = search("END");

            int stationAmount = routeAnchor - stationAnchor - 1;
            routeManager = new RouteManager(storage, stationAmount);
            busManager = new BusManager(storage);
        }
        
        ManagerInstances read();
};
#endif
