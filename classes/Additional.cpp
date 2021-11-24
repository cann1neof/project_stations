#include <string>

struct LaneNode{
    int from;
    int to;
    int time;
    
    LaneNode(int _from, int _to, int _time){
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
