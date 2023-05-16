//
// Created by Peter Nagymathe on 16/05/2023.
//

#include "BinomPricer.h"
#include <cmath>
#include <ranges>
#include <algorithm>
#include <functional>
#include <iostream>

template<std::ranges::range Range, typename T, typename Function>
static T fold_left(Range&& iter, T init, Function&& f) {
    std::ranges::for_each(iter, [&init, &f](T value){init = f(init, value);});
    return init;
}

double static BinomialCoefficient(int N, int n) {
    if (n < 0 || n > N) {
        return 0;
    }
    double result = 1;
    for (int i = 1; i <= n; i++) {
        result = result * (N - n + i) / i;
    }
    return result;
}

double static RiskNeutralProb(double U, double D, double R) {
    return (R + D) / (U + D);
}

double static BinomialPrice(double S0, double U, double D, int N, double i) {
    return S0 * pow(1 + U, N - i) * pow(1 - D, i);
}

double static AnalyticBinomialPrice(double S0, double U, double D, double R, int N, std::function<double(double)> priceFunc) {
    double q = RiskNeutralProb(U, D, R);
    auto res = std::ranges::views::iota(0, N)
               | std::ranges::views::transform(
            [S0, U, D, N, q, priceFunc](int i) {
                return BinomialCoefficient(N, i) * pow(q, i) * pow(1 - q, N - i) * priceFunc(BinomialPrice(S0, U, D, N, i));
            });
    auto sum = 0.0;
    std::ranges::for_each(res, [&sum](double x) {sum += x;});
    return sum / pow(1 + R, N);
}

// Function to calculate the price of a European call option using the binomial model
double static BinomialBackpropPrice(double S0, double U, double D, double R, int N, std::function<double(double)> priceFunc)
{
    double q = RiskNeutralProb(U, D, R); // Calculate risk-neutral probability

    double Price[N+1]; // Initialize array to hold option prices

    // Calculate option payoffs at final time step
    for (int i = 0; i <= N; i++)
    {
        Price[i] = priceFunc(BinomialPrice(S0, U, D, N, i));
    }

    // Calculate option prices working backwards through the binomial tree
    for (int n = N-1; n >= 0; n--)
    {
        for (int i = 0; i <= n; i++)
        {
            Price[i] = (q * Price[i+1] + (1 - q) * Price[i]) / (1 + R);
        }
    }
    return Price[0]; // Return option price at initial time step
}

// Function to calculate the price of a European call option using the binomial model
double static BinomialBackpropPriceAmerican(double S0, double U, double D, double R, int N, std::function<double(double)> priceFunc)
{
    double q = RiskNeutralProb(U, D, R); // Calculate risk-neutral probability
    double Price[N+1]; // Initialize array to hold option prices

    // Calculate option payoffs at final time step
    for (int i = 0; i <= N; i++)
    {
        Price[i] = priceFunc(BinomialPrice(S0, U, D, N, i));
    }

    // Calculate option prices working backwards through the binomial tree
    for (int n = N-1; n >= 0; n--)
    {
        for (int i = 0; i <= n; i++)
        {
            Price[i] = std::max(
                    (q * Price[i+1] + (1 - q) * Price[i]) / (1 + R),
                    priceFunc(BinomialPrice(S0, U, D, n, i))
            );
        }
    }
    return Price[0]; // Return option price at initial time step
}

class BinomPricer {
private:
    double U;
    double D;
    double R;

public:
    BinomPricer(double up, double down, double rfr) {
        U = up;
        D = down;
        R = rfr;
    }

    double Evaluate(double S0, int N, std::function<double(double)> price) {
        return AnalyticBinomialPrice(S0, U, D, R, N, price);
    }

    double EvaluateAmerican(double S0, int N, std::function<double(double)> price) {
        return BinomialBackpropPriceAmerican(S0, U, D, R, N, price);
    }

    double EvaluateBackprop(double S0, int N, std::function<double(double)> price) {
        return BinomialBackpropPrice(S0, U, D, R, N, price);
    }
};
