/**
 *  @file ConcreteLeaves.h
 *  @brief Concreate Leaf types for the TechConnect conference. Each type reacts meaningfully differently to the same notices. 
 *  Satisfying task 4.1
 * 
 * 
 * 
 */

#ifndef EVENTFLOW_CONCRETELEAVES_H
#define EVENTFLOW_CONCRETELEAVES_H

#include "EventUnit.h"

/**
 * @brief A keynote/talk room. Pauses talks on PAUSE/EVACUATE, resumes on RESUME and closes entirely on CANCEL or CLOSE.
 * 
 */
class LectureHall:public EventUnit{
    public:
    /**
     * @param name Display the name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     */
        LectureHall(const std::string& name, int capacity, bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
};

/**
 * @brief A hands-on workshop room. Distinct from LectureHall: an
 *        EVACUATE stops the practical activity immediately, but a mere
 *        SCHEDULE_CHANGE causes it to re-plan rather than pause.
 * 
 */
class WorkshopRoom: public EventUnit{
    public:
    /**
     * @param name Display name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     */
        WorkshopRoom(const std::string& name, int capacity, bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
};

/**
 * @brief The attendee registration/check-in desk. Ignores CAPACITY_ALERT
 *        (it is not a room with an occupancy limit) but stops issuing
 *        badges on CLOSE/EVACUATE/CANCEL.
 * 
 */
class RegistrationDesk: public EventUnit{
    public:
     /**
     * @param name Display name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     */
        RegistrationDesk(const std::string& name, int capacity,bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
};
/**
 * @brief A sponsor/exhibition booth. Suspends live demos on PAUSE but, unlike a LectureHall, fully packs down (closes) on CANCEL 
 *        rather than just pausing.
 */
class ExhibitonBooth: public EventUnit{
    public:
     /**
     * @param name Display name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     */
        ExhibitonBooth(const std::string& name, int capacity,bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
};
/**
 * @brief A catering station. on POWER_OUTAGE it switches to a cold-food 
 *        a fallback menu instead of pausing outright, a different reaction from any other leaf type.
 */
class CateringStation: public EventUnit{
    public:
     /**
     * @param name Display name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     */
        CateringStation(const std::string& name, int capacity,bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
    
    private:
        bool coldMenuOnly_=false;
};

/**
 * @brief A shuttle pickup point. Reroutes on VENUE_CHANGE, halts pickups on EVACUATE but otherwise keeps running through PAUSE notices 
 *        that stop indoor activity, since attendees still need transport.
 */
class ShuttleStop:public EventUnit{
    public:
     /**
     * @param name Display name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     */
        ShuttleStop(const std::string& name, int capacity, bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
    private:
        std::string route_="Main Campus Loop";
    };

    
    /**
     * @brief The on-site medical station. Deliberately remains operational through PAUSE/EVACUATE/POWER_OUTAGE, this leaf cannot just stop. 
     *        again identical notices will produce different behaviour through polymorphism.
     */
class MedicalStation:public EventUnit{
    public:
     /**
     * @param name Display name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     */
        LectureHall(const std::string& name, int capacity, bool indoors=true);

    protected:
        void reactToNotice(const Notice& notice) override;
};

#endif