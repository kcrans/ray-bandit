#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
    public:
    double min, max;

    interval() : min(infinity), max(-infinity) {} // default empty interval
    
    interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    static const interval empty, universe;
};

const static interval empty;
const static interval universe(-infinity, infinity);

#endif
