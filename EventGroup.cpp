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