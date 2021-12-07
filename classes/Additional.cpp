#include <string>

struct LaneNode{
    int laneId;
    int from;
    int to;
    int time;
    
    LaneNode(int _from, int _to, int _time, int _laneId){
        from = _from;
        to = _to;
        time = _time;
        laneId = _laneId;
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
