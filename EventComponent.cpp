#include "EventComponent.h"
#include "Notice.h"
#include <iostream>

void EventComponent::reportCapacityAlert(const std::string& label) const {
    if (isAtOrOverCapacityThreshold()) {
        std::cout << "  [" << label << "] VENUE FULL: attendance " << currentAttendance_
                  << " has reached the threshold of " << capacityThreshold_ << "\n";
    } else if (capacityThreshold_ >= 0) {
        std::cout << "  [" << label << "] capacity check: " << currentAttendance_ << "/"
                  << capacityThreshold_ << ", not yet full\n";
    }
}

void EventComponent::updateSchedule(const Notice& notice, const std::string& label) {
    std::string oldStart = startTime_;
    std::string oldEnd = endTime_;
    if (!notice.newStartTime.empty()) startTime_ = notice.newStartTime;
    if (!notice.newEndTime.empty()) endTime_ = notice.newEndTime;
    std::cout << "  [" << label << "] schedule updated: " << oldStart << "-" << oldEnd
              << " -> " << startTime_ << "-" << endTime_ << "\n";
}
