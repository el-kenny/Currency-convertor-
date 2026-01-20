#include "Currency.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

// Constructor
Currency::Currency(string code, double rate, string name) {
    currencyCode = code;
    exchangeRate = rate;
    currencyName = name;
}

// Accessor functions
string Currency::getCurrencyCode() const {
    return currencyCode;
}

double Currency::getExchangeRate() const {
    return exchangeRate;
}

string Currency::getCurrencyName() const {
    return currencyName;
}

// Mutator functions
void Currency::setCurrencyCode(string code) {
    currencyCode = code;
}

void Currency::setExchangeRate(double rate) {
    exchangeRate = rate;
}

void Currency::setCurrencyName(string name) {
    currencyName = name;
}

// Display function
void Currency::display() const {
    cout << left << setw(10) << currencyCode
        << setw(25) << currencyName
        << "Rate: " << fixed << setprecision(4) << exchangeRate << endl;
}

// Conversion functions using math operations
double Currency::convertToUSD(double amount) const {
    return amount / exchangeRate;
}

double Currency::convertFromUSD(double amount) const {
    return amount * exchangeRate;
}

// Constructor
CurrencyConverter::CurrencyConverter() : Currency() {
    lastConversion = "No conversions yet";
}

// Overloaded conversion function (3 parameters)
double CurrencyConverter::convert(double amount, string fromCurrency, string toCurrency) {
    // Validate input amount
    if (amount < 0) {
        throw invalid_argument("Amount cannot be negative");
    }

    // Check if currencies exist
    if (!findCurrency(fromCurrency)) {
        throw invalid_argument("Source currency not found: " + fromCurrency);
    }
    if (!findCurrency(toCurrency)) {
        throw invalid_argument("Target currency not found: " + toCurrency);
    }

    // Get exchange rates
    double fromRate = currencyDatabase[fromCurrency].first;
    double toRate = currencyDatabase[toCurrency].first;

    // Convert: amount -> USD -> target currency
    double amountInUSD = amount / fromRate;
    double result = amountInUSD * toRate;

    // Apply rounding using ceil/floor
    result = floor(result * 100 + 0.5) / 100.0;

    // Store last conversion info
    lastConversion = to_string(amount) + " " + fromCurrency +
        " = " + to_string(result) + " " + toCurrency;

    return result;
}

// Overloaded conversion function 
double CurrencyConverter::convert(double amount, string toCurrency) {
    return convert(amount, "USD", toCurrency);
}

// Add currency to database
void CurrencyConverter::addCurrency(string code, double rate, string name) {
    currencyDatabase[code] = make_pair(rate, name);
}

// Display all currencies
void CurrencyConverter::showAllCurrencies() const {
    cout << "\n========================================" << endl;
    cout << "AVAILABLE CURRENCIES" << endl;
    cout << "========================================" << endl;
    cout << left << setw(10) << "Code"
        << setw(25) << "Currency Name"
        << "Exchange Rate (to USD)" << endl;
    cout << "----------------------------------------" << endl;

    for (const auto& currency : currencyDatabase) {
        cout << left << setw(10) << currency.first
            << setw(25) << currency.second.second
            << fixed << setprecision(4) << currency.second.first << endl;
    }
    cout << "========================================\n" << endl;
}

// Find currency by code
bool CurrencyConverter::findCurrency(string code) const {
    return currencyDatabase.find(code) != currencyDatabase.end();
}

// Get currency rate
double CurrencyConverter::getRate(string code) const {
    if (findCurrency(code)) {
        return currencyDatabase.at(code).first;
    }
    return -1.0;
}

// Get currency name
string CurrencyConverter::getName(string code) const {
    if (findCurrency(code)) {
        return currencyDatabase.at(code).second;
    }
    return "Unknown";
}

// Save to binary file
void CurrencyConverter::saveToBinaryFile(string filename) const {
    ofstream outFile(filename, ios::binary);

    if (!outFile) {
        throw runtime_error("Cannot open file for writing: " + filename);
    }

    // Write the number of currencies
    int count = currencyDatabase.size();
    outFile.write(reinterpret_cast<const char*>(&count), sizeof(count));

    // Write each currency
    for (const auto& currency : currencyDatabase) {
        // Write code length and code
        int codeLength = currency.first.length();
        outFile.write(reinterpret_cast<const char*>(&codeLength), sizeof(codeLength));
        outFile.write(currency.first.c_str(), codeLength);

        // Write rate
        double rate = currency.second.first;
        outFile.write(reinterpret_cast<const char*>(&rate), sizeof(rate));

        // Write name length and name
        int nameLength = currency.second.second.length();
        outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        outFile.write(currency.second.second.c_str(), nameLength);
    }

    outFile.close();
    cout << "Data saved to " << filename << " successfully!" << endl;
}

// Load from binary file
void CurrencyConverter::loadFromBinaryFile(string filename) {
    ifstream inFile(filename, ios::binary);

    if (!inFile) {
        throw runtime_error("Cannot open file for reading: " + filename);
    }

    currencyDatabase.clear();

    // Read the number of currencies
    int count;
    inFile.read(reinterpret_cast<char*>(&count), sizeof(count));

    // Read each currency
    for (int i = 0; i < count; i++) {
        // Read code
        int codeLength;
        inFile.read(reinterpret_cast<char*>(&codeLength), sizeof(codeLength));
        char* codeBuffer = new char[codeLength + 1];
        inFile.read(codeBuffer, codeLength);
        codeBuffer[codeLength] = '\0';
        string code = codeBuffer;
        delete[] codeBuffer;

        // Read rate
        double rate;
        inFile.read(reinterpret_cast<char*>(&rate), sizeof(rate));

        // Read name
        int nameLength;
        inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        char* nameBuffer = new char[nameLength + 1];
        inFile.read(nameBuffer, nameLength);
        nameBuffer[nameLength] = '\0';
        string name = nameBuffer;
        delete[] nameBuffer;

        currencyDatabase[code] = make_pair(rate, name);
    }

    inFile.close();
    cout << "Data loaded from " << filename << " successfully!" << endl;
}

// Get last conversion info
string CurrencyConverter::getLastConversion() const {
    return lastConversion;
}

// Load currencies from text file
void CurrencyConverter::loadFromTextFile(string filename) {
    ifstream inFile(filename);

    if (!inFile) {
        throw runtime_error("Cannot open file for reading: " + filename);
    }

    currencyDatabase.clear();

    string code, name;
    double rate;

    // Read each line
    while (inFile >> code >> rate) {
        inFile.ignore(); // Ignore the space after rate
        getline(inFile, name);
        currencyDatabase[code] = make_pair(rate, name);
    }

    inFile.close();
    cout << "Loaded " << currencyDatabase.size() << " currencies from " << filename << " successfully!" << endl;
}
