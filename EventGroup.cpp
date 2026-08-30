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

EventComponent* EventGroup::removeChild(EventComponent* child) {
    // Manual linear search, find the index of the
    // pointer we were asked to remove.
    std::size_t foundIndex=children_.size();
    for (std::size_t i=0;i<children_.size();++i) {
        if (children_[i]==child) {
            foundIndex=i;
            break;
        }
    }
    if(foundIndex==children_.size()) {
        return nullptr; // Not found among this group's children.
    }

    Observer* asObserver=dynamic_cast<Observer*>(child);
    if(asObserver) {
        detach(asObserver);
    }

    // Manual erase-by-index.
   
    children_.erase(children_.begin()+static_cast<long>(foundIndex));
    return child; 
}

void EventGroup::update(const Notice& notice) {
    reactToNotice(notice);
    // Cascade to our own observers (our children). This is what lets a
    // notice travel EventControl -> root EventGroup -> child EventGroup
    // -> leaf EventUnit 
    notify(notice);
}

void EventGroup::reactToNotice(const Notice& notice) {
    switch(notice.type) {
        case NoticeType::OPEN:
            isOpen_=true;
            break;
        case NoticeType::CLOSE:
        case NoticeType::EVACUATE:
        case NoticeType::CANCEL:
            isOpen_=false;
            break;
        default:
            break; 
    }
     std::cout << "[" << describeSubject() << "] received " << noticeTypeName(notice.type)
              << " (" << notice.message << "), cascading to " << observerCount()
              << " observer(s)\n";
}

std::string EventGroup::describeSubject() const {
    return "EventGroup["+name_+"]";
}