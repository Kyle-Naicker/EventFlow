#include "EventGroup.h"
#include <iostream>

EventGroup::EventGroup(const std::string& name, const std::string& startTime, const std::string& endTime):EventComponent(name, startTime, endTime) {}

EventGroup::~EventGroup() {
    // Clear our own observer list first (these are non-owning pointers to
    // the very children about to be destroyed below -- clearing here just
    // avoids anyone calling notify() again during teardown; it does not
    // delete anything).
    observers_.clear();
    // Explicitly delete every owned child exactly once. Because add()
    // and removeChild() maintain the invariant that each EventComponent
    // is owned by at most one EventGroup at a time, this cannot
    // double-delete anything even after a runtime transfer.
    for (EventComponent* child : children_) {
        delete child;
    }
    children_.clear();
}

void EventGroup::open() {
    isOpen_ = true;
    for (EventComponent* child : children_) {
        child->open();
    }
}

void EventGroup::close() {
    isOpen_ = false;
    for (EventComponent* child : children_) {
        child->close();
    }
}

void EventGroup::reportStatus(int indent) const {
    std::cout << std::string(static_cast<size_t>(indent) * 2, ' ')
              << "+ " << name_ << " [" << (isOpen_ ? "open" : "closed")
              << ", total capacity=" << getCapacity();
    if (!startTime_.empty() || !endTime_.empty()) {
        std::cout << ", schedule=" << startTime_ << "-" << endTime_;
    }
    if (capacityThreshold_ >= 0) {
        std::cout << ", attendance=" << currentAttendance_ << "/" << capacityThreshold_;
    }
    std::cout << "]\n";
    for (const EventComponent* child : children_) {
        child->reportStatus(indent + 1);
    }
}

int EventGroup::getCapacity() const {
    int total = 0;
    for (const EventComponent* child : children_) {
        total += child->getCapacity();
    }
    return total;
}

void EventGroup::add(EventComponent* child) {
    if (!child) return;
    // Only auto-register as observer if the child actually is one
    // (all current EventUnit/EventGroup types are, but this keeps the
    // Composite side generic and safe for any future EventComponent that
    // is not also an Observer).
    Observer* asObserver = dynamic_cast<Observer*>(child);
    children_.push_back(child);
    if (asObserver) {
        attach(asObserver);
    }
}

EventComponent* EventGroup::removeChild(EventComponent* child) {
    // Manual linear search (no <algorithm>): find the index of the
    // pointer we were asked to remove.
    std::size_t foundIndex = children_.size();
    for (std::size_t i = 0; i < children_.size(); ++i) {
        if (children_[i] == child) {
            foundIndex = i;
            break;
        }
    }
    if (foundIndex == children_.size()) {
        return nullptr; // Not found among this group's children.
    }

    Observer* asObserver = dynamic_cast<Observer*>(child);
    if (asObserver) {
        detach(asObserver);
    }

    // Manual erase-by-index. vector::erase() is a plain member function
    // (not from <algorithm>), so this is fine without that header.
    children_.erase(children_.begin() + static_cast<long>(foundIndex));
    return child; // Ownership now passes to the caller.
}

void EventGroup::update(const Notice& notice) {
    reactToNotice(notice);
    // Cascade to our own observers (our children). This is what lets a
    // notice travel EventControl -> root EventGroup -> child EventGroup
    // -> leaf EventUnit 
    notify(notice);
}

void EventGroup::reactToNotice(const Notice& notice) {
    switch (notice.type) {
        case NoticeType::OPEN:
            isOpen_ = true;
            break;
        case NoticeType::CLOSE:
        case NoticeType::EVACUATE:
            // Required to affect every EventComponent. Deliberately
            // does NOT include CANCEL: per policy, CANCEL only affects
            // specific ConcreteLeaves, never a group.
            isOpen_ = false;
            break;
        case NoticeType::CAPACITY_ALERT:
            // Required to apply to every EventComponent, including
            // every EventGroup (root included).
            reportCapacityAlert(describeSubject());
            break;
        case NoticeType::ANNOUNCEMENT:
            // Can be sent either cascaded from the root (a conference-
            // wide PA announcement) or targeted directly at one specific
            // EventGroup (an announcement local to just that area) via
            // EventControl::issueDirectNotice. Per policy, ANNOUNCEMENT
            // only ever affects EventGroups -- it is purely informational
            // (it does not change isOpen_ or any other state) -- so it is
            // announced here and then simply cascades to this group's
            // children like any other notice; individual leaf types have
            // no reaction to it at all.
            std::cout << "  [" << describeSubject() << "] PA ANNOUNCEMENT: " << notice.message << "\n";
            break;
        default:
            // PAUSE, RESUME, SCHEDULE_CHANGE, CANCEL, WEATHER_ALERT and
            // POWER_OUTAGE are all deliberately *not* handled here --
            // per policy, none of them affect an EventGroup, only
            // specific ConcreteLeaves.
            break;
    }
    std::cout << "[" << describeSubject() << "] received " << noticeTypeName(notice.type)
              << " (" << notice.message << "), cascading to " << observerCount()
              << " observer(s)\n";
}

std::string EventGroup::describeSubject() const {
    return "EventGroup["+name_+"]";
}