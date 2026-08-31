#include "EventControl.h"
#include "EventComponent.h"
#include <iostream>

void EventControl::issueNotice(const Notice& notice){
    std::cout << "\n   EventControl[" << eventName_ << "] issuing " << noticeTypeName(notice.type) << ": "
    << notice.message << "\n";
    notify(notice);
}

void EventControl::issueDirectNotice(Observer* target, const Notice& notice){
    if(!target) return;
    std::string label = "a specific target";
    if (const EventComponent* asComponent = dynamic_cast<const EventComponent*>(target)){
        label = asComponent->getName();
    }
    std::cout << "\n   " << describeSubject() << " targeting " << label << " directly with "
            << noticeTypeName(notice.type) << ": " << notice.message << "\n";
    target->update(notice);
}

std::string EventControl::describeSubject() const {
    return "EventControl[" + eventName_ + "]";
}