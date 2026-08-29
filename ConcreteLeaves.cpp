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
