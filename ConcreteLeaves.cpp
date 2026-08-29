#include "ConcreteLeaves.h"
#include <iostream>

LectureHall::LectureHall(const std::string& name,int capacity,bool indoors):EventUnit(name,capacity,indoors){}

void LectureHall::reactToNotice(const Notice& notice) {
    switch(notice.type){
        case NoticeType::PAUSE:
        case NoticeType::EVACUATE:
            isPaused_=true;
            std::cout<<"  [LectureHall:"<<name_<<"] talk paused ("<<notice.message<<")\n";
            break;
        case NoticeType::RESUME:
            isPaused_=false;
            std::cout<<"  [LectureHall:"<<name_<<"] talk resumed\n";
            break;
        case NoticeType::CLOSE:
        case NoticeType::CANCEL:
            isOpen_=false;
            std::cout<<"  [LectureHall:"<<name_<<"] hall closed\n";
            break;
        case NoticeType::SCHEDULE_CHANGE:
            std::cout<<"  [LectureHall:"<<name_<< "] speaker slot updated\n";
            break;
        default:
            break;
    }
}



WorkshopRoom::WorkshopRoom(const std::string& name,int capacity,bool indoors):EventUnit(name, capacity, indoors) {}


void WorkshopRoom::reactToNotice(const Notice& notice) {
    switch(notice.type) {
        case NoticeType::EVACUATE:
            isPaused_=true;
            std::cout<<"  [WorkshopRoom:"<<name_<<"] hands-on activity stopped immediately (evacuation)\n";
            break;
        case NoticeType::PAUSE:
            if(indoors_){
                std::cout<<"  [WorkshopRoom:"<< name_<< "] indoors, continuing session through pause\n";
            }else{
                isPaused_=true;
                std::cout << "  [WorkshopRoom:" << name_ << "] outdoor session paused\n";
            }
            break;
        case NoticeType::RESUME:
            isPaused_=false;
            std::cout<<"  [WorkshopRoom:"<<name_<< "] session resumed\n";
            break;
        case NoticeType::SCHEDULE_CHANGE:
            std::cout<<"  [WorkshopRoom:"<<name_<< "] re-planning exercises for new slot\n";
            break;
        case NoticeType::CLOSE:
        case NoticeType::CANCEL:
            isOpen_=false;
            std::cout<<"  [WorkshopRoom:"<<name_<<"] room closed\n";
            break;
        default:
            break;
    }
}


RegistrationDesk::RegistrationDesk(const std::string& name, int capacity, bool indoors):EventUnit(name, capacity, indoors) {}

void RegistrationDesk::reactToNotice(const Notice& notice) {
    switch (notice.type) {
        case NoticeType::CLOSE:
        case NoticeType::EVACUATE:
        case NoticeType::CANCEL:
            isOpen_=false;
            std::cout<<"  [RegistrationDesk:"<<name_<<"] badge issuing stopped\n";
            break;
        case NoticeType::RESUME:
        case NoticeType::OPEN:
            isOpen_=true;
            std::cout<<"  [RegistrationDesk:"<<name_<<"] badge issuing resumed\n";
            break;
        case NoticeType::CAPACITY_ALERT:
            
            std::cout<<"  [RegistrationDesk:"<<name_<< "] capacity alert noted, no action needed here\n";
            break;
        default:
            break;
    }
}
