#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"
#include "Notice.h"

class Subject {
    public:
        virtual ~Subject() {}
        virtual void attach(Observer* observer);
        virtual void detach(Observer* observer);
        virtual void notify(const Notice& notice);
        std::size_t observerCount() const {return observers_.size();}

    protected:
        std::vector<Observer*> observers_;
};

#endif