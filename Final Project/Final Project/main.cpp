// Name: Mico Shumbusho
// Class: CIS 1202
// Date: 12/9/2025
// Description: A currency converter program that converts between multiple currencies and allows adding custom currencies.
#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <iomanip>
#include "Currency.h"

using namespace std;

// Function prototypes
void displayMenu();
void convertCurrency(CurrencyConverter& converter);
void addNewCurrency(CurrencyConverter& converter);
void clearInputStream();
string toUpperCase(string str);
bool validateAmount(double amount);
bool validateCurrencyCode(string code);

int main() {
    CurrencyConverter converter;
    int choice;
    bool running = true;

    // Display program instructions
    cout << "\n============================================" << endl;
    cout << "  CURRENCY CONVERTER PROGRAM" << endl;
    cout << "============================================" << endl;
    cout << "\nWELCOME! This program allows you to:" << endl;
    cout << "  1. Convert between different currencies" << endl;
    cout << "  2. View all available currencies" << endl;
    cout << "  3. Add custom currencies" << endl;
    cout << "\nAll currency codes should be in 3-letter format (e.g., USD, EUR)" << endl;
    cout << "Exchange rates are relative to USD (1 USD = X currency)" << endl;
    cout << "============================================\n" << endl;

    // Load currencies from text file on startup
    try {
        converter.loadFromTextFile("currencies.txt");
        cout << endl;
    }
    catch (const runtime_error& e) {
        cout << "Note: Could not load currencies.txt. Please load currency data manually." << endl;
        cout << "Make sure currencies.txt exists in the same folder as the program.\n" << endl;
    }

    // Main program loop with exception handling
    while (running) {
        try {
            displayMenu();

            // Input validation for menu choice
            while (!(cin >> choice)) {
                clearInputStream();
                cout << "\nInvalid input! Please enter a number: ";
            }
            clearInputStream();

            cout << endl;

            switch (choice) {
            case 1:
                convertCurrency(converter);
                break;

            case 2:
                converter.showAllCurrencies();
                break;

            case 3:
                addNewCurrency(converter);
                break;

            case 4:
                cout << "Last Conversion: " << converter.getLastConversion() << endl << endl;
                break;

            case 5:
                cout << "Thank you for using Currency Converter!" << endl;
                cout << "Goodbye!" << endl;
                running = false;
                break;

            default:
                cout << "Invalid choice! Please select 1-5." << endl << endl;
            }
        }
        catch (const invalid_argument& e) {
            cout << "\nERROR: " << e.what() << endl << endl;
        }
        catch (const runtime_error& e) {
            cout << "\nERROR: " << e.what() << endl << endl;
        }
        catch (...) {
            cout << "\nAn unexpected error occurred!" << endl << endl;
        }
    }

    return 0;
}

// Display main menu
void displayMenu() {
    cout << "========== MAIN MENU ==========" << endl;
    cout << "1. Convert Currency" << endl;
    cout << "2. View All Currencies" << endl;
    cout << "3. Add New Currency" << endl;
    cout << "4. View Last Conversion" << endl;
    cout << "5. Exit" << endl;
    cout << "===============================" << endl;
    cout << "Enter your choice (1-5): ";
}

// Convert currency function with validation
void convertCurrency(CurrencyConverter& converter) {
    double amount;
    string fromCurrency, toCurrency;

    cout << "========== CURRENCY CONVERSION ==========" << endl;

    // Get and validate amount
    cout << "Enter amount to convert: ";
    while (!(cin >> amount) || !validateAmount(amount)) {
        clearInputStream();
        cout << "Invalid amount! Please enter a positive number: ";
    }
    clearInputStream();

    // Get and validate source currency
    cout << "Enter source currency code (e.g., USD): ";
    getline(cin, fromCurrency);
    fromCurrency = toUpperCase(fromCurrency);

    while (!validateCurrencyCode(fromCurrency) || !converter.findCurrency(fromCurrency)) {
        if (!validateCurrencyCode(fromCurrency)) {
            cout << "Invalid format! Enter 3-letter code: ";
        }
        else {
            cout << "Currency not found! Enter valid code: ";
        }
        getline(cin, fromCurrency);
        fromCurrency = toUpperCase(fromCurrency);
    }

    // Get and validate target currency
    cout << "Enter target currency code (e.g., EUR): ";
    getline(cin, toCurrency);
    toCurrency = toUpperCase(toCurrency);

    while (!validateCurrencyCode(toCurrency) || !converter.findCurrency(toCurrency)) {
        if (!validateCurrencyCode(toCurrency)) {
            cout << "Invalid format! Enter 3-letter code: ";
        }
        else {
            cout << "Currency not found! Enter valid code: ";
        }
        getline(cin, toCurrency);
        toCurrency = toUpperCase(toCurrency);
    }

    // Perform conversion
    double result = converter.convert(amount, fromCurrency, toCurrency);

    // Display formatted result
    cout << "\n========================================" << endl;
    cout << "CONVERSION RESULT" << endl;
    cout << "========================================" << endl;
    cout << fixed << setprecision(2);
    cout << amount << " " << fromCurrency << " (" << converter.getName(fromCurrency) << ")" << endl;
    cout << "  = " << result << " " << toCurrency << " (" << converter.getName(toCurrency) << ")" << endl;
    cout << "========================================\n" << endl;
}

// Add new currency function with validation
void addNewCurrency(CurrencyConverter& converter) {
    string code, name;
    double rate;

    cout << "========== ADD NEW CURRENCY ==========" << endl;

    // Get currency code
    cout << "Enter 3-letter currency code: ";
    getline(cin, code);
    code = toUpperCase(code);

    while (!validateCurrencyCode(code)) {
        cout << "Invalid format! Enter 3-letter code: ";
        getline(cin, code);
        code = toUpperCase(code);
    }

    // Get currency name
    cout << "Enter currency name: ";
    getline(cin, name);

    // Get exchange rate with validation
    cout << "Enter exchange rate (1 USD = ? " << code << "): ";
    while (!(cin >> rate) || rate <= 0) {
        clearInputStream();
        cout << "Invalid rate! Enter positive number: ";
    }
    clearInputStream();

    // Add currency
    converter.addCurrency(code, rate, name);
    cout << "\nCurrency " << code << " added successfully!" << endl << endl;
}

// Clear input stream
void clearInputStream() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Convert string to uppercase
string toUpperCase(string str) {
    for (size_t i = 0; i < str.length(); i++) {
        str[i] = toupper(str[i]);
    }
    return str;
}

// Validate amount (must be positive)
bool validateAmount(double amount) {
    return amount > 0;
}

// Validate currency code (must be 3 letters)
bool validateCurrencyCode(string code) {
    if (code.length() != 3) {
        return false;
    }

    for (char c : code) {
        if (!isalpha(c)) {
            return false;
        }
    }

    return true;
}