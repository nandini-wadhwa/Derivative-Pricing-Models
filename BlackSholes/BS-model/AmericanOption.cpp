// Group A & B: Source file for Option Pricing and Sensitivity Measurement of Plain (European) options

#include "AmericanOption.h"
#include <iostream>
#include <cmath>
#include <boost/math/distributions/normal.hpp>
#include <vector>
using namespace std;
using namespace boost::math;

namespace Options
{
	AmericanOption::AmericanOption() : sig(0.0), r(0.0), S(0.0), K(0.0), b(0.0) // default constructor
	{}

	AmericanOption::AmericanOption(double sigma, double rate, double B, double a_p, double s_p) : sig(sigma), r(rate), S(a_p), K(s_p), b(B) // parameterized constructor
	{}

	AmericanOption::AmericanOption(const AmericanOption& aop) : sig(aop.sig), r(aop.r), S(aop.S), K(aop.K), b(aop.b) // copy constructor
	{}

	AmericanOption::~AmericanOption() // destructor
	{}

	double AmericanOption::y1() const // function to calculate value of y1
	{
		double y1 = (1.0/2.0) - (b / (sig * sig)) + sqrt(pow(((b / (sig * sig)) - (1.0 / 2.0)), 2) + (2 * r) / (sig * sig));
		return y1;
	}

	double AmericanOption::y2() const // function to calculate value of y2
	{
		double y2 = (1.0 / 2.0) - (b / (sig * sig)) - sqrt(pow(((b / (sig * sig)) - (1.0 / 2.0)), 2) + (2 * r) / (sig * sig));
		return y2;
	}

	double AmericanOption::AmCallPrice() const // function to calculate price of American Call Option
	{
		double C = (K / (y1() - 1)) * pow(((S / K) * ((y1() - 1) / y1())), y1());
		return C;
	}

	double AmericanOption::AmPutPrice() const// function to calculate price of American Put Option
	{
		double P = (K / (1 - y2())) * pow(((S / K) * ((y2() - 1) / y2())), y2());
		return P;
	}

	double AmericanOption::AmCallPrice(double underlying) // function to calculate price of American Call Option, with dependancy on change of underlying price
	{
		S = underlying;
		double C = (K / (y1() - 1)) * pow(((S / K) * ((y1() - 1) / y1())), y1());
		return C;
	}

	double AmericanOption::AmPutPrice(double underlying) // function to calculate price of American Put Option, with dependancy on change of underlying price
	{
		S = underlying;
		double P = (K / (1 - y2())) * pow(((S / K) * ((y2() - 1) / y2())), y2());
		return P;
	}

	// Setter Functions for Parameters affecting option prices

	void AmericanOption::SetVolatility(double vol) 
	{
		sig = vol;
	}

	void AmericanOption::SetRate(double rate)
	{
		r = rate;
	}
	void AmericanOption::SetUnderlying(double underlying)
	{
		S = underlying;
	}
	void AmericanOption::SetStrike(double strike)
	{
		K = strike;
	}
	void AmericanOption::SetB(double B)
	{
		b = B;
	}
	
	// Global function to create a vector of doubles separated by size h
	vector<double> AmericanOption::FactorCNG(double start, double end, double h)
	{
		// Create an empty vector to store the factor values
		vector<double> factor;

		// Loop from start to end with increment h, and add each value to the factor vector
		for (double i = start; i <= end; i += h)
		{
			factor.push_back(i);
		}

		// Return the completed factor vector
		return factor;
	}

	// Function to create and print a vector of changed option prices, having changed asset price
	void AmericanOption::SingleFactorPricer(double start, double end, double h)
	{
		// Create a vector of factor values using the FactorCNG function
		vector<double> F = FactorCNG(start, end, h);

		// Create empty vectors to store the call and put prices
		vector<double> CallVector;
		vector<double> PutVector;

		// Loop through the factor vector, and for each value, calculate the call and put prices
		for (auto it = F.begin(); it != F.end(); it += h)
		{
			// Calculate the call price using the AmCallPrice function
			CallVector.push_back(AmCallPrice(*it));

			// Calculate the put price using the AmPutPrice function
			PutVector.push_back(AmPutPrice(*it));
		}

		// Print the header for the output table
		cout << setw(15) << "Underlying Price" << setw(15) << "Call Price" << setw(15) << "Put Price" << endl;

		// Loop through the factor vector and print the underlying price, call price, and put price for each scenario
		for (int i = 0; i < F.size(); ++i)
		{
			cout << setw(15) << F[i] << setw(15) << CallVector[i] << setw(15) << PutVector[i] << endl;
		}
	}

	// Function to create and print a vector of changed option prices, having changing multiple factors affecting option pricing
	void AmericanOption::MultiFactorPricer(vector<vector<double>> multiFactor)
	{
		// Create empty vectors to store the call and put prices for each scenario
		vector<double> callValues;
		vector<double> putValues;

		// Print the header for the output table
		cout << setw(10) << "Strike" << setw(15) << "Volatility" << setw(10) << "Rate" << setw(15) << "Underlying" << setw(10) << "B-value" << setw(15) << "Call Price" << setw(15) << "Put Price" << endl;

		// Loop through the multi-factor scenarios
		for (auto it = multiFactor.begin(); it != multiFactor.end(); ++it)
		{
			// Set the parameters for the current scenario
			SetStrike(it->at(0));
			SetVolatility(it->at(1));
			SetRate(it->at(2));
			SetUnderlying(it->at(3));
			SetB(it->at(4));

			// Price the European call and put options using the AmCallPrice and AmPutPrice functions
			double callPrice = AmCallPrice();
			double putPrice = AmPutPrice();

			// Print the scenario parameters and option prices
			cout << setw(10) << it->at(0) << setw(10) << it->at(1) << setw(15) << it->at(2) << setw(10) << it->at(3) << setw(15) << it->at(4) << setw(15) << callPrice << setw(15) << putPrice << endl;

			// Add the call and put prices to the result vectors
			callValues.push_back(callPrice);
			putValues.push_back(putPrice);
		}
	}

}