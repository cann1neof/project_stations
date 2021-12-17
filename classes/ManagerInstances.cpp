void ManagerInstances::go(int from, int to){
    Path* p = routeManager->go(from, to);

    std::vector<std::pair<int, int>> transfers = busManager->getTransfers(p);
    
    std::vector<Path*> totalPath;
    
    for(auto each: transfers){
        totalPath.push_back(routeManager->generateSubPath(from, each));
        totalPath.push_back(routeManager->generateSubPath(to, each, true));
    }
    std::cout << p << "\n";
    for(auto each: totalPath){
        std::cout << each << "\n";
    }
    // Bus* closest = busManager->findClosest(from, true);
    // std::cout << closest << "\n";
}