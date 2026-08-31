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
        EventControl(const std::string& eventName) : eventName_(eventName){}
        /// @return The name of the overall event this control center runs.
        const std::string& getEventName() const { return eventName_;}
        /**
         * @brief Issues a notice to the whole event, for example, broadcasts it to every
         * currently attached observer
         * @param[in] notice The notice to issue
         */
        void issueNotice(const Notice& notice);
        /**
         * @brief Sends a notice directly to one specific Observer without going through
         *        this control centre's own registration list
         * 
         * This is how EventFlow supports targeting one particular part of the venue instead of 
         * always cascading from the root
         * 
         * @param[in] target The specific observer to notify. Must not be nullptr.
         * @param[in] notice The notice to deliver.
         */
        void issueDirectNotice(Observer* target, const Notice& notice);
        /// @return A short label identifying this control center
        std::string describeSubject() const override;
    private:
        std::string eventName_; ///< Name of the overall event
};

#endif