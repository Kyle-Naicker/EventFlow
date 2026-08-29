/**
 * @file EventGroup.h
 * @brief The Composite participant of the GoF Composite pattern and a hybrid observer/subject in the observer pattern
 */

#ifndef EVENTFLOW_EVENTGROUP_H
#define EVENTFLOW_EVENTGROUP_H

#include <vector>
#include "EventComponent.h"
#include "Observer.h"
#include "Subject.h"
#include "Notice.h"


/**
 * @brief A grouping/area of the conference (e.g. a wing or a zone) that
 *        contains other EventComponents.
 *
 * GoF roles:
 * - Composite pattern: EventGroup is the Composite. It owns its
 *   children through explicitly-owned raw pointers, and recursively implements
 *   open()/close()/reportStatus()/getCapacity() by delegating to them.
 * - Observer pattern, Observer role: an EventGroup can be attached to
 *   a parent Subject (either another EventGroup or the root
 *   EventControl) so it hears about notices from above.
 * - Observer pattern, Subject role: an EventGroup is also a Subject
 *   to its own children -- when it hears a notice, it both reacts itself
 *   and cascades the notice to every child registered as its observer.
 *
 * This dual Observer/Subject role is exactly what lets a notice
 * originating at EventControl cascade through multiple composite levels
 * (Task 3.4): EventControl -> root EventGroup -> child EventGroup ->
 * leaf EventUnit.
 *
 * Ownership policy: adding a child to an EventGroup transfers
 * ownership to that EventGroup *and* automatically registers the child
 * as an observer of this group. EventFlow does not use smart pointers
 * (see project constraints); instead EventGroup owns its children
 * through plain 'EventComponent*' stored in a 'std::vector', and
 * '~EventGroup()' explicitly 'delete's each one exactly once. The two
 * relationships -- ownership and registration -- are coupled
 * deliberately here: anything physically inside a group should, by
 * default, hear about notices that affect the group. (Task 11: "does
 * every Composite observe its parent, does every child get
 * auto-registered" -- see design rationale for the justification of
 * this coupling, and for why it is *not* the only possible design.)
 */
class EventGroup: public EventComponent, public Observer, public Subject{
    public:
        /// @param name Display name of this group e.g. "Innovation Wing"
        explicit EventGroup(const std::string& name);

        // cascading delete
        ~EventGroup() override;
        void open() override;
        void close() override;
        void reportStatus(int indent=0) const override;
        int getCapacity() const override;

        /**
         * @brief adds a child to this group, taking ownership of it and automatically registers it as an observer.
         * 
         * @param child A pointer to the new child. Must not be null. Must not be already owned by another EventGroup.
         */
        void add(EventComponent* child);

        /**
        * @brief Removes a child from this group by pointer identity,
        *        releasing ownership to the caller and detaching it as an observer of
        *        this group.
        *
        * This is the operation used for runtime reorganisation
        * the caller typically immediately adds the returned pointer to a
        * different EventGroup, which re-establishes both ownership and
        * observer registration in the new parent.
        *
        * @param child Raw pointer identifying which child to remove
        *              (compared by address). Ownership passes to the
        *              caller if found.
        * @return The now-detached child, ready to be add()ed elsewhere, or
        *         nullptr if child was not found among this group's children.
        */
        EventComponent* removeChild(EventComponent* child);

        /// @return The number of direct children.
        std::size_t childCount() const{
            return children_.size();
        }

        /// @param notice The notice pushed from a parent Subject.
        void update(const Notice& notice) override;

    protected:
        /**
         * @brief Self-reaction hook, analogous to EventUnit::reactToNotice
         *         but for group-level state.
         * @param notice The notice being reacted to. 
         */
        virtual void reactToNotice(const Notice& notice) override;
    
    private:
        std::vector<EventComponent*> children_; ///< Own subtree
}

#endif