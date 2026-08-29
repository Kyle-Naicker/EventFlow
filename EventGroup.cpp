#include "EventGroup.h"
#include <iostream>

EventGroup::EventGroup(const std::string& name):EventComponent(name){}

EventGroup::~EventGroup(){
    observers_.clear();

    for(EventComponent* child:children_){
        delete child;
    }
    children_.clear();
}

void EventGroup::open(){
    isOpen_=true;
    for(EventComponent* child:children_){
        child->open();
    }
}

void EventGroup::close(){
    isOpen_=false;
    for(EventComponent* child:children_){
        child->close();
    }
}

void EventGroup::reportStatus(int indent) const {
    std::cout<<std::string(static_cast<size_t>(indent)*2,' ')
              <<"+ "<<name_<<" ["<<(isOpen_?"open":"closed")
              <<", total capacity="<<getCapacity()<<"]\n";
    for (const EventComponent* child:children_) {
        child->reportStatus(indent+1);
    }
}

int EventGroup::getCapacity() const {
    int total=0;
    for(const EventComponent* child:children_) {
        total+=child->getCapacity();
    }
    return total;
}

void EventGroup::add(EventComponent* child){
    if(!child)return;

    Observer* asObserver=dynamic_cast<Observer*>(child);
    children_.push_back(child);
    if(asObserver){
        attach(asObserver);
    }
}