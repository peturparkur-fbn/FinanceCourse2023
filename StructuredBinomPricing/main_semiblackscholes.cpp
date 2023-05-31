#include <iostream>
// Don't know how to import from other project ¯\_(ツ)_/¯
#include "../Structured/BinomPricer.cpp"
#include "../Structured/Options.h"

int main() {

    double maxTime = 1.0;
    long N = 100;

    double volatility = 0.01;
    double rate = 0.01;
    std::cout << "Enter time(T), number of steps (N)" << std::endl;
    std::cin >> maxTime >> N;

    double h = maxTime / N;

    std::cout << "Enter volatility(sigma), risk free rate (r)" << std::endl;
    std::cin >> volatility >> rate;

    double up = exp((rate + volatility * volatility / 2) * h + volatility * pow(h, 0.5)) - 1.0;
    double down = -(exp((rate + volatility * volatility / 2) * h - volatility * pow(h, 0.5)) - 1.0);
    double rfr = exp(rate * h) - 1.0;


    // Create binomial model with UpFactor=0.05, DownFactor=0.05, and riskFreeRate=0.0
    auto pricer = BinomPricer(up, down, rfr);

    double strike = 100.0;
    std::cout << "Enter strike price: " << std::endl;
    std::cin >> strike;



    // Create standard option with strike 100
    auto callOption = CallOption(strike);
    auto putOption = PutOption(strike);

    std::cout << "Enter initial stock price (S0): " << std::endl;
    int S0 = 100.0;
    std::cin >> S0;

    std::cout << "Strike Price: " << strike << std::endl;
    std::cout << "Hyperparameters: [U: " << up << ", D: " << down << ", R: " << rfr << "]" << std::endl;


    // Price european option as call
    auto call_price = pricer.Evaluate(
            S0,
            N,
            [&callOption](double x){return callOption.Payout(x);}
    );
    std::cout << "Eur Call Price: " << call_price << std::endl;

    // Price european option as put
    auto put_price = pricer.Evaluate(
            S0,
            N,
            [&putOption](double x){return putOption.Payout(x);}
    );
    std::cout << "Eur Put Price: " << put_price << std::endl;

    // Price american option as call
    call_price = pricer.EvaluateAmerican(
            S0,
            N,
            [&callOption](double x){return callOption.Payout(x);}
    );
    std::cout << "American Call Price: " << call_price << std::endl;

    // Price american option as put
    put_price = pricer.EvaluateAmerican(
            S0,
            N,
            [&putOption](double x){return putOption.Payout(x);}
    );
    std::cout << "American Put Price: " << put_price << std::endl;


    auto digitalCall = DigitalOption<CallOption>(CallOption(strike));
    auto digitalPut = DigitalOption<PutOption>(PutOption(strike));

    // Price digital european option as call
    call_price = pricer.Evaluate(
            S0,
            N,
            [&digitalCall](double x){return digitalCall.Payout(x);}
    );
    std::cout << "Eur Digital Call Price: " << call_price << std::endl;

    // Price digital european option as put
    put_price = pricer.Evaluate(
            S0,
            N,
            [&digitalPut](double x){return digitalPut.Payout(x);}
    );
    std::cout << "Eur Digital Put Price: " << put_price << std::endl;

    auto doubleDigital = DoubleDigital<CallOption, PutOption>(CallOption(strike - down), PutOption(strike + up));
    // Price Double digital option
    call_price = pricer.Evaluate(
            S0,
            N,
            [&doubleDigital](double x){return doubleDigital.Payout(x);}
    );
    std::cout << "Eur Double Digital Call Price: " << call_price << std::endl;

    return 0;
}
