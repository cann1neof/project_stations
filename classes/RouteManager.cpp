#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "./Classes.h"

std::vector<int> RouteManager::getNeighbours(int target){
    std::vector<int> output;
    for(int i = 0; i < storage->getSize(); i++){
        if(storage->getMatrixElement(target, i) != -1 && target != i){
            output.push_back(i);
        }
    }
    return output;
}

Path* RouteManager::generatePath(int from, int to, std::vector<int> d){
    int current = to;
    Path* path = new Path(storage->getStation(to));

    while(current != from){
        std::vector<int> neighbours = getNeighbours(current);
        std::vector<std::pair<int, int>> possibleWays;

        for(int each : neighbours){
            possibleWays.push_back(std::make_pair(d[each], each));
        }
        
        int min = 1000000;
        
        for(auto each : possibleWays){
            if(each.first < min){
                min = each.first;
                current = each.second;
            }
        }
        path->setNext(new Path( storage->getStation(current) ));
    }
    return path;
}

std::vector<int> RouteManager::dijkstra(int s, int f){
    std::vector<int> d(storage->getSize(), 32767);
    f++;
    d[s] = 0;
    std::priority_queue<std::pair<int, int>> q;
    q.push(std::make_pair(0, s));

    while(!q.empty()){
        int len = -q.top().first;
        int v = q.top().second;
        q.pop();
        if(len > d[v]) continue;

        for(int i = 0; i < storage->getSize(); i++){
            int to = i;
            int length = storage->getMatrixElement(v, i);
            if(d[to] > d[v] + length && length >= 0) {
                d[to] = d[v] + length + storage->getStation(v)->aboard;
                q.push(std::make_pair(-d[to], to));
            }
        }
    }

    return d;
}

Path* RouteManager::getPath(int from, int to){
    std::pair<int, int> key = std::make_pair(from, to);
    
    if(storage->getPathCache().count(key) == 0){
        Path* tempPath = generatePath(from, to, storage->getAims()[from]);
        storage->setPathCache(key, tempPath);
    }
    
    return storage->getPathCache()[key];
}

Path* RouteManager::go(int from, int to){
    return getPath(from, to);
}

void RouteManager::addStation(StationNode* st){
    storage->addStation(st);
}

void RouteManager::addLaneNode(LaneNode rt){
    storage->setMatrixElement(rt.from, rt.to, rt.time);
    storage->setMatrixElement(rt.to, rt.from, rt.time);

    
    if(rt.time != 0){
        storage->addStationToLane(rt);
    } else if (rt.time == 0 && rt.from != rt.to){
        storage->addTransfer(rt);
    }
}

void RouteManager::configurateRoutes(){
    for(int i = 0; i < storage->getSize(); i++){
        std::vector<int> d = dijkstra(i, storage->getSize()-1);
        storage->setAimsVector(i, d);
    }
}

Path* RouteManager::generateSubPath(int from, std::pair<int, int> transfer, bool rev=false){
    Path* step;
    int targetLaneId = storage->getLaneIdFromStation(from);
    int laneId1 = storage->getLaneIdFromStation(transfer.first);
    int laneId2 = storage->getLaneIdFromStation(transfer.second);
    
    if(laneId1 == targetLaneId && laneId2 != targetLaneId){
        if(!rev){
            step = go(from, transfer.first);
        } else {
            step = go(transfer.first, from);
        }
    } else if(laneId1 != targetLaneId && laneId2 == targetLaneId){
        if(!rev){
            step = go(from, transfer.second);
        } else {
            step = go(transfer.second, from);
        }
    }
    return step;
}
