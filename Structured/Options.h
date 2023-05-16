#ifndef STRUCTURES_OPTIONS_H
#define STRUCTURES_OPTIONS_H

#include <vector>
#include <algorithm>

/// Concept/Interface for asset that can be Called and Put
template<typename T>
concept IOption = requires(T a, double b) {
    { a.Payout(b) } -> std::same_as<double>;
};

struct CallOption {
    double strike;
    CallOption(double k){
        strike = k;
    }
    double Payout(double s) {
        return std::max(s - strike, 0.0);
    }
};

struct PutOption {
    double strike;
    PutOption(double k){
        strike = k;
    }
    double Payout(double s) {
        return std::max(strike - s, 0.0);
    }
};

template<IOption T>
struct DigitalOption {
    T option;

    DigitalOption(T underlying) : option(underlying) {}
    DigitalOption(double k) : option(CallOption(k)) {}

    double Payout(double s) {
        return option.Payout(s) > 0.0 ? 1.0 : 0.0;
    }
};

template<IOption T, IOption U>
struct DoubleDigital {
    DigitalOption<T> low;
    DigitalOption<U> high;

    DoubleDigital(T _low, U _high): low(_low), high(_high) {}
    DoubleDigital(double _low, double _high): low(CallOption(_low)), high(PutOption(_high)) { }

    double Payout(double currPrice) {
        return (low.Payout(currPrice) > 0) && (high.Payout(currPrice) > 0);
    }
};

#endif
