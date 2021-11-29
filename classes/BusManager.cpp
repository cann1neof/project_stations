#include <vector>
#include <random>

class BusRoute{
    private:
        int aboard;
        int way;
        StationNode currStation;
        BusRoute* next;

    public:
        BusRoute(int _aboard, int _way){
            aboard = _aboard;
            way = _way;
            next = nullptr;
        }

        void addNext(BusRoute* br){         
            if(next != nullptr){
                next->addNext(br);
            } else {
                next = br;
            }
        }

        int get_aboard(){
            return aboard;
        }
        
        int get_way(){
            return way;
        }
};

class Bus{
    private:
        std::vector<BusRoute> lane;
        int counter;
        int curr_iter;
        int vec;
        bool drive;
    
    public:
        Bus(std::vector<BusRoute> _lane){
            lane = _lane;
            curr_iter = std::rand() % (lane.size());
            drive = false;
        }

        Bus(std::vector<BusRoute> _lane, int _curr_iter){
            lane = _lane;
            curr_iter = _curr_iter;
            drive = false;
        }

        void move(){
            counter -= 1;
            if(drive){
                if(counter == 0){
                    curr_iter -= 1;
                    counter = lane[curr_iter].aboard;
                }
            }
        }
};

class BusManager{
    private:
        std::vector<Bus> depot;
    
    public:
        void tick(){

        }

        void addBus(){

        }
};