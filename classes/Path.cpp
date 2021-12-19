#include <iostream>
#include <sstream>
#include <vector>

void Path::setNext(Path* _next){
    if(next == nullptr){
        next = _next;
    }
    else{
        next->setNext(_next);
    }
}
 
Path* Path::getNext() const{
    return this->next;
}
 
std::string Path::toString() const{
    std::stringstream str;
    Path* current = nullptr;
    Path* nxt = this->getNext();

    if(nxt != nullptr){
        current = nxt;
    }
 
    str << this->curr->name << "<-";

    while(current->getNext() != 0x0 && current->getNext() != nullptr){
        str << current->getCurrentStation()->name << "<-";
        current = current->getNext();
    }
    str << current->getCurrentStation()->name;
 
    std::string _out;
    std::vector<std::string> reverseVector;
    std::string out;
    str >> _out;
    int lastIt = 0;

    for(int i = 1; i < _out.length(); i++){
        if(_out[i] == '-' && _out[i-1] == '<'){
            reverseVector.push_back(_out.substr(lastIt, i-lastIt-1));
            lastIt = i+1;
        }
        if(i == _out.length() - 1){
            reverseVector.push_back(_out.substr(lastIt, i-lastIt+1));
        }
    }

    str.clear();

    for(int i = reverseVector.size()-1; i >= 0; i--){
        str << reverseVector[i];
        if(i != 0){
            str << "->";
        }
    }

    str >> out;
    
    return out;
}

std::vector<int> Path::getStationIds(){
    std::vector<int> output;
    Path* current = this->getNext();
    output.push_back(current->getCurrentStation()->id);

    while(current->getNext() != nullptr){
        output.push_back(current->getCurrentStation()->id);
        current = current->getNext();
    }
    return output;
}

int Path::getTotalAboardTime(){
    int output;
    Path* current = this->getNext();
    // output.push_back(current->getCurrentStation()->id);
    output += current->getCurrentStation()->aboard;

    while(current->getNext() != nullptr){
        output += current->getCurrentStation()->aboard;
        current = current->getNext();
    }
    return output;
}

int Path::getPathLength(){
    int output = 0;
    Path* current = this->getNext();
    output++;

    while(current->getNext() != nullptr){
        output++;
        current = current->getNext();
    }
    return output;
}

StationNode* Path::getStartNode(){
    Path* current = this;
    while(current->getNext() != nullptr){
        current = current->getNext();
    }

    return current->getCurrentStation();
}