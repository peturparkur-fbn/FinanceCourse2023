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
    auto option = new Option(strike);

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
            [option](double x){return option->Call(x);}
    );
    std::cout << "Eur Call Price: " << call_price << std::endl;

    // Price european option as put
    auto put_price = pricer.Evaluate(
            S0,
            N,
            [option](double x){return option->Put(x);}
    );
    std::cout << "Eur Put Price: " << put_price << std::endl;

    // Price american option as call
    call_price = pricer.EvaluateAmerican(
            S0,
            N,
            [option](double x){return option->Call(x);}
    );
    std::cout << "American Call Price: " << call_price << std::endl;

    // Price american option as put
    put_price = pricer.EvaluateAmerican(
            S0,
            N,
            [option](double x){return option->Put(x);}
    );
    std::cout << "American Put Price: " << put_price << std::endl;


    auto digital = DigitalOption<Option>(Option(strike));
    // Price digital european option as call
    call_price = pricer.Evaluate(
            S0,
            N,
            [&digital](double x){return digital.Call(x);}
    );
    std::cout << "Eur Digital Call Price: " << call_price << std::endl;

    // Price digital european option as put
    put_price = pricer.Evaluate(
            S0,
            N,
            [&digital](double x){return digital.Put(x);}
    );
    std::cout << "Eur Digital Put Price: " << put_price << std::endl;

    auto doubleDigital = DoubleDigital<Option, Option>(Option(90.0), Option(100.0));

    // Price Double digital option
    call_price = pricer.Evaluate(
            S0,
            N,
            [&doubleDigital](double x){return doubleDigital.Call(x);}
    );
    std::cout << "Eur Double Digital Call Price: " << call_price << std::endl;

    // Price Double digital option
    put_price = pricer.Evaluate(
            S0,
            N,
            [&doubleDigital](double x){return doubleDigital.Put(x);}
    );
    std::cout << "Eur Double Digital Put Price: " << put_price << std::endl;

    return 0;
}
