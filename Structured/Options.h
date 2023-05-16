#ifndef STRUCTURES_OPTIONS_H
#define STRUCTURES_OPTIONS_H

#include <vector>
#include <algorithm>

/// Concept/Interface for asset that can be Called and Put
template<typename T>
concept IOption = requires(T a, double b) {
    { a.Call(b) } -> std::same_as<double>;
    { a.Put(b) } -> std::same_as<double>;
};

// struct to represent a simple option.
struct Option {
    Option(double k){
        strike = k;
    }

    double strike;
    double Call(double currPrice) {
        return std::max(currPrice - strike, 0.0);
    }
    double Put(double currPrice) {
        return std::max(strike - currPrice, 0.0);
    }
};

template<IOption T>
struct DigitalOption {

    T option;

    DigitalOption(T underlying) : option(underlying) {}
    DigitalOption(double k) : option(Option(k)) {}

    double Call(double currPrice) {
        return option.Call(currPrice) > 0.0 ? 1.0 : 0.0;
    }
    double Put(double currPrice) {
        return option.Put(currPrice) > 0.0 ? 1.0 : 0.0;
    }
};

template<IOption T>
struct DoubleOption {
    T low;
    T high;

    DoubleOption(T _low, T _high): low(_low), high(_high) {}

    DoubleOption(double _low, double _high): low(Option(_low)), high(Option(_high)) { }

    double Call(double currPrice){
        return low.Call(currPrice) + high.Put(currPrice);
    }

    double Put(double currPrice) {
        return low.Put(currPrice) + high.Call(currPrice);
    }
};

#endif
