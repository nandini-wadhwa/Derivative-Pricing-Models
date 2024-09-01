// Group A & B: Header file for Option Pricing and Sensitivity measurement of American options 

#ifndef AmericanOptionHPP
#define AmericanOptionHPP

#include <iostream>
#include <cmath>
#include <boost/math/distributions/normal.hpp>
#include <vector>
using namespace std;
using namespace boost::math;

namespace Options
{
	class AmericanOption
	{
	private:
		double sig; // volatility
		double r; // rate
		double S; // underlying asset price
		double K; // strike price
		double b; // cost of carry (r-q), where q is dividend 

	public:
		AmericanOption(); // default constructor
		AmericanOption( double sigma, double rate, double B, double a_p, double s_p); // parameterized constructor
		AmericanOption(const AmericanOption& aop); // copy constructor
		~AmericanOption(); // destructor

		double y1() const; // function to calculate value of y1
		double y2() const; // function to calculate value of y2
		double AmCallPrice() const; // function to calculate price of American Call Option
		double AmPutPrice() const; // function to calculate price of American Put Option
		double AmCallPrice(double underlying); // function to calculate price of American Call Option, with dependancy on change of underlying price
		double AmPutPrice(double underlying); // function to calculate price of American Put Option, with dependancy on change of underlying price

		void SetVolatility(double vol); // setter function for sigma
		void SetRate(double rate); // setter function for rate
		void SetUnderlying(double underlying); // setter function for underlying price
		void SetStrike(double strike); // setter function for strike price
		void SetB(double B); // setter function for cost of carry

		vector<double> FactorCNG(double start, double end, double h); // global function to create a vector of doubles seperated by size h

		void SingleFactorPricer(double start, double end, double h); // function to create and print a vector of changed option prices, having changed asset price
		void MultiFactorPricer(vector<vector<double>> multiFactor); // function to create anf print a vector of changed option prices, having changing multiple factors affecting option pricing
	};

}
#endif