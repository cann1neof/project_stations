#include <iostream>
#include <vector>
#include <cmath>
#include <string>

struct RouteNode{
    int from;
    int to;
    int time;
    
    RouteNode(int _from, int _to, int _time){
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

// подграфы в графе

class Router{
    private:
        int size;
        std::vector<std::vector<int>> matrix;
        std::vector<std::vector<int>> aims;
        std::vector<StationNode> stations;

        std::vector<int> getNeighbours(int target){
            std::vector<int> output;
            for(int i = 0; i < size; i++){
                if(matrix[target][i] != -1 && target != i){
                    output.push_back(i);
                }
            }
            return output;
        }

        void showRoute(int from, int to, std::vector<int> d){
            int current = to;
            while(current != from){
                std::cout << stations[current].name << " <- ";

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
            }
            std::cout << stations[from].name << "\n";
        }

        std::vector<int> dijkstra(int s, int f){
            std::vector<int> d(size, 32767);
            d[s] = 0;
            std::priority_queue<std::pair<int, int>> q;
            q.push(std::make_pair(0, s));

            while(!q.empty()){
                int len = -q.top().first;
                int v = q.top().second;
                q.pop();
                // std::cout << s << " " << f << " " << v << " " << d[v] << "\n";
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

    public:
        Router(int _size){
            size = _size;
            // инициализируем 3d-вектор
            matrix = std::vector<std::vector<int>> (size, std::vector<int>(size, -1));
            aims = std::vector<std::vector<int>> (size, std::vector<int>(size, 0));
        }

        Router(){
            size = 0;
        }

        void addStation(StationNode st){
            stations.push_back(st);
        }

        void addRouteNode(RouteNode rt){
            matrix[rt.from][rt.to] = rt.time;
            matrix[rt.to][rt.from] = rt.time;
        }

        void configurateRoutes(){
            for(int i = 0; i < size; i++){
                for(int j = 0; j < size; j++){
                    if (i != j){
                        std::vector<int> d = dijkstra(i, j);
                        aims[i][j] = d[j];

                        showRoute(i, j, d);
                    }
                }
            }
            for(int i = 0; i < size; i++){
                for(int j = 0; j < size; j++){
                    std::cout << aims[i][j] << " ";
                }
                std::cout << "\n";
            }

            std::vector<int> d = dijkstra(0, 5);
            std::cout << 0 << ", " << 5 << ": ";
            
            for(int k = 0; k < size; k++){
                std::cout << d[k] << " ";
            }
            
            std::cout << "\n";
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

};