#include "ConcreteLeaves.h"
#include <iostream>

LectureHall::LectureHall(const std::string& name, int capacity, bool indoors):EventUnit(name,capacity,indoors){}

void LectureHall::reactToNotice(const Notice& notice) {
    switch(notice.type){
        case NoticeType::OPEN:
            isOpen_=true;
            std::cout<<"  [LectureHall:"<<name_<<"] opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_=false;
            std::cout<<"  [LectureHall:"<<name_<<"] closed\n";
            break;

        // CAPACITY_ALERT

        case NoticeType::WEATHER_ALERT:
            if(indoors_){
                std::cout<<"  [LectureHall:"<< name_<< "] indoors, continuing talk\n";
            }else{
                isPaused_=true;
                std::cout << "  [LectureHall:" << name_ << "] outdoors, talk paused ("<<notice.message<<")\n";
            }
            break;
        case NoticeType::PAUSE:
            isPaused_=true;
            std::cout<<"  [LectureHall:"<<name_<<"] talk paused ("<<notice.message<<")\n";
            break;
        case NoticeType::RESUME:
            isPaused_=false;
            std::cout<<"  [LectureHall:"<<name_<<"] talk resumed\n";
            break;
        case NoticeType::EVACUATE:
            isPaused_=true;
            std::cout<<"  [LectureHall:"<<name_<<"] talk stopped immediately ("<<notice.message<<")\n";
            break;
        case NoticeType::CANCEL:
            isOpen_=false;
            std::cout<<"  [LectureHall:"<<name_<<"] talk cancelled ("<<notice.message<<")\n";
            break;

        // ANNOUNCEMENT

        default:
            break;
    }
}



WorkshopRoom::WorkshopRoom(const std::string& name, int capacity, bool indoors):EventUnit(name, capacity, indoors) {}


void WorkshopRoom::reactToNotice(const Notice& notice) {
    switch(notice.type) {
        case NoticeType::OPEN:
            isOpen_=true;
            std::cout<<"  [WorkshopRoom:"<<name_<<"] opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_=false;
            std::cout<<"  [WorkshopRoom:"<<name_<<"] closed\n";
            break;

        // CAPACITY_ALERT
        
        case NoticeType::WEATHER_ALERT:
            if(indoors_){
                std::cout<<"  [WorkshopRoom:"<< name_<< "] indoors, continuing session\n";
            }else{
                isPaused_=true;
                std::cout << "  [WorkshopRoom:" << name_ << "] outdoors, session paused ("<<notice.message<<")\n";
            }
            break;
        case NoticeType::PAUSE:
            isPaused_=true;
            std::cout<<"  [WorkshopRoom:"<<name_<<"] session paused ("<<notice.message<<")\n";
            break;
        case NoticeType::RESUME:
            isPaused_=false;
            std::cout<<"  [WorkshopRoom:"<<name_<< "] session resumed\n";
            break;
        case NoticeType::EVACUATE:
            isPaused_=true;
            std::cout<<"  [WorkshopRoom:"<<name_<<"] session stopped immediately ("<<notice.message<<")\n";
            break;
        case NoticeType::CANCEL:
            isOpen_=false;
            std::cout<<"  [WorkshopRoom:"<<name_<<"] session cancelled ("<<notice.message<<")\n";
            break;

        // ANNOUNCEMENT
        
        default:
            break;
    }
}


RegistrationDesk::RegistrationDesk(const std::string& name, int capacity, bool indoors):EventUnit(name, capacity, indoors) {}

void RegistrationDesk::reactToNotice(const Notice& notice) {
    switch (notice.type) {
        case NoticeType::OPEN:
            isOpen_=true;
            std::cout<<"  [RegistrationDesk:"<<name_<<"] opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_=false;
            std::cout<<"  [RegistrationDesk:"<<name_<<"] closed\n";
            break;
        case NoticeType::WEATHER_ALERT:
            if(indoors_){
                std::cout<<"  [RegistrationDesk:"<< name_<< "] indoors, continuing badge issuing\n";
            }else{
                isPaused_=true;
                std::cout << "  [RegistrationDesk:" << name_ << "] outdoors, badge issuing paused ("<<notice.message<<")\n";
            }
            break;
        case NoticeType::PAUSE:
            isPaused_=true;
            std::cout<<"  [RegistrationDesk:"<<name_<<"] badge issuing paused ("<<notice.message<<")\n";
            break;
        case NoticeType::RESUME:
            isPaused_=false;
            std::cout<<"  [RegistrationDesk:"<<name_<<"] badge issuing resumed\n";
            break;
        case NoticeType::EVACUATE:
            isPaused_=true;
            std::cout<<"  [RegistrationDesk:"<<name_<<"] badge issuing stopped immediately ("<<notice.message<<")\n";
            break;
        case NoticeType::CANCEL:
            isOpen_=false;
            std::cout<<"  [RegistrationDesk:"<<name_<<"] badge issuing stopped ("<<notice.message<<")\n";
            break;

        // ANNOUNCEMENT

        default:
            break;
    }
}

ExhibitionBooth::ExhibitionBooth(const std::string& name, int capacity, bool indoors):EventUnit(name,capacity,indoors) {}

void ExhibitionBooth::reactToNotice(const Notice& notice) {
    switch(notice.type) {
        case NoticeType::OPEN:
            isOpen_=true;
            std::cout<<"  [ExhibitionBooth:"<<name_<<"] opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_=false;
            std::cout<<"  [ExhibitionBooth:"<<name_<<"] closed\n";
            break; 
            
        // CAPACITY_ALERT

        case NoticeType::WEATHER_ALERT:
            if(indoors_){
                std::cout<<"  [ExhibitionBooth:"<< name_<< "] indoors, continuing live demo\n";
            }else{
                isPaused_=true;
                std::cout << "  [ExhibitionBooth:" << name_ << "] outdoors, live demo paused ("<<notice.message<<")\n";
            }
            break;
        case NoticeType::PAUSE:
            isPaused_=true;
            std::cout<<"  [ExhibitionBooth:"<< name_<< "] live demo paused ("<<notice.message<<")\n";
            break;
        case NoticeType::RESUME:
            isPaused_=false;
            std::cout<<"  [ExhibitionBooth:"<<name_<<"] live demo resumed\n";
            break;
        case NoticeType::EVACUATE:
            isOpen_=false;
            std::cout<<"  [ExhibitionBooth:"<<name_<< "] booth vacated immediately ("<<notice.message<<")\n";
            break;
        case NoticeType::CANCEL:
            isOpen_=false;
            std::cout<<"  [ExhibitionBooth:"<<name_<<"] booth packed down ("<<notice.message<<")\n";
            break;
        
        // ANNOUNCEMENT

        default:
            break;
    }
}


CateringStation::CateringStation(const std::string& name, int capacity, bool indoors):EventUnit(name, capacity, indoors) {}

void CateringStation::reactToNotice(const Notice& notice) {
    switch(notice.type) {
        case NoticeType::OPEN:
            isOpen_=true;
            std::cout<<"  [CateringStation:"<<name_<<"] opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_=false;
            std::cout<<"  [CateringStation:"<<name_<<"] closed\n";
            break; 
        case NoticeType::WEATHER_ALERT:
            if(indoors_){
                std::cout<<"  [CateringStation:"<< name_<< "] indoors, continuing service\n";
            }else{
                isPaused_=true;
                std::cout << "  [CateringStation:" << name_ << "] outdoors, service paused ("<<notice.message<<")\n";
            }
            break;
        case NoticeType::PAUSE:
            isPaused_=true;
            std::cout<<"  [CateringStation:"<< name_<< "] service paused ("<<notice.message<<")\n";
            break;
        case NoticeType::RESUME:
            isPaused_=false;
            if(coldMenuOnly_){
                coldMenuOnly_=false;
                std::cout<<"  [CateringStation:"<<name_<<"] generator online, hot menu restored\n";
            }else{
                std::cout<<"  [CateringStation:"<<name_<<"] service resumed\n";
            }
            break;
        case NoticeType::EVACUATE:
            isPaused_=true;
            std::cout<<"  [CateringStation:"<<name_<<"] service stopped immediately ("<<notice.message<<")\n";
            break;
        case NoticeType::POWER_OUTAGE:
            coldMenuOnly_=true;
            std::cout<<"  [CateringStation:"<<name_<<"] switching to cold-food fallback menu ("<<notice.message<<")\n";
            break;
        case NoticeType::CANCEL:
            isOpen_=false;
            std::cout<<"  [CateringStation:"<<name_<<"] service stopped ("<<notice.message<<")\n";
            break;
        
        // ANNOUNCEMENT

        default:
            break;
    }
}

ShuttleStop::ShuttleStop(const std::string& name, int capacity, bool indoors):EventUnit(name, capacity, indoors) {}

void ShuttleStop::reactToNotice(const Notice& notice) {
    switch(notice.type){
        case NoticeType::OPEN:
            isOpen_=true;
            std::cout<<"  [ShuttleStop:"<<name_<<"] opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_=false;
            std::cout<<"  [ShuttleStop:"<<name_<<"] closed\n";
            break; 
        case NoticeType::RESUME:
            isPaused_=false;
            std::cout<<"  [ShuttleStop:"<<name_<<"] pickups resumed\n";
            break;
        case NoticeType::WEATHER_ALERT:
            isPaused_=true;
            std::cout << "  [ShuttleStop:" << name_ << "] pickups paused due to weather alert ("<<notice.message<<")\n";
            break;
        case NoticeType::EVACUATE:
            isPaused_=true;
            std::cout<<"  [ShuttleStop:"<<name_<< "] pickups halted for evacuation ("<<notice.message<<")\n";
            break;
        default:
            break;
    }
}

MedicalStation::MedicalStation(const std::string& name, int capacity, bool indoors):EventUnit(name, capacity, indoors) {};

void MedicalStation::reactToNotice(const Notice& notice) {
    switch(notice.type){
        case NoticeType::OPEN:
            isOpen_=true;
            std::cout<<"  [MedicalStation:"<<name_<<"] opened\n";
            break;
        case NoticeType::CLOSE:
            isOpen_=false;
            std::cout<<"  [MedicalStation:"<<name_<<"] closed\n";
            break;
        case NoticeType::EVACUATE:
            std::cout<<"  [MedicalStation:"<<name_<<"] remaining active, assisting evacuation\n";
            break;
        case NoticeType::POWER_OUTAGE:
            std::cout<<"  [MedicalStation:"<<name_<<"] switched to backup battery, still operational\n";
            break;
        case NoticeType::CANCEL:
            isOpen_=false;
            std::cout<<"  [MedicalStation:"<<name_<<"] stood down (event cancelled)\n";
            break;

        // ANNOUNCEMENT

        default:
            std::cout<<"  [MedicalStation:"<<name_<<"] remains operational\n";
            break;
    }
}