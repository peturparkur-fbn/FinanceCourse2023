#include <iostream>
#include "../structures/BinomPricer.h"
#include "../structures/Options.h"
#include <string>

int main() {

    // Create standard option with strike 100
    auto option = new Option(100.0);

    // Create binomial model with UpFactor=0.05, DownFactor=0.05, and riskFreeRate=0.0
    auto pricer = new BinomPricer(0.05, 0.05, 0.0);


    // Price european option as call
    auto call_price = pricer->Evaluate(
            100.0,
            1,
            [option](double x){return option->Call(x);}
    );

    // Price european option as put
    auto put_price = pricer->Evaluate(
            100.0,
            1,
            [option](double x){return option->Put(x);}
    );


    auto digital = new DigitalOption(100.0);
    // Price digital european option as call
    call_price = pricer->Evaluate(
            100.0,
            1,
            [option](double x){return option->Call(x);}
    );

    // Price digital european option as put
    put_price = pricer->Evaluate(
            100.0,
            1,
            [option](double x){return option->Put(x);}
    );

//    std::cout << "Hello, World!" << std::endl;
    return 0;
}
