#ifndef EVENTFLOW_EVENTGROUP_H
#define EVENTFLOW_EVENTGROUP_H

#include <vector>
#include "EventComponent.h"
#include "Observer.h"
#include "Subject.h"
#include "Notice.h"

class EventGroup: public EventComponent, public Observer, public Subject{
    public:
        explicit EventGroup(const std::string& name);
        ~EventGroup() override;
        void open() override;
        void close() override;
        void reportStatus(int indent=0) const override;
        int getCapacity() const override;

        void add(EventComponent* child);
        EventComponent* removeChild(EventComponent* child);

        std::size_t childCount() const{
            return children_.size();
        }

        void update(const Notice& notice) override;

    protected:

        virtual void reactToNotice(const Notice& notice) override;
    
    private:
        std::vector<EventComponent*> children_;
}

#endif