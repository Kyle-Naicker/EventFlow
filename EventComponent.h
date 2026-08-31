/**
 * @file EventComponent.h
 * @brief The Component participant of the GoF Composite pattern.
 */
#ifndef EVENTCOMPONENT_H
#define EVENTCOMPONENT_H

#include <string>

struct Notice;

/**
 * @brief Common interface for anything that can appear in the TechConnect
 *        conference's part-whole tree: individual operational units
 *        (Leaves, see EventUnit) and groupings of them (Composites, see
 *        EventGroup).
 *
 * A client holding an EventComponent* can open(), close(), reportStatus()
 * and getCapacity() without knowing or caring whether it is talking to a
 * single room or an entire wing of the conference centre. This is the
 * GoF Composite pattern's Component role.
 *
 * Every EventComponent -- both Leaves and Composites -- carries a
 * schedule (startTime_/endTime_) and a capacity-tracking pair
 * (currentAttendance_/capacityThreshold_), because OPEN, CLOSE,
 * CAPACITY_ALERT and EVACUATE are required to affect *every* kind of
 * EventComponent uniformly (see README.md's per-notice reaction table).
 * Only EventUnit (leaves) additionally track indoors_/isPaused_, since
 * "indoors" and "paused" are only ever meaningful for an individual
 * operational unit, not for a whole area.
 */
class EventComponent {
public:
    /**
     * @param name Human readable name for this part of the event.
     * @param startTime Scheduled start time, e.g. "09:00". Empty if not
     *                  yet scheduled.
     * @param endTime Scheduled end time, e.g. "17:00". Empty if not yet
     *                scheduled.
     */
    EventComponent(const std::string& name, const std::string& startTime = "",
                   const std::string& endTime = "")
        : name_(name), startTime_(startTime), endTime_(endTime) {}

    /// Every polymorphic base in EventFlow has a virtual destructor.
    virtual ~EventComponent() {}

    /**
     * @brief Opens this part of the event.
     *
     * For a Leaf this affects only itself. For a Composite this is a
     * *structural* operation that recurses into every child, distinct
     * from (and complementary to) an OPEN notice travelling through the
     * Observer collaboration.
     */
    virtual void open() = 0;

    /**
     * @brief Closes this part of the event. See open() for the
     *        structural-vs-notification distinction.
     */
    virtual void close() = 0;

    /**
     * @brief Prints a human-readable status report for this component,
     *        recursively including any children.
     * @param indent Number of indentation levels to prefix output lines
     *               with; used so nested components print as a tree.
     */
    virtual void reportStatus(int indent = 0) const = 0;

    /**
     * @brief Computes the capacity of this component.
     *
     * For a Leaf, this is the unit's own capacity. For a Composite, this
     * is the recursive sum of every child's capacity, satisfying the
     * "client can query capacity without knowing Leaf vs Composite"
     * requirement. This is a *different* concept from
     * currentAttendance_/capacityThreshold_ below: getCapacity() is the
     * fixed number of seats/slots that exist, while attendance/threshold
     * track whether it is full *right now*.
     * @return Capacity in attendee headcount.
     */
    virtual int getCapacity() const = 0;

    /// @return This component's display name.
    const std::string& getName() const { return name_; }

    /// @return Whether this component currently considers itself open.
    bool isOpen() const { return isOpen_; }

    /// @return This component's currently scheduled start time.
    const std::string& getStartTime() const { return startTime_; }

    /// @return This component's currently scheduled end time.
    const std::string& getEndTime() const { return endTime_; }

    /**
     * @brief Records how many attendees are currently present at this
     *        component. Simulates a headcount feed (e.g. a door
     *        counter); called by the client, not derived from any
     *        Notice.
     * @param count Current attendance. Should be >= 0.
     */
    void setCurrentAttendance(int count) { currentAttendance_ = count; }

    /// @return The most recently recorded current attendance.
    int getCurrentAttendance() const { return currentAttendance_; }

    /**
     * @brief Sets the attendance figure at which this component should
     *        treat itself as full when it next reacts to a
     *        CAPACITY_ALERT.
     * @param threshold Attendance threshold; a negative value disables
     *                  capacity-alert reactions for this component (the
     *                  default).
     */
    void setCapacityThreshold(int threshold) { capacityThreshold_ = threshold; }

    /// @return The current capacity-alert threshold (-1 if disabled).
    int getCapacityThreshold() const { return capacityThreshold_; }

protected:
    /**
     * @brief Shared CAPACITY_ALERT reaction, used by every concrete
     *        EventComponent (both Leaves and Composites) so that "OPEN,
     *        CLOSE, CAPACITY_ALERT and EVACUATE affect all
     *        EventComponents" does not require duplicating the same
     *        comparison-and-print logic in seven leaf classes plus
     *        EventGroup. Compares currentAttendance_ against
     *        capacityThreshold_ and logs "VENUE FULL" or a "not yet
     *        full" status; does nothing if threshold-tracking is
     *        disabled (capacityThreshold_ < 0).
     * @param label Short label identifying this component in the log
     *              line, e.g. "LectureHall:Hall A" or
     *              describeSubject() for an EventGroup.
     */
    void reportCapacityAlert(const std::string& label) const;

    /**
     * @brief Shared SCHEDULE_CHANGE reaction: updates startTime_/
     *        endTime_ from @p notice (leaving either unchanged if the
     *        corresponding field on the notice is empty) and logs the
     *        old and new schedule. Only called by the four leaf types
     *        SCHEDULE_CHANGE actually affects (LectureHall, WorkshopRoom,
     *        RegistrationDesk, ExhibitionBooth) -- EventGroup and the
     *        remaining leaf types simply never call it.
     * @param notice The SCHEDULE_CHANGE notice being reacted to.
     * @param label Short label identifying this component in the log line.
     */
    void updateSchedule(const Notice& notice, const std::string& label);

    /**
     * @brief True once currentAttendance_ has reached or exceeded
     *        capacityThreshold_ (and threshold-tracking is enabled).
     * @return Whether this component currently considers itself full.
     */
    bool isAtOrOverCapacityThreshold() const {
        return capacityThreshold_ >= 0 && currentAttendance_ >= capacityThreshold_;
    }

    std::string name_;      ///< Display name of this component.
    bool isOpen_ = false;    ///< Whether this component currently considers itself open.
    std::string startTime_; ///< Scheduled start time for this component.
    std::string endTime_;   ///< Scheduled end time for this component.
    int currentAttendance_ = 0;   ///< Most recently recorded headcount.
    int capacityThreshold_ = -1;  ///< Attendance that counts as "full"; -1 = not tracked.
};

#endif // EVENTFLOW_EVENTCOMPONENT_H
