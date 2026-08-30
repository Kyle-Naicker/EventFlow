/**
 * @file Observer.h
 * @brief The Observer participant of the Observer pattern
 */
#ifndef OBSERVER_H
#define OBSERVER_H

#include "Notice.h"

/**
 * @brief Abstract Observer role
 * 
 * EventFlow uses a push model, so update() receives the full Notice rather than having
 * to query the subject afterwards.
 * Ownership: Subjects store observer as 'non-owning' raw pointers, therefore observer's lifetime is managed
 * elsewhere. An Observer must detach itself before its destroyed.
 */
class Observer {
    public:
        /// @brief Vitual destructor for a polymorphic base. Observers do not own
        virtual ~Observer() {}
        /**
         * @brief Called by a Subject to push a notice to this Observer.
         * @param[in] notice The notice being broadcast.
         */
        virtual void update(const Notice& notice) = 0;
};

#endif