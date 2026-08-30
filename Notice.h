/**
 * @file Notice.h
 * @brief Defines the notice representation broadcast through the observer
 *        collaboration in EventFlow
 */
#ifndef NOTICE_H
#define NOTICE_H

#include <string>
/**
 * @brief The kind of notice that can travel through EventFlows's Observer
 * 
 * The first seven values are notices required be the specificaation
 * The final three (VENUE_CHANGE, POWER_OUTAGE, CANCEL) are original features (Task 4.4)
 */

enum class NoticeType {
    OPEN,                   ///< An area should open for business
    CLOSE,                  ///< An area should close
    SCHEDULE_CHANGE,        ///< A schedule change has occured
    CAPACITY_ALERT,         ///< A capacity threshold has been reached
    PAUSE,                  ///< Activity should pause
    RESUME,                 ///< A previously paused activity may resume
    EVACUATE,               ///< Attendees must evacuate the affected area
    VENUE_CHANGE,           ///< The Venue of the event changed
    POWER_OUTAGE,           ///< Loadshedding or power outage in progress
    CANCEL                  ///< The event is cancelled
};

/**
 * @brief Coverts a NoticeType to a short label, used for console output only
 * @param[in] type The notice type to describe
 * @return A short, non-owning string describing the type
 */

const char* noticeTypeName(NoticeType type);
/**
 * @brief A single notice broadcast by a Subject to it Observer
 * 
 * EventFlow uses a 'push' model: The subject packages everything an Observer needs directly
 * into Notice
 */
struct Notice {
    NoticeType type;                ///< What kind of notice this is
    std::string message;            ///< Detail for Logging/UI
    int capacityThreshold = -1;     ///< -1 if unused
    /// Default-constructor
    Notice() : type(NoticeType::SCHEDULE_CHANGE), message(""), capacityThreshold(-1){}
    /**
     * @param[in] t Kind of notice
     * @param[in] msg Detail for logging/UI
     * @param[in] threshold Capacity threshold; -1 if unused
     */
    Notice(NoticeType t, const std::string& msg, int threshold = -1) : type(t), message(msg), capacityThreshold(threshold){}
};

#endif