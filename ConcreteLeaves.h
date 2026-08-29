#ifndef EVENTFLOW_CONCRETELEAVES_H
#define EVENTFLOW_CONCRETELEAVES_H

#include "EventUnit.h"


class LectureHall:public EventUnit{
    public:
        LectureHall(const std::string& name, int capacity, bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
};

class WorkshopRoom: public EventUnit{
    public:
        WorkshopRoom(const std::string& name, int capacity, bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
};

class RegistrationDesk: public EventUnit{
    public:
        RegistrationDesk(const std::string& name, int capacity,bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
};

class ExhibitonBooth: public EventUnit{
    public:
        ExhibitonBooth(const std::string& name, int capacity,bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
};

class CateringStation: public EventUnit{
    public:
        CateringStation(const std::string& name, int capacity,bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
    
    private:
        bool coldMenuOnly_=false;
};


class ShuttleStop:public EventUnit{
    public:
        ShuttleStop(const std::string& name, int capacity, bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
    private:
        std::string route_="Main Campus Loop";
    };

class MedicalStation:public EventUnit{
    public:
        LectureHall(const std::string& name, int capacity, bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
};

#endif