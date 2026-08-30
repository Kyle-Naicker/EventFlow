#include "Subject.h"

void Subject::attach(Observer* observer){
    if(!observer) return;
    for (std::size_t i = 0; i < observers_.size(); i++){
        if (observers_[i] == observer){
            return;
        }
    }
    observers_.push_back(observer);
}

void Subject::detach(Observer* observer){
    if (!observer) return;
    for (std::size_t i = 0; i < observers_.size(); i++){
        if (observers_[i] == observer){
            observers_.erase(observers_.begin() + static_cast<long>(i));
            return;
        }
    }
}