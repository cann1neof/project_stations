#include <iostream>
#include <sstream>
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

        std::string toString() const{
            std::stringstream str;

            Path* current = this->getNext();
            str << this->getCurrentStation()->name << "<-";

            while(current->getNext() != nullptr){
                str << current->getCurrentStation()->name << "<-";
                current = current->getNext();
            }
            str << current->getCurrentStation()->name;

            std::string out;
            str >> out;
            return out;
        }

        friend std::ostream& operator<<(std::ostream& strm, const Path& p){
            strm << p.toString();
            return strm;
        }
};