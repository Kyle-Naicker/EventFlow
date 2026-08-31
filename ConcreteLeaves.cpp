#include "ConcreteLeaves.h"
#include <iostream>

LectureHall::LectureHall(const std::string& name, int capacity, bool indoors,const std::string& startTime, const std::string& endTime):EventUnit(name, capacity, indoors, startTime, endTime) {}

void LectureHall::reactToNotice(const Notice& notice) {
    // LectureHall is one of the four leaf types PAUSE/RESUME/
    // SCHEDULE_CHANGE/CANCEL affect, in addition to the OPEN/CLOSE/
    // CAPACITY_ALERT/EVACUATE quartet required for every EventComponent.
    switch (notice.type) {
        case NoticeType::OPEN:
            isOpen_ = true;
            std::cout << "  [LectureHall:" << name_ << "] hall opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_ = false;
            std::cout << "  [LectureHall:" << name_ << "] hall closed\n";
            break;
        case NoticeType::CAPACITY_ALERT:
            reportCapacityAlert("LectureHall:" + name_);
            break;
        case NoticeType::EVACUATE:
            isPaused_ = true;
            std::cout << "  [LectureHall:" << name_ << "] talk paused (" << notice.message << ")\n";
            break;
        case NoticeType::PAUSE:
            isPaused_ = true;
            std::cout << "  [LectureHall:" << name_ << "] talk paused\n";
            break;
        case NoticeType::RESUME:
            isPaused_ = false;
            std::cout << "  [LectureHall:" << name_ << "] talk resumed\n";
            break;
        case NoticeType::SCHEDULE_CHANGE:
            updateSchedule(notice, "LectureHall:" + name_);
            break;
        case NoticeType::CANCEL:
            isOpen_ = false;
            std::cout << "  [LectureHall:" << name_ << "] talk cancelled\n";
            break;
        case NoticeType::WEATHER_ALERT:
            if (!indoors_) {
                isPaused_ = true;
                std::cout << "  [LectureHall:" << name_ << "] outdoor stage paused for weather\n";
            }
            break;
        default:
            break; // POWER_OUTAGE and ANNOUNCEMENT don't affect a LectureHall.
    }
}



// ---------------------------------------------------------------- WorkshopRoom
WorkshopRoom::WorkshopRoom(const std::string& name, int capacity, bool indoors,
                            const std::string& startTime, const std::string& endTime)
    : EventUnit(name, capacity, indoors, startTime, endTime) {}

void WorkshopRoom::reactToNotice(const Notice& notice) {
    switch (notice.type) {
        case NoticeType::OPEN:
            isOpen_ = true;
            std::cout << "  [WorkshopRoom:" << name_ << "] room opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_ = false;
            std::cout << "  [WorkshopRoom:" << name_ << "] room closed\n";
            break;
        case NoticeType::CAPACITY_ALERT:
            reportCapacityAlert("WorkshopRoom:" + name_);
            break;
        case NoticeType::EVACUATE:
            isPaused_ = true;
            std::cout << "  [WorkshopRoom:" << name_ << "] hands-on activity stopped immediately (evacuation)\n";
            break;
        case NoticeType::PAUSE:
            isPaused_ = true;
            std::cout << "  [WorkshopRoom:" << name_ << "] session paused\n";
            break;
        case NoticeType::RESUME:
            isPaused_ = false;
            std::cout << "  [WorkshopRoom:" << name_ << "] session resumed\n";
            break;
        case NoticeType::SCHEDULE_CHANGE:
            updateSchedule(notice, "WorkshopRoom:" + name_);
            std::cout << "  [WorkshopRoom:" << name_ << "] re-planning exercises for new slot\n";
            break;
        case NoticeType::CANCEL:
            isOpen_ = false;
            std::cout << "  [WorkshopRoom:" << name_ << "] session cancelled\n";
            break;
        case NoticeType::WEATHER_ALERT:
            if (!indoors_) {
                isPaused_ = true;
                std::cout << "  [WorkshopRoom:" << name_ << "] outdoor session paused for weather\n";
            }
            break;
        default:
            break;
    }
}


// ---------------------------------------------------------------- RegistrationDesk
RegistrationDesk::RegistrationDesk(const std::string& name, int capacity, bool indoors,
                                    const std::string& startTime, const std::string& endTime)
    : EventUnit(name, capacity, indoors, startTime, endTime) {}

void RegistrationDesk::reactToNotice(const Notice& notice) {
    switch (notice.type) {
        case NoticeType::OPEN:
            isOpen_ = true;
            std::cout << "  [RegistrationDesk:" << name_ << "] desk opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_ = false;
            std::cout << "  [RegistrationDesk:" << name_ << "] badge issuing stopped\n";
            break;
        case NoticeType::CAPACITY_ALERT:
            reportCapacityAlert("RegistrationDesk:" + name_);
            break;
        case NoticeType::EVACUATE:
            isOpen_ = false;
            std::cout << "  [RegistrationDesk:" << name_ << "] desk evacuated\n";
            break;
        case NoticeType::PAUSE:
            isPaused_ = true;
            std::cout << "  [RegistrationDesk:" << name_ << "] queue paused\n";
            break;
        case NoticeType::RESUME:
            isPaused_ = false;
            std::cout << "  [RegistrationDesk:" << name_ << "] badge issuing resumed\n";
            break;
        case NoticeType::SCHEDULE_CHANGE:
            updateSchedule(notice, "RegistrationDesk:" + name_);
            break;
        case NoticeType::CANCEL:
            isOpen_ = false;
            std::cout << "  [RegistrationDesk:" << name_ << "] desk closed (event cancelled)\n";
            break;
        case NoticeType::WEATHER_ALERT:
            if (!indoors_) {
                isOpen_ = false;
                std::cout << "  [RegistrationDesk:" << name_ << "] outdoor registration tent closed for weather\n";
            }
            break;
        default:
            break;
    }
}

// ---------------------------------------------------------------- ExhibitionBooth
ExhibitionBooth::ExhibitionBooth(const std::string& name, int capacity, bool indoors,
                                  const std::string& startTime, const std::string& endTime)
    : EventUnit(name, capacity, indoors, startTime, endTime) {}

void ExhibitionBooth::reactToNotice(const Notice& notice) {
    switch (notice.type) {
        case NoticeType::OPEN:
            isOpen_ = true;
            std::cout << "  [ExhibitionBooth:" << name_ << "] booth opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_ = false;
            std::cout << "  [ExhibitionBooth:" << name_ << "] booth closed\n";
            break;
        case NoticeType::CAPACITY_ALERT:
            reportCapacityAlert("ExhibitionBooth:" + name_);
            break;
        case NoticeType::EVACUATE:
            isOpen_ = false;
            std::cout << "  [ExhibitionBooth:" << name_ << "] booth vacated\n";
            break;
        case NoticeType::PAUSE:
            isPaused_ = true;
            std::cout << "  [ExhibitionBooth:" << name_ << "] live demo suspended\n";
            break;
        case NoticeType::RESUME:
            isPaused_ = false;
            std::cout << "  [ExhibitionBooth:" << name_ << "] live demo resumed\n";
            break;
        case NoticeType::SCHEDULE_CHANGE:
            updateSchedule(notice, "ExhibitionBooth:" + name_);
            break;
        case NoticeType::CANCEL:
            isOpen_ = false;
            std::cout << "  [ExhibitionBooth:" << name_ << "] booth packed down (event cancelled)\n";
            break;
        case NoticeType::WEATHER_ALERT:
            if (!indoors_) {
                isPaused_ = true;
                std::cout << "  [ExhibitionBooth:" << name_ << "] outdoor booth suspended for weather\n";
            }
            break;
        default:
            break;
    }
}

// ---------------------------------------------------------------- CateringStation
CateringStation::CateringStation(const std::string& name, int capacity, bool indoors,
                                  const std::string& startTime, const std::string& endTime)
    : EventUnit(name, capacity, indoors, startTime, endTime) {}

void CateringStation::reactToNotice(const Notice& notice) {
    // CateringStation is one of the three leaf types PAUSE/RESUME/
    // SCHEDULE_CHANGE/CANCEL deliberately do *not* affect; it reacts
    // only to the "affect all EventComponents" quartet, WEATHER_ALERT
    // (if outdoors), and POWER_OUTAGE (its one distinguishing reaction).
    switch (notice.type) {
        case NoticeType::OPEN:
            isOpen_ = true;
            std::cout << "  [CateringStation:" << name_ << "] station opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_ = false;
            std::cout << "  [CateringStation:" << name_ << "] service stopped\n";
            break;
        case NoticeType::CAPACITY_ALERT:
            reportCapacityAlert("CateringStation:" + name_);
            break;
        case NoticeType::EVACUATE:
            isOpen_ = false;
            std::cout << "  [CateringStation:" << name_ << "] station evacuated\n";
            break;
        case NoticeType::WEATHER_ALERT:
            if (!indoors_) {
                isOpen_ = false;
                std::cout << "  [CateringStation:" << name_ << "] outdoor food stand closed for weather\n";
            }
            break;
        case NoticeType::POWER_OUTAGE:
            coldMenuOnly_ = true;
            std::cout << "  [CateringStation:" << name_ << "] switching to cold-food fallback menu\n";
            break;
        default:
            break; // PAUSE, RESUME, SCHEDULE_CHANGE and CANCEL don't affect a CateringStation.
    }
}

// ---------------------------------------------------------------- ShuttleStop
ShuttleStop::ShuttleStop(const std::string& name, int capacity,
                          const std::string& startTime, const std::string& endTime)
    : EventUnit(name, capacity, /*indoors=*/false, startTime, endTime) {
    // ShuttleStops are always outdoors -- forced in the base-class
    // constructor call above, since a bus stop cannot be indoors.
}

void ShuttleStop::reactToNotice(const Notice& notice) {
    switch (notice.type) {
        case NoticeType::OPEN:
            isOpen_ = true;
            std::cout << "  [ShuttleStop:" << name_ << "] stop reopened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_ = false;
            std::cout << "  [ShuttleStop:" << name_ << "] stop closed\n";
            break;
        case NoticeType::CAPACITY_ALERT:
            reportCapacityAlert("ShuttleStop:" + name_);
            break;
        case NoticeType::EVACUATE:
            isOpen_ = false;
            std::cout << "  [ShuttleStop:" << name_ << "] pickups halted for evacuation\n";
            break;
        case NoticeType::WEATHER_ALERT:
            // Always outdoors (see constructor), so this always triggers.
            std::cout << "  [ShuttleStop:" << name_ << "] outdoor stop affected by weather, expect delays\n";
            break;
        default:
            break; // PAUSE, RESUME, SCHEDULE_CHANGE, CANCEL and POWER_OUTAGE don't affect a ShuttleStop.
    }
}



// ---------------------------------------------------------------- MedicalStation
MedicalStation::MedicalStation(const std::string& name, int capacity, bool indoors,
                                const std::string& startTime, const std::string& endTime)
    : EventUnit(name, capacity, indoors, startTime, endTime) {}

void MedicalStation::reactToNotice(const Notice& notice) {
    // The medical station is unaffected by PAUSE/RESUME/SCHEDULE_CHANGE/
    // CANCEL and (when indoors) WEATHER_ALERT -- the one leaf type that
    // must never simply "stop" -- demonstrating polymorphic difference
    // in behaviour for identical notices, purely through each leaf's own
    // reactToNotice(), not type inspection anywhere else in the system.
    switch (notice.type) {
        case NoticeType::OPEN:
            isOpen_ = true;
            std::cout << "  [MedicalStation:" << name_ << "] station opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_ = false;
            std::cout << "  [MedicalStation:" << name_ << "] station closed\n";
            break;
        case NoticeType::CAPACITY_ALERT:
            reportCapacityAlert("MedicalStation:" + name_);
            break;
        case NoticeType::EVACUATE:
            std::cout << "  [MedicalStation:" << name_ << "] remaining active, assisting evacuation\n";
            break;
        case NoticeType::WEATHER_ALERT:
            if (!indoors_) {
                std::cout << "  [MedicalStation:" << name_ << "] outdoor post remains active despite weather\n";
            }
            break;
        case NoticeType::POWER_OUTAGE:
            std::cout << "  [MedicalStation:" << name_ << "] switched to backup battery, still operational\n";
            break;
        default:
            break; // PAUSE, RESUME, SCHEDULE_CHANGE, CANCEL and ANNOUNCEMENT don't affect a MedicalStation.
    }
}