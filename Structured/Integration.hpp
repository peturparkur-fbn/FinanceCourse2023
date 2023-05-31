//
// Created by Peter Nagymathe on 31/05/2023.
//

#ifndef STRUCTURED_INTEGRATION_HPP
#define STRUCTURED_INTEGRATION_HPP

#include <cmath>
#include <ranges>
#include <algorithm>
#include <functional>
#include <iostream>

namespace Integration {

    double Integrate_Trapezoid(std::function<double(double)> func, double from = 0.0, double to = 1.0, int n = 1000){
        const double h = (to - from) / n;
        double sum = 0.0;

        // intermediate value iterator
        auto intermediate = std::ranges::views::iota(1, n - 1)
        | std::ranges::views::transform([&func, &from, &h](int i){ return 2 * func(from + i * h); } );

        // Compute sum for integral
        auto r = std::ranges::for_each(intermediate, [&sum](double x){sum + x; return sum;}); // sum
        sum = 0.5 * h * (func(from) + sum + func(to));

        return sum;
    }
}


#endif //STRUCTURED_INTEGRATION_HPP
