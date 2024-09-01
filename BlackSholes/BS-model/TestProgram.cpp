// Group A & B: Test Program for Option Pricing and Option Sensitivities

#include "EuroOption.h"
#include "AmericanOption.h"
#include <iostream>
#include <cmath>
#include <boost/math/distributions/normal.hpp>
#include <vector>
using namespace std;
using namespace boost::math;
using namespace Options;

void main()
{
	// Calculate and print European Option Prices from batch 1 to 4 
	EuroOption batch1(0.65, 0.35, 0.065, 0, 70, 65);
	cout << "Call Price of Batch 1: " << batch1.EuroCallPrice() << endl << "Put Price of Batch 1: " << batch1.EuroPutPrice() << endl;

	cout << "delta" << batch1.PutDelta() << endl;
	cout << "gamma " << batch1.Gamma();

	/*EuroOption batch2(1.0, 0.2, 0.0, 0.0, 100, 100);
	cout << "Call Price of Batch 2: " << batch2.EuroCallPrice() << endl << "Put Price of Batch 2: " << batch2.EuroPutPrice() << endl;

	EuroOption batch3(1.0, 0.50, 0.12, 0.0, 5, 10);
	cout << "Call Price of Batch 3: " << batch3.EuroCallPrice() << endl << "Put Price of Batch 3: " << batch3.EuroPutPrice() << endl;

	EuroOption batch4(30.0, 0.30, 0.08, 0.0, 100, 100);
	cout << "Call Price of Batch 4: " << batch4.EuroCallPrice() << endl << "Put Price of Batch 4: " << batch4.EuroPutPrice() << endl;

	cout << endl;

	// Test Put- Call Parity by matching expression generated call price with actual price calculated above
	cout << "Call Price Based on Put-Call Parity - " << endl;
	cout << "Batch 1 - Call Price corresponding to Put Price (" << batch1.EuroPutPrice() << ") is " << batch1.CallParityPrice() << endl;
	cout << "Batch 2 - Call Price corresponding to Put Price (" << batch2.EuroPutPrice() << ") is " << batch2.CallParityPrice() << endl;
	cout << "Batch 3 - Call Price corresponding to Put Price (" << batch3.EuroPutPrice() << ") is " << batch3.CallParityPrice() << endl;
	cout << "Batch 4 - Call Price corresponding to Put Price (" << batch4.EuroPutPrice() << ") is " << batch4.CallParityPrice() << endl;

	cout << endl;

	// Test Put- Call Parity by matching expression generated put price with actual price calculated above
	cout << "Put Price Based on Put-Call Parity - " << endl;
	cout << "Batch 1 - Put Price corresponding to Call Price (" << batch1.EuroCallPrice() << ") is " << batch1.PutParityPrice() << endl;
	cout << "Batch 2 - Put Price corresponding to Call Price (" << batch2.EuroCallPrice() << ") is " << batch2.PutParityPrice() << endl;
	cout << "Batch 3 - Put Price corresponding to Call Price (" << batch3.EuroCallPrice() << ") is " << batch3.PutParityPrice() << endl;
	cout << "Batch 4 - Put Price corresponding to Call Price (" << batch4.EuroCallPrice() << ") is " << batch4.PutParityPrice() << endl;

	cout << endl;

	// Test Put-Call parity for the prices of call and put options be equating both sides of the expression
	cout << "Test if Options comply with Put-Call Parity - " << endl;
	cout << "For Batch 1: ";
	batch1.ParityCheck();
	cout << "For Batch 2: ";
	batch2.ParityCheck();
	cout << "For Batch 3: ";
	batch3.ParityCheck();
	cout << "For Batch 4: ";
	batch4.ParityCheck();

	cout << endl;

	// Test how change in underlying price affects option pricing
	cout << "On changing one parameter affecting option pricing - " << endl;
	EuroOption batch5(0.25, 0.30, 0.08, 0.0, 60, 80);
	batch5.SingleFactorPricer(60, 80, 1);

	cout << endl;

	// Test how changing multiple parameters affect option pricing by creating different scenarios for a batch 
	cout << "On changing multiple parameters affecting option pricing - " << endl;
	vector<vector<double>> scenarios1 = { {0.25, 80, 0.30, 0.08, 60, 0.0}, {0.50, 85, 0.35, 0.1, 65, 0.01}, {1.00, 90, 0.40, 0.12, 70, 0.02}, {1.25, 95, 0.45, 0.14, 75, 0.03}, {1.50, 100, 0.50, 0.16, 80, 0.04}, {2.0, 105, 0.55, 0.18, 85, 0.05},{0.25, 110, 0.60, 0.20, 90, 0.06}};

	batch5.MultiFactorPricer(scenarios1); 

	cout << endl;

	// Calculating and printing greeks
	EuroOption batch6(0.5, 0.36, 0.1, 0.1, 105, 100);
	cout << "Delta of Batch 6 call option: " << batch6.CallDelta() << endl;
	cout << "Delta of Batch 6 put option: " << batch6.PutDelta() << endl;
	cout << "Gamma of Batch 6 call and put option: " << batch6.Gamma() << endl << endl;

	// Test how changing underlying asset price can affect greeks
	cout << "On changing one parameter affecting option sensitivity - " << endl;
	batch6.SingleFactorGreek(85, 105, 1);

	cout << endl;

	// Test how changing multiple parameters affects option sensitivity using multiple scenarios for a batch
	cout << "On changing multiple parameters affecting option sensitivity - " << endl;
	vector<vector<double>> scenarios2 = { {0.25, 85, 0.30, 0.1, 85, 0.0}, {0.50, 80, 0.35, 0.1, 105, 0.01}, {1.00, 95, 0.40, 0.12, 90, 0.02}, {1.25, 90, 0.45, 0.14, 95, 0.03}, {1.50, 110, 0.50, 0.16, 100, 0.04}, {2.0, 105, 0.55, 0.18, 85, 0.05},{0.25, 100, 0.60, 0.20, 95, 0.06} };

	batch6.MultiFactorGreek(scenarios2);

	// Test how different value of h, has an effect on accuracy of greeks
	cout << endl << "On slowly increasing the difference value(h), value of delta/gamma become less accurate. See below: " << endl;
	batch6.SingleFactorApproximation(0.0001, 0.01, 0.001);

	cout << endl;

	AmericanOption Amop(0.1, 0.1, 0.02, 110, 100);

	cout << "Call Price of American Option: " << Amop.AmCallPrice() << endl; // calculating and printing the price of american call option
	cout << "Put Price of American Option: " << Amop.AmPutPrice() << endl << endl; // calculating and printing the price of american call option

	// Test how changing underlying price can affect option pricing for american options
	cout << "On changing one parameter affecting option pricing - " << endl; 
	Amop.SingleFactorPricer(105, 110, 1);

	cout << endl;

	// Test how changing multiple parameters affect american option pricing by creating different scenarios for a batch 
	cout << "On changing multiple parameters affecting option pricing - " << endl;
	vector<vector<double>> scenarios3 = { {80, 0.3, 0.08, 95, 0.0}, {120, 0.25, 0.1, 100, 0.01}, {90, 0.10, 0.12, 105, 0.02}, {95, 0.15, 0.14, 90, 0.03}, {100, 0.05, 0.16, 110, 0.04}, {105, 0.25, 0.18, 115, 0.05},{110, 0.20, 0.20, 120, 0.06} };

	Amop.MultiFactorPricer(scenarios3);*/
}

