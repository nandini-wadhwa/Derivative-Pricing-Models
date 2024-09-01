// Group A & B: Header file for Option Pricing and Sensitivity measurement of Plain (European) options 

#ifndef EuroOptionHPP
#define EuroOptionHPP

#include <iostream>
#include <cmath>
#include <boost/math/distributions/normal.hpp>
#include <vector>
using namespace std;
using namespace boost::math;

namespace Options
{
		class EuroOption
		{
		private:
			double T; // time
			double sig; // volatility
			double r; // rate
			double S; // Underlying Price
			double K; // Strike Price
			double q; // dividend

		public:
			EuroOption(); // default constructor
			EuroOption(double time, double sigma, double rate, double div, double a_p, double s_p); // parameterized constructor
			EuroOption(const EuroOption& eop); // copy constructor
			~EuroOption(); // destructor

			double d1() const; // function to calculate d1
			double d2() const; // function to calculate d2
			double EuroCallPrice() const; // function to calculate price of European Call Option
			double EuroPutPrice() const; // function to calculate price of European Put Option
			double EuroCallPrice(double underlying); // function to calculate price of European Call Option, with dependancy on change of underlying price
			double EuroPutPrice(double underlying); // function to calculate price of European Put Option, with dependancy on change of underlying price

			// Setter Functions for parameters affecting option price
			void SetTime(double time); 
			void SetVolatility(double vol);
			void SetRate(double rate);
			void SetUnderlying(double underlying);
			void SetStrike(double strike);
			void SetDividend(double div);

			double CallParityPrice() const; // function to return back call price derived from Put-Call Parity expression
			double PutParityPrice() const; //  function to return back put price derived from Put-Call Parity expression
			void ParityCheck() const; // function to check if a set of call and put option prices validate the Put-Call Parity

			vector<double> FactorCNG(double start, double end, double h); // global function to create a vector of doubles seperated by size h

			void SingleFactorPricer(double start, double end, double h); // function to create and print a vector of changed option prices, having changed asset price
			void MultiFactorPricer(vector<vector<double>> multiFactor); // function to create anf print a vector of changed option prices, having changing multiple factors affecting option pricing

			double CallDelta() const; // function to calculate delta of plain european call option
			double CallDelta(double underlying); // function to calculate delta of plain european call option, with dependancy on changing asset price
			double PutDelta() const; // function to calculate delta of plain european put option
			double PutDelta(double underlying); // function to calculate delta of plain european put option, with dependancy on changing asset price
			double Gamma() const; // function to calculate gamma of put and call options
			double Vega() const; // function to calculate vega of put and call option
			double CallTheta() const; // function to calculate theta of plain european call option
			double PutTheta() const; // function to calculate theta of plain european put option

			void SingleFactorGreek(double start, double end, double h); // function to create and print a vector of changed option deltas, having changed asset price
			void MultiFactorGreek(vector<vector<double>> multiFactor); // function to create anf print a vector of changed option deltas, having changing multiple factors affecting option pricing

			double CallDeltaApx(double S, double h); // function to calculate delta of call option using taylor's approximation
			double PutDeltaApx(double S, double h); // function to calculate delta of put option using taylor's approximation
			double GammaApx(double S, double h); // function to calculate gamma using taylor's approximation
			void SingleFactorApproximation(double start, double end, double h); // function to create and print a vector of changed option deltas approximations, having changed asset price
		};
}

#endif