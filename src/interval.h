#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
    // Closed intervals on the real line plus 
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

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
    // Static variables which represent the empty set and R respectively
    static const interval empty, universe;
};

static const interval empty;
static const interval universe(-infinity, infinity);

#endif
