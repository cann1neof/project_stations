#include <vector>


// setters
void Storage::setSize(int _size){
    size = _size;
    matrix = std::vector<std::vector<int>> (size, std::vector<int>(size, -1));
    aims = std::vector<std::vector<int>> (size, std::vector<int>(size, 0));
}

void Storage::setPathCache(std::pair<int, int> key, Path* val){
    PathCache[key] = val;
}

void Storage::setMatrixElement(int from, int to, int val){
    matrix[from][to] = val;
}

void Storage::setAimsVector(int i, std::vector<int> d){
    aims[i] = d;
}

void Storage::addStation(StationNode* st){
    stations.push_back(st);
}

void Storage::addStationToLane(LaneNode rt){
    if(lanes.count(rt.laneId) == 0){
        laneAmount++;
        lanes[rt.laneId].push_back(rt.from);
        lanes[rt.laneId].push_back(rt.to);
    } else {
        lanes[rt.laneId].push_back(rt.to);
    }
}

void Storage::addTransfer(LaneNode rt){
    transferList.push_back(std::make_pair(rt.from, rt.to));
}

void Storage::addBus(Bus* b){
    depot.push_back(b);
}


// getters
int Storage::getSize(){
    return size;
}

int Storage::getMatrixElement(int from, int to){
    return matrix[from][to];
}

StationNode* Storage::getStation(int i){
    return stations[i];
}

std::vector<std::vector<int>> Storage::getAims(){
    return aims;
}
std::map<std::pair<int, int>, Path*> Storage::getPathCache(){
    return PathCache;
}

int Storage::getLaneSize(int laneId){
    return lanes[laneId].size();
}

int Storage::getStationIdFromLane(int laneId, int i){
    return lanes[laneId][i];
}

int Storage::getDepotSize(){
    return (int)depot.size();
}
Bus* Storage::getBus(int i){
    return depot[i];
}

int Storage::getLaneIdFromStation(int stationId){
    for(int i = 0; i < laneAmount; i++){
        std::vector<int>::iterator it = std::find(lanes[i].begin(), lanes[i].end(), stationId);
        if(it != lanes[i].end()){
            return i;
        }
    }
    std::cout << "FATAL ERROR: NO LANE\n";
    return -1;
}

std::vector<int> Storage::getLane(int laneId){
    return lanes[laneId];
}

std::vector<std::pair<int, int>> Storage::getTransferList(){
    return transferList;
}

void Storage::setNextStation(Bus* b){
    int lane = b->get_laneId();
    int curr = b->get_stationIndex();
    int max = getLaneSize(lane);
    int vec = b->get_vec();

    if((curr == 0 && vec == -1 )|| (curr == max - 1 && vec == 1)){
        b->switchvec();
        vec *= -1;
    }
    int nextStationId = getStationIdFromLane(lane, curr + vec);
    b->setNewStation(nextStationId, curr + vec,
                    getMatrixElement(curr, curr + vec), 
                    getStation(nextStationId)->aboard
                    );
}

Bus* Storage::getBusState(int i, int stateTick){
    if(stateTick > 0){
        Bus* targetBus = getBus(i);
        BusState* initialState = targetBus->get_state();
        Bus* currentBus = new Bus(initialState);

        for(int i = 0; i < stateTick; i++){
            int out = currentBus->tick(false);
            if(out == -1){
                setNextStation(currentBus);
            }
        }
        return currentBus;
    }
    return getBus(i);
}