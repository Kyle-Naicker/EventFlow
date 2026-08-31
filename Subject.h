/**
 * @file Subject.h
 * @brief The Subject participant of the Observer pattern
 */
#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"
#include "Notice.h"

/**
 * @brief Abstruct Subject with shared, reusable implementation
 * Subject is not made purley abstract because attact/detach/notify have a single implementation
 * that every concrete Subject in EventFlow wants to reuse. Subclasses can still override notify().
 */
class Subject {
    public:
        /// @brief Virtual destructor for a polymorphic base. A Destructor doesn't own its observers.
        virtual ~Subject() {}
        /**
         * @brief Registers an observer for future notices
         * Stores a non-owning pointer
         * @param[in] observer Observer to register. Must not be a nullptr.
         */
        virtual void attach(Observer* observer);
        /**
         * @brief Deregisters an observer so it no longer receives notices
         * @param[in] observer Obsrever to deregister. May be nullptr.
         */
        virtual void detach(Observer* observer);
        /**
         * @brief Pushes a notice to every currently registered observer
         * 
         * Only Observers registered before notify() begins.
         * This method iterates over snapshot of the registration list so that an observer
         * detaching itself mid-notification does not invalidate the loop
         * 
         * @param[in] notice The notice to broadcast
         */
        virtual void notify(const Notice& notice);
        /// @return The number of currently registered observers
        std::size_t observerCount() const {return observers_.size();}

        /**
         * @brief Returns a short, label identifying this concrete Subject or used in status/log output
         * 
         * @return A short label identifying this Subject
         */
        virtual std::string describeSubject() const = 0;

    protected:
        std::vector<Observer*> observers_;///< Non-owning registration list
};

#endif