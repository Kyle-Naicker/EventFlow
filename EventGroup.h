/**
 * @file EventGroup.h
 * @brief The Composite participant of the GoF Composite pattern and a 
 *        hybrid observer/subject in the observer pattern
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
 * EventControl -> root EventGroup -> child EventGroup ->
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
        /**
         * @param name Display name of this group (e.g. "Keynote Zone").
         * @param startTime Scheduled start time for this group's programme,
         *                  e.g. "09:00".
         * @param endTime Scheduled end time for this group's programme.
         */

        EventGroup(const std::string& name, const std::string& startTime = "",const std::string& endTime = "");

        /// Destroys this group and, by explicitly `delete`-ing every pointer
        /// in its owned children_ vector, its entire owned subtree exactly
        /// once. Because every EventComponent in the tree is reachable
        /// through exactly one owning EventGroup at a time (add()/
        /// removeChild() maintain this invariant), no pointer is ever
        /// deleted twice and nothing is ever leaked.
        ~EventGroup() override;


        void open() override;
        void close() override;
        void reportStatus(int indent=0) const override;
        int getCapacity() const override;

        // --- Composite-specific structural operations ---------------------
        /**
         * @brief Adds a child to this group, taking ownership of it, and
         *        automatically registers it as an observer of this group.
         * @param child Owning pointer to the new child. Must not be null,
         *              and must not already be owned by another EventGroup
         *              (use removeChild() on the current owner first if it
         *              is being transferred -- see Task 4.2).
         */
        void add(EventComponent* child);

        /**
         * @brief Removes a child from this group by pointer identity,
         *        releasing ownership to the caller (the caller now becomes
         *        responsible for either add()ing it to another EventGroup or
         *        explicitly deleting it) and detaching it as an observer of
         *        this group.
         *
         * This is the operation used for runtime reorganisation (Task 4.2):
         * the caller typically immediately add()s the returned pointer to a
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

         // --- Observer (this group is notified from above) -----------------
         /**
          * @brief Reacts to a notice pushed from a parent Subject, then
          *        cascades it to this group's own observers (its children).
          *        This is the "receives from above, notifies below" behaviour
          *        required by Task 3.4.
          */
         /// @param notice The notice pushed from a parent Subject.
        void update(const Notice& notice) override;

        // --- Subject (this group is a Subject to its own children) --------
        // attach()/detach()/notify() are inherited unchanged from Subject
        // (see Subject.h); only Subject's pure virtual describeSubject()
        // needs an override here.
        /// @return A short label identifying this group.
        std::string describeSubject() const override;

    protected:
        /**
         * @brief Self-reaction hook, analogous to EventUnit::reactToNotice
         *        but for group-level state (kept separate from cascading in
         *        notify()).
         *
         * Per the current notice-reaction policy (see README.md), EventGroup
         * only reacts to: OPEN/CLOSE/EVACUATE (open/closed state -- required
         * to affect *every* EventComponent), CAPACITY_ALERT (compares its
         * own currentAttendance_ against capacityThreshold_ via the shared
         * EventComponent::reportCapacityAlert() helper -- also required to
         * affect every EventComponent), and ANNOUNCEMENT (a PA-style notice
         * that, per policy, only ever affects EventGroups). PAUSE, RESUME,
         * SCHEDULE_CHANGE and CANCEL are deliberately *not* handled here --
         * those four only affect specific ConcreteLeaves, never a group.
         * @param notice The notice being reacted to.
         */
        virtual void reactToNotice(const Notice& notice) override;
    
    private:
        std::vector<EventComponent*> children_; ///< Owned subtree
}

#endif