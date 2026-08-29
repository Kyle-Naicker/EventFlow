#ifndef OBSERVER_H
#define OBSERVER_H

class Observer {
    public:
        virtual ~Observer() {}
        virtual void update(const Notice& notice) = 0;
};

#endif