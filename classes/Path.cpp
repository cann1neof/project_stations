#include <iostream>

class Path{
    private:
        StationNode* curr;
        Path* next;
    
    public:
        Path(StationNode* _curr){
            curr = _curr;
            next = nullptr;
        }

        void setNext(Path* _next){
            if(next == nullptr){
                next = _next;
            }
            else{
                next->setNext(_next);
            }
        }

        Path* getNext() const{
            return next;
        }

        StationNode* getCurrentStation() const{
            return curr;
        }

        friend std::ostream& operator<<(std::ostream& strm, const Path& p){
            Path* current = p.getNext();
            
            strm << p.getCurrentStation()->name << "<-";

            while(current->getNext() != nullptr){
                strm << current->getCurrentStation()->name << "<-";
                current = current->getNext();
            }
            strm << current->getCurrentStation()->name << "\n";
            return strm;
        }
};