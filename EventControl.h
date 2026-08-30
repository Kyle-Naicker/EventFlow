/**
 * @file EventControl.h
 * @brief EventControl - the event-wide control center
 */
#ifndef EVENTCONTROL_H
#define EVENTCONTROL_H

#include <string>
#include "Subject.h"
#include "Notice.h"

/**
 * @brief The event-level coordinator that originates notices.
 * 
 * EventControl is deliberately not part of the Composite tree (Explained in Task 11:
 * "Is EventControl itself part of the Composite or only a Subject/client collaborator").
 * Represents the human control-room staff issuing istructions. It is a concrete Subject:
 * whatever top-level EventGroup/s are attached to it will receive and cascade its notices down
 * through the whole tree.
*/


class EventControl : public Subject {
    public:
        /// @param[in] eventName Name of the overall event, this control centre runs
        explicit EventControl(const std::string& eventName) : eventName_(eventName){}
        /// @return The name of the overall event this control center runs.
        const std::string& getEventName() const { return eventName_;}
        /**
         * @brief Issues a notice to the whole event, for example, broadcasts it to every
         * currently attached observer
         * @param[in] notice The notice to issue
         */
        void issueNotice(const Notice& notice);
    private:
        std::string eventName_; ///< Name of the overall event
};

#endif