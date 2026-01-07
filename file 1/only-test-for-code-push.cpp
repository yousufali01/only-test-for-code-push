#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;

// ---------------------------
// Transaction Class
// ---------------------------
class Transaction {
public:
    string type;       // "Income" or "Expense"
    string category;   // "Food", "Salary", etc.
    string date;       // "YYYY-MM-DD"
    string account;    // "Bank", "Cash", "Bkash"
    double amount;

    Transaction(string t, string cat, string d, string acc, double amt)
        : type(t), category(cat), date(d), account(acc), amount(amt) {}

    void display() const {
        cout << left << setw(10) << date
             << setw(8) << type
             << setw(12) << category
             << setw(10) << account
             << fixed << setprecision(2) << amount << endl;
    }
};

// ---------------------------
// Account Manager Class
// ---------------------------
class AccountManager {
private:
    map<string, double> balances;

public:
    AccountManager() {
        // Initialize default accounts
        balances["Bank"] = 0.0;
        balances["Cash"] = 0.0;
        balances["Bkash"] = 0.0;
    }

    void addAmount(const string& account, double amount) {
        balances[account] += amount;
    }

    bool spendAmount(const string& account, double amount) {
        if (balances[account] >= amount) {
            balances[account] -= amount;
            return true;
        } else {
            cout << "Insufficient balance in " << account << "!\n";
            return false;
        }
    }

    void displayBalances() const {
        cout << "\n---- Account Balances ----\n";
        for (map<string,double>::const_iterator it = balances.begin(); it != balances.end(); ++it)
            cout << it->first << ": " << fixed << setprecision(2) << it->second << endl;
        cout << "-------------------------\n";
    }

    map<string, double>& getBalances() {
        return balances;
    }
};

// ---------------------------
// Finance Tracker Class
// ---------------------------
class FinanceTracker {
private:
    AccountManager accountManager;
    vector<Transaction> transactions;
    double monthlyBudget = 0.0;

public:
    void setBudget(double budget) {
        monthlyBudget = budget;
    }

    void addTransaction() {
        string type, category, date, account;
        double amount;

        // Input type
        cout << "Enter type (Income/Expense): ";
        cin >> type;
        while (type != "Income" && type != "Expense") {
            cout << "Invalid input. Enter 'Income' or 'Expense': ";
            cin >> type;
        }

        // Input category
        cout << "Enter category: ";
        cin >> category;

        // Input date
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> date;

        // Input account
       #include <algorithm> // Add at the top for transform()

// ...

cout << "Choose account (Bank/Cash/Bkash): ";
cin >> account;

// Convert input to lowercase for comparison
string accountLower = account;
transform(accountLower.begin(), accountLower.end(), accountLower.begin(), ::tolower);

while (accountLower != "bank" && accountLower != "cash" && accountLower != "bkash") {
    cout << "Invalid account. Choose Bank, Cash, or Bkash: ";
    cin >> account;
    transform(account.begin(), account.end(), accountLower.begin(), ::tolower);
}

// Normalize account name to proper capitalization
if (accountLower == "bank") account = "Bank";
else if (accountLower == "cash") account = "Cash";
else if (accountLower == "bkash") account = "Bkash";


        // Input amount with validation
        cout << "Enter amount: ";
        while (!(cin >> amount) || amount < 0) {
            cout << "Invalid input. Enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Add transaction
        transactions.emplace_back(type, category, date, account, amount);

        // Update account
        if (type == "Income") {
            accountManager.addAmount(account, amount);
        } else { // Expense
            if (accountManager.spendAmount(account, amount)) {
                double totalExpense = getMonthlyExpense();
                if (monthlyBudget > 0 && totalExpense > monthlyBudget) {
                    cout << "\n*** OVERSPENDING ALERT! ***\n";
                }
            }
        }

        cout << "Transaction added successfully!\n";
    }

    void displayTransactions() const {
        cout << "\n---- Transactions ----\n";
        cout << left << setw(10) << "Date"
             << setw(8) << "Type"
             << setw(12) << "Category"
             << setw(10) << "Account"
             << "Amount\n";
        cout << "------------------------------------------\n";
        for (size_t i = 0; i < transactions.size(); ++i)
            transactions[i].display();
        cout << "--------------------\n";
    }

    double getMonthlyIncome() const {
        double total = 0.0;
        for (size_t i = 0; i < transactions.size(); ++i)
            if (transactions[i].type == "Income") total += transactions[i].amount;
        return total;
    }

    double getMonthlyExpense() const {
        double total = 0.0;
        for (size_t i = 0; i < transactions.size(); ++i)
            if (transactions[i].type == "Expense") total += transactions[i].amount;
        return total;
    }

    void monthlySummary() const {
        double income = getMonthlyIncome();
        double expense = getMonthlyExpense();
        double net = income - expense;

        cout << "\n---- Monthly Summary ----\n";
        cout << "Total Income: " << fixed << setprecision(2) << income << endl;
        cout << "Total Expense: " << fixed << setprecision(2) << expense << endl;
        cout << "Net Savings: " << fixed << setprecision(2) << net << endl;
        cout << "-------------------------\n";
    }

    void displayBalances() const {
        accountManager.displayBalances();
    }

    void saveData() {
        ofstream file("finance_data.txt");
        if (file.is_open()) {
            // Save accounts
            map<string,double>& balances = accountManager.getBalances();
            for (map<string,double>::iterator it = balances.begin(); it != balances.end(); ++it) {
                file << it->first << " " << it->second << "\n";
            }
            // Save transactions
            for (size_t i = 0; i < transactions.size(); ++i) {
                file << transactions[i].type << " "
                     << transactions[i].category << " "
                     << transactions[i].date << " "
                     << transactions[i].account << " "
                     << transactions[i].amount << "\n";
            }
            file.close();
        }
    }

    void loadData() {
        ifstream file("finance_data.txt");
        if (file.is_open()) {
            // Load accounts first
            map<string,double>& balances = accountManager.getBalances();
            string line;
            string acc;
            double bal;

            // Load balances (3 lines)
            for (map<string,double>::iterator it = balances.begin(); it != balances.end(); ++it) {
                if (file >> acc >> bal) {
                    balances[it->first] = bal;
                }
            }

            // Load transactions
            transactions.clear();
            string type, category, date, account;
            double amount;
            while (file >> type >> category >> date >> account >> amount) {
                transactions.emplace_back(type, category, date, account, amount);
            }
            file.close();
        }
    }
};

// ---------------------------
// Main Menu
// ---------------------------
int main() {
    FinanceTracker tracker;
    tracker.loadData();

    int choice;
    do {
        cout << "\n--- Personal Finance Tracker ---\n";
        cout << "1. Add Transaction\n";
        cout << "2. Display Transactions\n";
        cout << "3. Set Monthly Budget\n";
        cout << "4. Monthly Summary\n";
        cout << "5. Display Account Balances\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            tracker.addTransaction();
            break;
        case 2:
            tracker.displayTransactions();
            break;
        case 3: {
            double budget;
            cout << "Enter monthly budget: ";
            cin >> budget;
            tracker.setBudget(budget);
            break;
        }
        case 4:
            tracker.monthlySummary();
            break;
        case 5:
            tracker.displayBalances();
            break;
        case 0:
            tracker.saveData();
            cout << "Data saved. Exiting...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 0);//hello

    return 0;
}