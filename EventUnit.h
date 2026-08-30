/**
 * @file EventUnit.h
 * @brief The leaf participant of Composite, which is simaltaneously an Observer participant of Observer
 */

#ifndef EVENTFLOW_EVENTUNIT_H
#define EVENTFLOW_EVENTUNIT_H
#include "EventComponent.h"
#include "Observer.h"
#include "Notice.h"



/**
 * @brief Abstract base for every individual (non-composite) operational
 *        unit at TechConnect: a leaf in the Composite tree.
 *
 * GoF roles:
 * - Composite pattern: EventUnit is a concrete Leaf. It has no
 *   children and implements the component operations directly.
 * - Observer pattern: EventUnit is a concrete Observer. It is
 *   registered with the EventGroup that contains it and reacts to
 *   notices pushed down from above via update().
 *
 * These are two separate collaborations for two separate reasons:
 * Composite asks "what does this unit consist of?" (answer: nothing, it
 * is atomic) while Observer asks "who does this unit need to hear from?"
 * (answer: its containing EventGroup). Being both is not a pattern
 * misuse - see design rationale in README.md.
 *
 * Concrete subclasses only need to implement reactToNotice() to get
 * meaningfully different, polymorphic behaviour for the same notice -
 * no type-checking is used anywhere in the dispatch.
 */
class EventUnit:public EventComponent,public Observer{
    public:
        /**
        * @param name Display name of this unit.
        * @param capacity Attendee capacity of this unit.
        * @param indoors Whether this unit is indoors (used by some units to
        *                decide how to react to safety-related notices).
        */
        EventUnit(const std::string& name, int capacity, bool indoors);

        /// Virtual Destructor
        ~EventUnit() override{};

        void open() override;
        void close() override;
        void reportStatus(int index=0) const override;
        int getCapacity() const override;

        /**
        * @brief Observer entry point. Dispatches to reactToNotice() so that
        *        concrete leaves can react differently to the same notice
        *        through polymorphism rather than type inspection.
        * @param notice The notice pushed from this unit's containing group.
        */
        void update(const Notice& notice) override;

        
        /// @return Whether this unit is currently paused
        bool isPaused() const{
            return isPaused_;
        }

        
        /// @return Whether this unit is physically indoors
        bool isIndoors() const{
            return indoors_;
        }

    protected:
        /**
        * @brief Concrete-leaf-specific reaction to an incoming notice.
        *
        * Subclasses override this to implement the "same notice, different
        * behaviour" requirement. Implementations should update
        * isOpen_/isPaused_/status_ as appropriate and may print a short log
        * line describing what happened.
        * @param notice The notice being reacted to.
        */
        virtual void reactToNotice(const Notice& notice)=0;

        /// @return A short label describing this unit's current activity,
        ///         shown by the default reportStatus() implementation.
        virtual std::string statusLine() const;

        bool indoors_;
        bool isPaused_=false;
        int capacity_=0;
}
#endif