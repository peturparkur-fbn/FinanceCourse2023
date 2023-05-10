#include <iostream>
#include <cmath>
#include <ranges>
#include <vector>
#include <algorithm>
using namespace std;
namespace rs = std::ranges;

// Function to calculate the risk-neutral probability
double RiskNeutralProb(double U, double D, double R)
{
    return (R + D) / (U + D);
}

// Function to calculate the payoff of a call option
double CallPayoff(double S, double K)
{
    return max(S - K, 0.0);
}

double BinomialPrice(double S0, double U, double D, int N, double i)
{
    return S0 * pow(1 + U, N - i) * pow(1 - D, i);
}

// Function to calculate the price of a European call option using the binomial model
double BinomialBackpropPrice(double S0, double U, double D, double R, int N, double K)
{
    double q = RiskNeutralProb(U, D, R); // Calculate risk-neutral probability

    double Price[N+1]; // Initialize array to hold option prices

    // Calculate option payoffs at final time step
    for (int i = 0; i <= N; i++)
    {
        Price[i] = CallPayoff(BinomialPrice(S0, U, D, N, i), K);
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

/**
 * Calculates the binomial coefficient using the Newton symbol.
 *
 * @param N The total number of items to choose from.
 * @param n The number of items to choose.
 * @return The binomial coefficient C(N, n) using the Newton symbol.
 */
double NewtonSymb(int N, int n) {
    // Check if n is out of range
    if (n < 0 || n > N) {
        return 0;
    }
    double result = 1;
    // Calculate the binomial coefficient using the Newton symbol
    for (int i = 1; i <= n; i++) {
        result = result * (N - n + i) / i;
    }
    return result;
}

double AnalyticBinomialCallPrice(double S0, double U, double D, double R, int N, double K)
{
    double q = RiskNeutralProb(U, D, R); // Calculate risk-neutral probability
    auto res = rs::views::iota(0, N)
               | rs::views::transform(
            [S0, U, D, N, K, q](int i) {
                return NewtonSymb(N, i)
                       * pow(q, i)
                       * pow(1 - q, N - i)
                       * CallPayoff(BinomialPrice(S0, U, D, N, i), K);
            });
    auto sum = rs::fold_left(res, 0.0, [](double state, double x) {
        return state + x;
    });
    return sum / pow(1 + R, N);
}

int main()
{
    double S0 = 100; // Initial stock price
    double U = 0.05;   // Up factor
    double D = 0.05;   // Down factor
    double R = 0.00;  // Risk-free interest rate
    int N = 1;        // Number of time steps
    double K = 100;  // Strike price

    auto q = RiskNeutralProb(U, D, R);
    cout << "Risk neutral probability is: " << q << endl;

    double callPrice = BinomialBackpropPrice(S0, U, D, R, N, K); // Calculate option price
    cout << "(NUMERIC) The price of the call option is: " << callPrice << endl;
    double anaCallPrice = AnalyticBinomialCallPrice(S0, U, D, R, N, K); // Calculate option price
    cout << "(ANALYTIC) The price of the call option is: " << anaCallPrice << endl;

    return 0;
}