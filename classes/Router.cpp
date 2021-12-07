#include <iostream>
#include <vector>
#include <cmath>
#include <string>

class Router{
    private:
        int size;
        std::vector<std::vector<int>> matrix;
        std::vector<std::vector<int>> aims;
        std::vector<StationNode> stations;
        std::map<std::pair<int, int>, Path*> PathCache;

        std::vector<int> getNeighbours(int target){
            std::vector<int> output;
            for(int i = 0; i < size; i++){
                if(matrix[target][i] != -1 && target != i){
                    output.push_back(i);
                }
            }
            return output;
        }

        Path generatePath(int from, int to, std::vector<int> d){
            int current = to;
            Path path(&stations[to]);

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
                path.setNext(new Path(&stations[current]));
            }
            return path;
        }

        std::vector<int> dijkstra(int s, int f){
            std::vector<int> d(size, 32767);
            f++;
            d[s] = 0;
            std::priority_queue<std::pair<int, int>> q;
            q.push(std::make_pair(0, s));

            while(!q.empty()){
                int len = -q.top().first;
                int v = q.top().second;
                q.pop();
                if(len > d[v]) continue;

                for(int i = 0; i < size; i++){
                    int to = i;
                    int length = matrix[v][i];
                    if(d[to] > d[v] + length && length >= 0) {
                        d[to] = d[v] + length + stations[v].aboard;
                        q.push(std::make_pair(-d[to], to));
                    }
                }
            }

            return d;
        }

        Path* getPath(int from, int to){
            std::pair<int, int> key = std::make_pair(from, to);
           
            if(PathCache.count(key) == 0){
                Path tempPath = generatePath(from, to, aims[from]);
                PathCache[key] = &tempPath;
            }
            
            return PathCache[key];
        }

    public:
        Router(int _size){
            size = _size;
            matrix = std::vector<std::vector<int>> (size, std::vector<int>(size, -1));
            aims = std::vector<std::vector<int>> (size, std::vector<int>(size, 0));
        }

        Router(){
            size = 0;
        }

        Path* go(int from, int to){
            return getPath(from, to);
        }

        void addStation(StationNode st){
            stations.push_back(st);
        }

        void addLaneNode(LaneNode rt){
            matrix[rt.from][rt.to] = rt.time;
            matrix[rt.to][rt.from] = rt.time;
        }

        void configurateRoutes(){
            for(int i = 0; i < size; i++){
                std::vector<int> d = dijkstra(i, size-1);
                aims[i] = d;
            }
        }

        friend std::ostream& operator<<(std::ostream& strm, const Router& p)
        {
            int s = p.get_size();
            for(int i = 0; i < s; i++){
                for(int j = 0; j < s; j++){
                    if(i==j){
                        strm << "# ";
                    } else {
                        strm << (p.get_matrixElement(i, j) == -1 ? "*" : std::to_string(p.get_matrixElement(i, j))) << " ";
                    }
                }
                strm << "\n";
            }
            return strm;
        }
        
        StationNode get_staion(int i) const {return stations[i]; }

        int get_size() const { return size; }

        int get_matrixElement(int i, int j) const { 
            if(i < size && j < size){
                return matrix[i][j]; 
            }
            return 0;
        }

        std::vector<std::vector<int>> get_matrix(){
            return matrix;
        }

};