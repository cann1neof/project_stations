void ManagerInstances::go(int from, int to){
    Path* globalPath = routeManager->go(from, to);

    std::vector<std::pair<int, int>> transfers = busManager->getTransfers(globalPath);
    
    std::vector<Path*> totalPath;
    
    for(auto each: transfers){
        totalPath.push_back(routeManager->generateSubPath(from, each));
        totalPath.push_back(routeManager->generateSubPath(to, each, true));
    }

    std::cout << "Самый быстрый маршрут:\n"<<globalPath << "\n";

    int totalTime;

    if(totalPath.size() != 0){
        totalTime = busManager->getNRouteTime(totalPath);
    } else {
        totalTime = busManager->getRouteTime(globalPath);
    }

    std::cout << "Время в пути: " << totalTime << "\n";
}