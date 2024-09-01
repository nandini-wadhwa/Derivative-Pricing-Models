// Group A & B: Source file for Option Pricing and Sensitivity Measurement of Plain (European) options

#include "EuroOption.h"
#include <iostream>
#include <cmath>
#include <boost/math/distributions/normal.hpp>
#include <vector>
using namespace std;
using namespace boost::math;

namespace Options
{
		EuroOption::EuroOption() : T(0.0), sig(0.0), r(0.0), S(0.0), K(0.0), q(0.0)  // default constructor
		{}

		EuroOption::EuroOption(double time, double sigma, double rate, double div, double a_p, double s_p) : T(time), sig(sigma), r(rate), S(a_p), K(s_p), q(div) // parameterized constructor
		{}

		EuroOption::EuroOption(const EuroOption& eop) : T(eop.T), sig(eop.sig), r(eop.r), S(eop.S), K(eop.K), q(eop.q) // copy constructor
		{}

		EuroOption::~EuroOption() // destructor
		{}

		double EuroOption::d1() const // function to calculate d1
		{
			double d1 = (log(S / K) + (r - q + ((sig * sig) / 2)) * T) / (sig * sqrt(T));
			return d1;
		}

		double EuroOption::d2() const // function to calculate d2
		{
			double d2 = d1() - (sig * sqrt(T));
			return d2;
		}

		double EuroOption::EuroCallPrice() const // function to calculate price of European Call Option
		{
			normal_distribution<> myNormal(0, 1);
			double C = (S * exp(-q * T) * cdf(myNormal, d1())) - (K * exp(-r * T) * cdf(myNormal, d2()));
			return C;
		}

		double EuroOption::EuroPutPrice() const // function to calculate price of European Put Option
		{
			normal_distribution<> myNormal(0, 1);
			double P = (K * exp(-r * T) * cdf(myNormal, -d2())) - (S * exp(-q * T) * cdf(myNormal, -d1()));
			return P;
		}

		double EuroOption::EuroCallPrice(double underlying) // function to calculate price of European Call Option, with dependancy on change of underlying price
		{
			S = underlying;
			normal_distribution<> myNormal(0, 1);
			double C = (S * exp(-q * T) * cdf(myNormal, d1())) - (K * exp(-r * T) * cdf(myNormal, d2()));
			return C;
		}

		double EuroOption::EuroPutPrice(double underlying) // function to calculate price of European Put Option, with dependancy on change of underlying price
		{
			S = underlying;
			normal_distribution<> myNormal(0, 1);
			double P = (K * exp(-r * T) * cdf(myNormal, -d2())) - (S * exp(-q * T) * cdf(myNormal, -d1()));
			return P;
		}

		double EuroOption::CallParityPrice() const // function to return back call price derived from Put-Call Parity expression
		{
			double C_P = EuroPutPrice() + S - (K * exp(-r * T));
			return C_P;
		}

		double EuroOption::PutParityPrice() const //  function to return back put price derived from Put-Call Parity expression
		{
			double P_P = EuroCallPrice() + (K * exp(-r * T)) - S;
			return P_P;
		}

		void EuroOption::ParityCheck() const // function to check if a set of call and put option prices validate the Put-Call Parity
		{
			double LHS = EuroCallPrice() + (K * exp(-r * T));
			double RHS = EuroPutPrice() + S;

			if (LHS = RHS)
			{
				cout << "Put-Call Parity stands Validated" << endl;
			}
			else
			{
				cout << "Put-Call Parity stands Violated" << endl;
			}
		}


		// Setter Functions for parameters affecting option price
		void EuroOption::SetTime(double time)
		{
			T = time;
		}

		void EuroOption::SetVolatility(double vol)
		{
			sig = vol;
		}

		void EuroOption::SetRate(double rate)
		{
			r = rate;
		}
		void EuroOption::SetUnderlying(double underlying)
		{
			S = underlying;
		}
		void EuroOption::SetStrike(double strike)
		{
			K = strike;
		}
		void EuroOption::SetDividend(double div)
		{
			q = div;
		}

		// Global function to create a vector of doubles separated by size h
		vector<double> EuroOption::FactorCNG(double start, double end, double h)
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
		void EuroOption::SingleFactorPricer(double start, double end, double h)
		{
			// Create a vector of factor values using the FactorCNG function
			vector<double> F = FactorCNG(start, end, h);

			// Create empty vectors to store the call and put prices
			vector<double> CallVector;
			vector<double> PutVector;

			// Loop through the factor vector, and for each value, calculate the call and put prices
			for (auto it = F.begin(); it != F.end(); it += h)
			{
				// Calculate the call price using the EuroCallPrice function
				CallVector.push_back(EuroCallPrice(*it));

				// Calculate the put price using the EuroPutPrice function
				PutVector.push_back(EuroPutPrice(*it));
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
		void EuroOption::MultiFactorPricer(vector<vector<double>> multiFactor)
		{
			// Create empty vectors to store the call and put prices for each scenario
			vector<double> callValues;
			vector<double> putValues;

			// Print the header for the output table
			cout << setw(10) << "Time" << setw(10) << "Strike" << setw(15) << "Volatility" << setw(10) << "Rate" << setw(15) << "Underlying" << setw(10) << "Dividend" << setw(15) << "Call Price" << setw(15) << "Put Price" << endl;

			// Loop through the multi-factor scenarios
			for (auto it = multiFactor.begin(); it != multiFactor.end(); ++it)
			{
				// Set the parameters for the current scenario
				SetTime(it->at(0));
				SetStrike(it->at(1));
				SetVolatility(it->at(2));
				SetRate(it->at(3));
				SetUnderlying(it->at(4));
				SetDividend(it->at(5));

				// Price the European call and put options using the EuroCallPrice and EuroPutPrice functions
				double callPrice = EuroCallPrice();
				double putPrice = EuroPutPrice();

				// Print the scenario parameters and option prices
				cout << setw(10) << it->at(0) << setw(10) << it->at(1) << setw(15) << it->at(2) << setw(10) << it->at(3) << setw(15) << it->at(4) << setw(10) << it->at(5) << setw(15) << callPrice << setw(15) << putPrice << endl;

				// Add the call and put prices to the result vectors
				callValues.push_back(callPrice);
				putValues.push_back(putPrice);
			}
		}

		double EuroOption::CallDelta() const // function to calculate delta of plain european call option
		{
			normal_distribution<> myNormal(0, 1);
			double delta = exp(-q * T) * cdf(myNormal, d1());
			return delta;
		}

		double EuroOption::CallDelta(double underlying) // function to calculate delta of plain european call option, with dependancy on changing asset price
		{
			S = underlying;
			normal_distribution<> myNormal(0, 1);
			double delta = exp(-q * T) * cdf(myNormal, d1());
			return delta;
		}

		double EuroOption::PutDelta() const // function to calculate delta of plain european put option
		{
			normal_distribution<> myNormal(0, 1);
			double delta = exp(-q * T) * (cdf(myNormal, d1()) - 1);
			return delta;
		}

		double EuroOption::PutDelta(double underlying) // function to calculate delta of plain european put option, with dependancy on changing asset price
		{
			S = underlying;
			normal_distribution<> myNormal(0, 1);
			double delta = exp(-q * T) * (cdf(myNormal, d1()) - 1);
			return delta;
		}

		double EuroOption::Gamma() const // function to calculate gamma of put and call options
		{
			normal_distribution<> myNormal(0, 1);
			double gamma = ((exp(-q * T)) / (S * sig * sqrt(T))) * pdf(myNormal, d1());
			return gamma;
		}

		double EuroOption::Vega() const // function to calculate vega of put and call option
		{
			normal_distribution<> myNormal(0, 1);
			double vega = S * (exp(-q * T)) * sqrt(T) * pdf(myNormal, d1());
			return vega;
		}

		double EuroOption::CallTheta() const // function to calculate theta of plain european call option
		{
			normal_distribution<> myNormal(0, 1);
			double theta = -((S * sig * exp(-q * T)) / (2 * sqrt(T))) * pdf(myNormal, d1()) - (r * K * exp(-r * T) * cdf(myNormal, d2())) + (q * S * exp(-q * T) * cdf(myNormal, d1()));
			return theta;
		}

		double EuroOption::PutTheta() const // function to calculate theta of plain european put option
		{
			normal_distribution<> myNormal(0, 1);
			double theta = -((S * sig * exp(-q * T)) / (2 * sqrt(T))) * pdf(myNormal, d1()) + (r * K * exp(-r * T) * cdf(myNormal, -d2())) - (q * S * exp(-q * T) * cdf(myNormal, -d1()));
			return theta;
		}

		// Function to create and print a vector of changed option deltas, having changed asset price
		void EuroOption::SingleFactorGreek(double start, double end, double h)
		{
			// Create a vector of factor values using the FactorCNG function
			vector<double> F = FactorCNG(start, end, h);

			// Create empty vectors to store the call and put deltas
			vector<double> CallDelVector;
			vector<double> PutDelVector;

			// Loop through the factor vector, and for each value, calculate the call and put deltas
			for (auto it = F.begin(); it != F.end(); it += h)
			{
				// Calculate the call delta using the CallDelta function
				CallDelVector.push_back(CallDelta(*it));

				// Calculate the put delta using the PutDelta function
				PutDelVector.push_back(PutDelta(*it));
			}

			// Print the header for the output table
			cout << setw(15) << "Underlying Price" << setw(15) << "Call Delta" << setw(15) << "Put Delta" << endl;

			// Loop through the factor vector and print the underlying price, call delta, and put delta for each scenario
			for (int i = 0; i < F.size(); ++i)
			{
				cout << setw(15) << F[i] << setw(15) << CallDelVector[i] << setw(15) << PutDelVector[i] << endl;
			}
		}

		// Function to create and print a vector of changed option deltas, having changing multiple factors affecting option pricing
		void EuroOption::MultiFactorGreek(vector<vector<double>> multiFactor)
		{
			// Create empty vectors to store the call and put deltas for each scenario
			vector<double> calldelValues;
			vector<double> putdelValues;

			// Print the header for the output table
			cout << setw(10) << "Time" << setw(10) << "Strike" << setw(15) << "Volatility" << setw(10) << "Rate" << setw(15) << "Underlying" << setw(10) << "Dividend" << setw(15) << "Call Delta" << setw(15) << "Put Delta" << endl;

			// Loop through the multi-factor scenarios
			for (auto it = multiFactor.begin(); it != multiFactor.end(); ++it)
			{
				// Set the parameters for the current scenario
				SetTime(it->at(0));
				SetStrike(it->at(1));
				SetVolatility(it->at(2));
				SetRate(it->at(3));
				SetUnderlying(it->at(4));
				SetDividend(it->at(5));

				// Calculate the call and put deltas using the CallDelta and PutDelta functions
				double calldel = CallDelta();
				double putdel = PutDelta();

				// Print the scenario parameters and option deltas
				cout << setw(10) << it->at(0) << setw(10) << it->at(1) << setw(15) << it->at(2) << setw(10) << it->at(3) << setw(15) << it->at(4) << setw(10) << it->at(5) << setw(15) << calldel << setw(15) << putdel << endl;

				// Add the call and put deltas to the result vectors
				calldelValues.push_back(calldel);
				putdelValues.push_back(putdel);
			}
		}
		
		double EuroOption::CallDeltaApx(double S, double h) // function to calculate delta of call option using taylor's approximation
		{
			double a_delta = (EuroCallPrice(S + h) - EuroCallPrice(S - h)) / (2 * h);
			return a_delta;
		}

		double EuroOption::PutDeltaApx(double S, double h) // function to calculate delta of put option using taylor's approximation
		{
			double a_delta = (EuroPutPrice(S + h) - EuroPutPrice(S - h)) / (2 * h);
			return a_delta;
		}

		double EuroOption::GammaApx(double S, double h) // function to calculate gamma using taylor's approximation
		{
			double a_gamma = (EuroCallPrice(S + h) - (2 * EuroCallPrice(S)) + EuroCallPrice(S - h)) / (2 * h);
			return a_gamma;
		}

		// Function to perform single-factor approximation of option Greeks (delta and gamma)
		void EuroOption::SingleFactorApproximation(double start, double end, double h)
		{
			// Create a vector of h values using the FactorCNG function
			vector<double> hValues = FactorCNG(start, end, h);

			// Create empty vectors to store the approximated call delta, put delta, and gamma values
			vector<double> callDelApxValues;
			vector<double> putDelApxValues;
			vector<double> gammaApxValues;

			// Print the header for the output table
			cout << setw(15) << "h" << setw(25) << "Call Delta Approximation" << setw(25) << "Put Delta Approximation" << setw(25) << "Gamma Approximation" << endl;

			// Loop through the h values and perform the approximation for each value
			for (auto it = hValues.begin(); it != hValues.end(); ++it)
			{
				// Calculate the approximated call delta using the CallDeltaApx function
				callDelApxValues.push_back(CallDeltaApx(S, *it));

				// Calculate the approximated put delta using the PutDeltaApx function
				putDelApxValues.push_back(PutDeltaApx(S, *it));

				// Calculate the approximated gamma using the GammaApx function
				gammaApxValues.push_back(GammaApx(S, *it));

				// Print the h value and the approximated values for call delta, put delta, and gamma
				cout << setw(15) << *it << setw(25) << callDelApxValues.back() << setw(25) << putDelApxValues.back() << setw(25) << gammaApxValues.back() << endl;
			}
		}

}