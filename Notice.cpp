#include "Notice.h"

const char* noticeTypeName(NoticeType type){
    switch (type) {
        case NoticeType::OPEN:
            return "OPEN";
        case NoticeType::CLOSE:
            return "CLOSE";
        case NoticeType::SCHEDULE_CHANGE:
            return "SCHEDULE_CHANGE";
        case NoticeType::CAPACITY_ALERT:
            return "CAPACITY_ALERT";
        case NoticeType::PAUSE:
            return "PAUSE";
        case NoticeType::RESUME:
            return "RESUME";
        case NoticeType::EVACUATE:
            return "EVACUATE";
        case NoticeType::VENUE_CHANGE:
            return "VENUE_CHANGE";
        case NoticeType::POWER_OUTAGE:
            return "POWER_OUTAGE";
        case NoticeType::CANCEL:
            return "CANCEL";
    }
    return "UNKNOWN";
}