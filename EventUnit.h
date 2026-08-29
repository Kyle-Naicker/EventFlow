#ifndef EVENTFLOW_EVENTUNIT_H
#define EVENTFLOW_EVENTUNIT_H
#include "EventComponent.h"
#include "Observer.h"
#include "Notice.h"



class EventUnit:publuc EventComponent,public Observer{
    public:
        EventUnit(const std::string& name, int capacity, bool indoors);
        ~EventUnit() override{};

        void open() override;
        void close() override;
        void reportStatus(int index=0) const override;
        int getCapacity() const override;

        void update(const Notice& notice) override;

        bool isPaused() const{
            return isPaused_;
        }

        bool isIndoors() const{
            return indoors_;
        }

    protected:
        virtual void reactToNotice(const Notice& notice)=0;

        virtual std::string statusLine() const;

        bool indoors_;
        bool isPaused=false;
        int capacity_=0;
}
#endif