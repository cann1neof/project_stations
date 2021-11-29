#include <algorithm>
#include <vector>
#include <random>

class Bus{
    private: 
        int vec;
        int distance;
        int aboard;
        int station;
        int laneId;
        bool onStation;

    public:
        Bus(int _station, int _vec, int _laneId, int _distance){
            station = _station;
            vec = _vec;
            laneId = _laneId;
            distance = _distance;
            onStation = true;
        }

        int get_laneId(){ return laneId; }

        int get_station(){ return station; }

        bool get_onStation(){ return onStation; }

        int get_vec(){ return vec; }
        
        int get_aboard(){ return aboard; }
        
        int get_distance(){ return distance; }

        int tick(){
            if(onStation){
                aboard--;
                if(aboard == 0){
                    onStation = false;
                    return -1;
                }
            } else {
                distance--;
                if(distance == 0){
                    onStation = true;
                    return 0;
                }
            }
        }

        void setNewStation(int _station, int _distance, int _aboard){
            station = _station;
            distance = _distance;
            aboard = _aboard;
        }

        void switchvec(){
            vec *= -1;
        }

};

class BusManager{
    private:
        std::vector<Bus*> depot;
        std::vector<std::vector<StationNode*>> lanes;
        std::vector<std::vector<int>> matrix;
        void setNextStation(Bus* b){
            int lane = b->get_laneId();
            int curr = b->get_station();
            int max = lanes[lane].size();

            if(curr == 0 || curr == max - 1){
                b->switchvec();
            }
            int vec = b->get_vec();
            StationNode* newStation = lanes[lane][curr + vec];

            b->setNewStation(curr+vec, matrix[curr][curr+vec], newStation->aboard);
        }

    public:
        BusManager(std::vector<std::vector<int>> _matrix){
            matrix = _matrix;
        }

        void addStation(int laneId, StationNode* st){
            lanes[laneId].push_back(st);
        }
        void addNBus(int n, int laneId){
            for(int i = 0; i < n; i++){
                addBus(laneId);
            }
        }

        void addBus(int laneId){
            int randStation = rand() % lanes[laneId].size();
            int randVec = rand() % 2 - 1;

            if(randStation == 0){
                randVec = 1;
            }
            if(randStation == lanes[laneId].size() - 1){
                randVec = -1;
            }

            Bus* b = new Bus(randStation, randVec, laneId, matrix[randStation][randStation + randVec]);
            depot.push_back(b);
        }

        void go(){
            for(Bus* each: depot){
                int out = each->tick();
                if(out == -1){
                    setNextStation(each);
                }
            }
        }
};