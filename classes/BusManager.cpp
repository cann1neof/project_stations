#include <algorithm>
#include <vector>
#include <map>
#include <random>
#include <time.h>
#include <stdlib.h>
#include <iostream>

#include "./Classes.h"

// Bus methods

Bus::Bus(int _station, int _stationIndex, int _vec, int _laneId, int _distance, int _aboard){
    station = _station;
    stationIndex = _stationIndex;
    vec = _vec;
    laneId = _laneId;
    distance = _distance;
    aboard = _aboard;
    onStation = true;
}

Bus::Bus(BusState* state){
    vec = state->vec;
    distance = state->distance;
    aboard = state->aboard;
    station = state->station;
    laneId = state->laneId;
    stationIndex = state->stationIndex;
    prevStation = state->prevStation;
    onStation = state->onStation;
}

int Bus::tick(bool verbose=true){
    if(onStation){
        if(verbose) std::cout << "aboard: " << aboard << " " << station << " " << vec <<"\n";
        aboard--;
        if(aboard == 0){
            onStation = false;
            return -1;
        }
    } else {
        if(verbose) std::cout << "distance: " << distance << " " << station << " " << vec  << "\n";
        distance--;
        if(distance == 0){
            onStation = true;
            return 0;
        }
    }
    return 0;
}

void Bus::setNewStation(int _station, int _stationIndex, int _distance, int _aboard){
    prevStation = station;
    station = _station;
    distance = _distance;
    aboard = _aboard;
    stationIndex = _stationIndex;
}

void Bus::switchvec(){
    vec *= -1;
}

BusState* Bus::get_state(){
    return new BusState(vec, distance, aboard, station, laneId, stationIndex, prevStation, onStation);
}

// BusManager methods

void BusManager::setNextStation(Bus* b){
    int lane = b->get_laneId();
    int curr = b->get_stationIndex();
    int max = storage->getLaneSize(lane);
    int vec = b->get_vec();

    if((curr == 0 && vec == -1 )|| (curr == max - 1 && vec == 1)){
        b->switchvec();
        vec *= -1;
    }
    int nextStationId = storage->getStationIdFromLane(lane, curr + vec);
    b->setNewStation(nextStationId, curr + vec,
                    storage->getMatrixElement(curr, curr + vec), 
                    storage->getStation(nextStationId)->aboard
                    );
}
        
void BusManager::addBus(int laneId, int i){
    srand(i + laneId + time(NULL));
    int randStation = rand() % storage->getLaneSize(laneId);
    int randVec = rand() % 100;
    
    if(randVec % 2 == 0){
        randVec = 1;
    }else{
        randVec = -1;
    }

    if(randStation == 0){
        randVec = 1;
    }
    if(randStation == (int)storage->getLaneSize(laneId) - 1){
        randVec = -1;
    }

    int station = storage->getStationIdFromLane(laneId, randStation);
    Bus *b = new Bus(station, randStation, randVec, laneId, 
                    storage->getMatrixElement(station, station + randVec), 
                    storage->getStation(station)->aboard
                    );

    storage->addBus(b);
}
        
std::vector<int> BusManager::getLanesFromPath(Path* p){
    std::vector<int> output;
    for(auto stationId: p->getStationIds()){
        int laneId = storage->getLaneIdFromStation(stationId);

        std::vector<int>::iterator it = std::find(output.begin(), output.end(), laneId);
        
        if(it == output.end()){
            output.push_back(laneId);
        }

    }
    return output;
}

std::vector<std::pair<int, int>> BusManager::getTransfers(Path* p){
    std::vector<std::pair<int, int>> output;
    std::vector<int> ids = p->getStationIds();
    for(int i = 1; i < ids.size(); i++){
        if(ids[i] != ids[i-1]){
            std::pair<int, int> tempopair = std::make_pair(ids[i], ids[i-1]);

            std::vector<std::pair<int, int>>::iterator it = std::find(output.begin(), output.end(), tempopair);
            
            if(it == output.end()){
                output.push_back(tempopair);
            }
        }
    }

    return output;
}

void BusManager::addNBus(int laneId, int amount){
    for(int i = 0; i < amount; i++){
        addBus(laneId, i+1);
    }
}

void BusManager::addNBus(std::pair<int, int> parsedBus){
    for(int i = 0; i < parsedBus.first; i++){
        addBus(parsedBus.second, i+1);
    }
}

void BusManager::tick(){
    for(int i = 0; i < storage->getDepotSize(); i++){
        std::cout << i << ": ";
        int out = storage->getBus(i)->tick();
        if(out == -1){
            setNextStation(storage->getBus(i));
        }
    }
}

std::pair<int, Bus*> BusManager::findClosest(int from, int stateTick, bool verbose=false){
    std::vector<int> distances;
    int laneId = storage->getLaneIdFromStation(from);
    for(int i = 0; i < storage->getDepotSize(); i++){
        Bus* b = storage->getBusState(i, stateTick);
        if(b->get_laneId() == laneId){
            distances.push_back(getDistanceToStation(b, from));
        }
    }
    
    std::vector<int>::iterator minDistanceIt = std::min_element(distances.begin(), distances.end());
    int minDistance = std::distance(distances.begin(), minDistanceIt);

    if(verbose){
        std::cout << distances[minDistance] << "\n";
    }
    
    return std::make_pair(distances[minDistance], storage->getBus(minDistance));
}

std::pair<int, Bus*> BusManager::findClosest(StationNode* from, int stateTick, bool verbose=false){
    return findClosest(from->id, stateTick, verbose);
}

int BusManager::getRouteTime(Path* part, int stateTick=0){
    int totalTime = 0;
    StationNode* start = part->getStartNode();
    StationNode* end = part->getCurrentStation();
    std::pair<int, Bus*> closest = findClosest(start, stateTick);
    totalTime += closest.first;
    totalTime += getDistanceToStation(closest.second, end);
    return totalTime;
}

int BusManager::getNRouteTime(std::vector<Path*> totalPath){
    int totalTime = 0;
    for(auto each: totalPath){
        totalTime += getRouteTime(each, totalTime);
    }
    return totalTime;
}

int BusManager::getDistanceToStation(Bus* b, int target){
    int station = b->get_station();
    int totalTime = storage->getAims()[station][target];
    int delta = 0;
    if(b->get_onStation()){
        delta = b->get_aboard() - storage->getStation(station)->aboard;
    } else {
        delta = b->get_distance();   
    }
    totalTime += delta;
    return totalTime;
}

int BusManager::getDistanceToStation(Bus* b, StationNode* target){
    return getDistanceToStation(b, target->id);
}

int BusManager::getDepotSize(){
    return storage->getDepotSize();
}

Bus* BusManager::getBus(int i){
    return storage->getBus(i);
}