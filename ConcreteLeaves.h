/**
 *  @file ConcreteLeaves.h
 *  @brief Concrete Leaf types for the TechConnect conference. Each type reacts meaningfully differently to the same notices. 
 *  Satisfying task 4.1
 * 
 *  Every leaf's constructor accepts its own schedule (startTime/endTime,
 * inherited from EventComponent) since, per the current tree design,
 * both EventGroups and individual leaves are scheduled independently.
 * "Any EventComponent can be outdoors" (see README.md), so every leaf
 * below takes an `indoors` parameter defaulting to true -- except
 * ShuttleStop, which is always outdoors by nature (a bus stop cannot be
 * indoors), so its `indoors` flag is fixed rather than exposed.
 *
 * Notice-reaction policy (see README.md for the full table): OPEN,
 * CLOSE, CAPACITY_ALERT and EVACUATE affect every leaf type below (and
 * every EventGroup); PAUSE, RESUME, SCHEDULE_CHANGE and CANCEL affect
 * only LectureHall, WorkshopRoom, RegistrationDesk and ExhibitionBooth;
 * WEATHER_ALERT affects a leaf only when it is actually outdoors;
 * POWER_OUTAGE affects only MedicalStation and CateringStation;
 * ANNOUNCEMENT never affects a leaf (EventGroups only).
 * 
 * 
 */

#ifndef CONCRETELEAVES_H
#define CONCRETELEAVES_H

#include "EventUnit.h"

/**
 * @brief A keynote/talk room. 
 * 
 */
class LectureHall:public EventUnit{
    public:
    /**
     * @param name Display the name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     * @param startTime Scheduled start time, e.g. "10:00".
     * @param endTime Scheduled end time, e.g. "12:00".
     */
        LectureHall(const std::string& name, int capacity, bool indoors = true,const std::string& startTime = "", const std::string& endTime = "");

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
     * @param startTime Scheduled start time.
     * @param endTime Scheduled end time.
     */
       WorkshopRoom(const std::string& name, int capacity, bool indoors = true,const std::string& startTime = "", const std::string& endTime = "");

    protected:
        void reactToNotice(const Notice& notice) override;
};

/**
 * @brief The attendee registration/check-in desk. 
 * 
 */
class RegistrationDesk: public EventUnit{
    public:
     /**
     * @param name Display name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     * @param startTime Scheduled start time.
     * @param endTime Scheduled end time.
     */
         RegistrationDesk(const std::string& name, int capacity, bool indoors = true,const std::string& startTime = "", const std::string& endTime = "");

    protected:
        void reactToNotice(const Notice& notice) override;
};
/**
 * @brief A sponsor/exhibition booth. Suspends live demos on PAUSE but,
 *        unlike a LectureHall, fully packs down (closes) on CANCEL
 *        rather than merely pausing.
 */
class ExhibitionBooth: public EventUnit{
    public:
     /**
     * @param name Display name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     * @param startTime Scheduled start time.
     * @param endTime Scheduled end time.
     */
         ExhibitionBooth(const std::string& name, int capacity, bool indoors = true,const std::string& startTime = "", const std::string& endTime = "");

    protected:
        void reactToNotice(const Notice& notice) override;
};
/**
 * @brief A catering station. On POWER_OUTAGE it switches to a cold-food
 *        fallback menu instead of pausing outright -- a genuinely
 *        different reaction from every other leaf type. Per policy it
 *        does *not* react to PAUSE/RESUME/SCHEDULE_CHANGE/CANCEL.
 */

class CateringStation: public EventUnit{
    public:
     /**
     * @param name Display name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     * @param startTime Scheduled start time.
     * @param endTime Scheduled end time.
     */
        CateringStation(const std::string& name, int capacity, bool indoors = true,const std::string& startTime = "", const std::string& endTime = "");

    protected:
        void reactToNotice(const Notice& notice) override;
    
    private:
        bool coldMenuOnly_=false; ///< True while running on the cold-food fallback menu (power outage).
};

/**
 * @brief A shuttle pickup point. Always outdoors by nature (a bus stop
 *        cannot be indoors), so its `indoors` flag is fixed rather than
 *        exposed as a constructor parameter. Per policy it does *not*
 *        react to PAUSE/RESUME/SCHEDULE_CHANGE/CANCEL -- attendees still
 *        need transport during a session pause that isn't itself a
 *        weather alert or evacuation.
 */
class ShuttleStop:public EventUnit{
    public:
     /**
     * @param name Display name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     * @param startTime Scheduled start time.
     * @param endTime Scheduled end time.
     */
       ShuttleStop(const std::string& name, int capacity,const std::string& startTime = "", const std::string& endTime = "");

    protected:
        void reactToNotice(const Notice& notice) override;
    
    };

    
    /**
     * @brief The on-site medical station. Deliberately remains unaffected by
     *        PAUSE/RESUME/SCHEDULE_CHANGE/CANCEL/WEATHER_ALERT (when
     *        indoors) -- the one leaf type that must never simply "stop" --
     *        demonstrating that identical notices produce different
     *        behaviour purely through polymorphism, not type inspection.
     *        Reacts to POWER_OUTAGE by switching to backup battery power.
     */
class MedicalStation:public EventUnit{
    public:
     /**
     * @param name Display name of this unit.
     * @param capacity Attendee capacity of this unit.
     * @param indoors Whether this unit is physically indoors.
     * @param startTime Scheduled start time.
     * @param endTime Scheduled end time.
     */
       MedicalStation(const std::string& name, int capacity, bool indoors = true,const std::string& startTime = "", const std::string& endTime = "");

    protected:
        void reactToNotice(const Notice& notice) override;
};

#endif