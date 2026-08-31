#include "EventUnit.h"
#include <iostream>

EventUnit::EventUnit(const std::string& name, int capacity, bool indoors,const std::string& startTime, const std::string& endTime):EventComponent(name, startTime, endTime), indoors_(indoors) {
    capacity_ = capacity;
}
void EventUnit::open(){
    isOpen_=true;
}

void EventUnit::close(){
    isOpen_=false;
}

int EventUnit::getCapacity() const{
    return capacity_;
}

std::string EventUnit::statusLine() const{
    if (!isOpen_)return "closed";
    if (isPaused_)return "open but paused";
    return "open and running";
}

void EventUnit::reportStatus(int indent) const {
    std::cout << std::string(static_cast<size_t>(indent) * 2, ' ')
              << "- " << name_ << " [" << statusLine() << ", capacity="
              << capacity_ << (indoors_ ? "" : ", outdoors");
    if (!startTime_.empty() || !endTime_.empty()) {
        std::cout << ", schedule=" << startTime_ << "-" << endTime_;
    }
    if (capacityThreshold_ >= 0) {
        std::cout << ", attendance=" << currentAttendance_ << "/" << capacityThreshold_;
    }
    std::cout << "]\n";
}

void EventUnit::update(const Notice& notice){
    reactToNotice(notice);
}