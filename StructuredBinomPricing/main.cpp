#include <iostream>
// Don't know how to import from other project ¯\_(ツ)_/¯
#include "../Structured//BinomPricer.h"
#include "../Structured/Options.h"
#include <string>

int main() {

    double up = 0.05;
    double down = 0.025;
    double rfr = 0.0;

    std::cout << "Enter Up, Down and Rfr values: " << std::endl;
    std::cin >> up >> down >> rfr;

    // Create binomial model with UpFactor=0.05, DownFactor=0.05, and riskFreeRate=0.0
    auto pricer = BinomPricer(up, down, rfr);

    double strike = 100.0;
    std::cout << "Enter strike price: " << std::endl;
    std::cin >> strike;



    // Create standard option with strike 100
    auto callOption = CallOption(strike);
    auto putOption = PutOption(strike);

    std::cout << "Enter number of timesteps N, and initial stock price S0: " << std::endl;
    int N = 5;
    int S0 = 100.0;
    std::cin >> N >> S0;

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

    auto doubleDigital = DoubleDigital<CallOption, PutOption>(CallOption(90.0), PutOption(100.0));

    // Price Double digital option
    call_price = pricer.Evaluate(
            S0,
            N,
            [&doubleDigital](double x){return doubleDigital.Payout(x);}
    );
    std::cout << "Eur Double Digital Call Price: " << call_price << std::endl;

    return 0;
}
