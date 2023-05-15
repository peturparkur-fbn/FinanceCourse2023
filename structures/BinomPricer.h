#ifndef BINOMPRICER_H
#define BINOMPRICER_H

#include <cmath>
#include <ranges>
#include <algorithm>
#include <functional>
//using namespace std;

double static NewtonSymb(int N, int n) {
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
                return NewtonSymb(N, i) * pow(q, i) * pow(1 - q, N - i) * priceFunc(BinomialPrice(S0, U, D, N, i));
            });
    auto sum = std::ranges::fold_left(res, 0.0, [](double state, double x) {return x + state;});
    return sum / pow(1 + R, N);
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

    double AnalyticBinomialPrice(double S0, double U, double D, double R, int N, std::function<double(double)> priceFunc) {
        double q = RiskNeutralProb(U, D, R);
        auto res = std::ranges::views::iota(0, N)
                   | std::ranges::views::transform(
                [S0, U, D, N, q, priceFunc, this](int i) {
                    return NewtonSymb(N, i) * pow(q, i) * pow(1 - q, N - i) * priceFunc(BinomialPrice(S0, U, D, N, i));
                });
        auto sum = std::ranges::fold_left(res, 0.0, [](double state, double x) {return x + state;});
        return sum / pow(1 + R, N);
    }

    double Evaluate(double S0, int N, std::function<double(double)> price) {
        return AnalyticBinomialPrice(S0, U, D, R, N, price);
    }
};

#endif
