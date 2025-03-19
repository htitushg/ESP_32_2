//
// Created by thorgan on 3/17/25.
//

#ifndef IOBSERVABLE_H
#define IOBSERVABLE_H

#include <IObserver.h>

class IObservable {
public:
    virtual ~IObservable() = default;

    virtual void Attach(IObserver *observer) = 0;
    virtual void Detach(IObserver *observer) = 0;
    virtual void Notify() = 0;
};

#endif //IOBSERVABLE_H
