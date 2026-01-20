// This class handles currency conversion operations with file storage

#ifndef CURRENCY_H
#define CURRENCY_H

#include <string>
#include <map>
using namespace std;

// Base class for currency operations
class Currency {
protected:
    string currencyCode;
    double exchangeRate;
    string currencyName;

public:
    // Constructor
    Currency(string code = "USD", double rate = 1.0, string name = "US Dollar");

    // Accessor functions (get functions)
    string getCurrencyCode() const;
    double getExchangeRate() const;
    string getCurrencyName() const;

    // Mutator functions (set functions)
    void setCurrencyCode(string code);
    void setExchangeRate(double rate);
    void setCurrencyName(string name);

    // Display function
    void display() const;

    // Conversion function
    double convertToUSD(double amount) const;
    double convertFromUSD(double amount) const;
};

// Derived class with additional features
class CurrencyConverter : public Currency {
private:
    map<string, pair<double, string>> currencyDatabase;
    string lastConversion;

public:
    // Constructor
    CurrencyConverter();

    // Overloaded functions for conversion
    double convert(double amount, string fromCurrency, string toCurrency);
    double convert(double amount, string toCurrency); // Assumes USD as source

    // Add currency to database
    void addCurrency(string code, double rate, string name);

    // Display all currencies
    void showAllCurrencies() const;

    // Find currency by code
    bool findCurrency(string code) const;

    // Get currency rate
    double getRate(string code) const;

    // Get currency name
    string getName(string code) const;

    // Save to binary file
    void saveToBinaryFile(string filename) const;

    // Load from binary file
    void loadFromBinaryFile(string filename);

    // Load currencies from text file
    void loadFromTextFile(string filename);

    // Get last conversion info
    string getLastConversion() const;
};

#endif