//
// Created by Peter Nagymathe on 30/05/2023.
//
#include <iterator>
#include <random>
#include <cmath>
#include <ranges>
#include <algorithm>

auto GaussianSamples(int N = 1, double mean = 0.0, double stddev = 1.0)
{
    std::default_random_engine generator;
    std::normal_distribution<double> dist(mean, stddev);

    return std::ranges::views::iota(0, N)
            | std::ranges::views::transform([&generator, &dist](int i){ return dist(generator); });
}

auto BlackScholesPath(double T, int N, double sigma, double r, double s0){
    double h = T / N;
    auto result = GaussianSamples(N)
        | std::ranges::views::transform(
                [&r, &sigma, &h](double x){
                    return exp((r - sigma*sigma*0.5) * h + sigma * sqrt(h) * x);
                }
        )
        | std::ranges::views::transform([](double x){ return x; });
}