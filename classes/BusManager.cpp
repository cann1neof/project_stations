#include <algorithm>
#include <vector>
#include <map>
#include <random>
#include <time.h>
#include <stdlib.h>
#include <iostream>

class Bus{
    private: 
        int vec;
        int distance;
        int aboard;
        int station;
        int laneId;
        int stationIndex;

        bool onStation;

    public:
        Bus(int _station, int _stationIndex, int _vec, int _laneId, int _distance, int _aboard){
            station = _station;
            stationIndex = _stationIndex;
            vec = _vec;
            laneId = _laneId;
            distance = _distance;
            aboard = _aboard;
            onStation = true;
        }

        int get_laneId() const { return laneId; }

        int get_station() const { return station; }
        
        int get_stationIndex() const { return stationIndex; }

        bool get_onStation() const { return onStation; }

        int get_vec() const { return vec; }
        
        int get_aboard() const { return aboard; }
        
        int get_distance() const { return distance; }

        int tick(){
            if(onStation){
                std::cout << "aboard: " << aboard << "\n";
                aboard--;
                if(aboard == 0){
                    onStation = false;
                    return -1;
                }
            } else {
                std::cout << "distance: " << distance << "\n";
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

        friend std::ostream& operator<<(std::ostream& strm, const Bus *b){
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

class BusManager{
    private:
        int size;
        int laneAmount;
        std::vector<Bus*> depot;
        std::map<int, std::vector<int>> lanes;
        std::vector<std::vector<int>> matrix;
        std::vector<StationNode> stations;

        void setNextStation(Bus* b){
            int lane = b->get_laneId();
            int curr = b->get_stationIndex();
            int max = lanes[lane].size();
            int vec = b->get_vec();

            if((curr == 0 && vec == -1 )|| (curr == max - 1 && vec == 1)){
                b->switchvec();
                vec *= -1;
            }
            int nextStationId = lanes[lane][curr + vec];

            b->setNewStation(curr+vec, matrix[curr][curr+vec], stations[nextStationId].aboard);
        }

        int getStationLaneId(int id){
            for(int i = 0; i < laneAmount; i++){
                std::vector<int>::iterator it = std::find(lanes[i].begin(), lanes[i].end(), id);
                if(it != lanes[i].end()){
                    return i;
                }
            }
            std::cout << "FATAL ERROR: NO LANE\n";
            return -1;
        }
        
        void addBus(int laneId, int i){
            srand(i + laneId + time(NULL));
            int randStation = rand() % lanes[laneId].size();
            int randVec = rand() % 100;
            std::cout << randStation << " " << randVec << "\n";
            if(randVec % 2 == 0){
                randVec = 1;
            }else{
                randVec = -1;
            }

            if(randStation == 0){
                randVec = 1;
            }
            if(randStation == (int)lanes[laneId].size() - 1){
                randVec = -1;
            }

            int station = lanes[laneId][randStation];

            std::cout << "\n";
            Bus *b = new Bus(station, randStation, randVec, laneId, matrix[station][station + randVec], stations[station].aboard);
            depot.push_back(b);
        }

    public:
        BusManager(int _size){
            size = _size;
            laneAmount = 0;
        }
        
        BusManager(){
            size = 0;
            laneAmount = 0;
        }       

        void addLaneNode(LaneNode rt){
            if(lanes.count(rt.laneId) == 0){
                laneAmount++;
                lanes[rt.laneId].push_back(rt.from);
                lanes[rt.laneId].push_back(rt.to);
            } else {
                lanes[rt.laneId].push_back(rt.to);
            }
        }

        void setMatrix(std::vector<std::vector<int>> _matrix){
            matrix = _matrix;
        }

        void addStation(StationNode st){
            stations.push_back(st);
        }

        void addNBus(int laneId, int amount){
            for(int i = 0; i < amount; i++){
                addBus(laneId, i+1);
            }
        }

        void addNBus(std::pair<int, int> parsedBus){
            for(int i = 0; i < parsedBus.first; i++){
                addBus(parsedBus.second, i+1);
            }
        }

        std::vector<int> getLanesFromPath(Path* p){
            std::vector<int> output;
            for(auto stationId: p->getStationIds()){
                int laneId = getStationLaneId(stationId);

                std::vector<int>::iterator it = std::find(output.begin(), output.end(), laneId);
                
                if(it == output.end()){
                    output.push_back(laneId);
                }

            }
            return output;
        }

        void tick(){
            for(int i = 0; i < depot.size(); i++){
                std::cout << i << ": ";
                int out = depot[i]->tick();
                if(out == -1){
                    setNextStation(depot[i]);
                }
            }
        }
};