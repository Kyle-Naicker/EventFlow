#ifndef NOTICE_H
#define NOTICE_H

#include <string>

enum class NoticeType {
    OPEN,
    CLOSE,
    SCHEDULE_CHANGE,
    CAPACITY_ALERT,
    PAUSE,
    RESUME,
    EVACUATE,
    VENUE_CHANGE,
    POWER_OUTAGE,
    CANCEL
};

const char* noticeTypeName(NoticeType type);

struct Notice {
    NoticeType type;
    std::string message;
    int capacityThreshold = -1;
    Notice() : type(NoticeType::SCHEDULE_CHANGE), message(""), capacityThreshold(-1){}
    Notice(NoticeType t, const std::string& msg, int threshold = -1) : type(t), message(msg), capacityThreshold(threshold){}
};

#endif