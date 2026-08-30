#ifndef EVENTCONTROL_H
#define EVENTCONTROL_H

#include <string>
#include "Subject.h"
#include "Notice.h"

class EventControl : public Subject {
    public:
        explicit EventControl(const std::string& eventName) : eventName_(eventName){}
        const std::string& getEventName() const { return eventName_;}
        void issueNotice(const Notice& notice);
    private:
        std::string eventName_;
};

#endif