#include "EventControl.h"
#include <iostream>

void EventControl::issueNotice(const Notice& notice){
    std::cout << "\n   EventControl[" << eventName_ << "] issuing " << noticeTypeName(notice.type) << ": "
    << notice.message << "\n";
    notify(notice);
}

std::string EventControl::describeSubject() const {
    return "EventControl[" + eventName_ + "]";
}