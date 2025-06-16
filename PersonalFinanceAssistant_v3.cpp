
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

const string RED   = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN  = "\033[36m";
const string RESET = "\033[0m";

double readNonNegative(const string& label) {
    double v;
    while (true) {
        cout << label;
        if (cin >> v && v >= 0) return v;
        cout << RED << "Value must be ≥ 0. Try again.\n" << RESET;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

class emp {
private:
    string name;
    int age{};
    double salary{}, expense{}, savings{}, invest{};
    double salhike{}, exphike{}, savinc{}, invhike{}, expectedReturn{};
    double taxRate{}, inflationRate{};
    bool hikesSet{false};

public:
    void setData() {
        cout << CYAN << "\n[SETUP] Enter your data:\n" << RESET;
        cout << "Name: ";
        cin >> name;
        age = static_cast<int>(readNonNegative("Age: "));
        salary   = readNonNegative("Monthly Salary: ");
        expense  = readNonNegative("Monthly Expense: ");
        savings  = readNonNegative("Monthly Savings: ");
        invest   = readNonNegative("Monthly Investments: ");
    }
    void setRates() {
        cout << CYAN << "\n[SETUP] Enter tax & inflation:\n" << RESET;
        taxRate       = readNonNegative("Annual Tax Rate (% of income): ");
        inflationRate = readNonNegative("Annual Inflation Rate (%): ");
        expectedReturn = readNonNegative("Expected Annual Investment Return Rate (%): ");
    }

    void showData() const {
        cout << YELLOW << fixed << setprecision(2)
             << "\n[SUMMARY] Snapshot\n"
             << "Name: "        << name    << "\n"
             << "Age: "         << age     << "\n"
             << "Salary: ₹"     << salary  << "\n"
             << "Expenses: ₹"   << expense << "\n"
             << "Savings: ₹"    << savings << "\n"
             << "Investments: ₹"<< invest  << "\n"
             << "Net Balance: ₹"<< salary - expense - savings - invest
             << RESET << "\n";
    }

    void healthCheck() const {
        cout << CYAN << "\n[HEALTH CHECK]\n" << RESET;
        if (expense > salary) {
            cout << RED   << "Expense exceeds income by ₹" << expense - salary << ".\n" << RESET;
        } else if (salary - expense < savings) {
            cout << RED   << "Savings exceed disposable income. Check numbers.\n" << RESET;
        } else if (salary - expense - savings < invest) {
            cout << RED   << "Investments too high for remaining balance.\n" << RESET;
        } else {
            cout << GREEN << "Looks balanced so far.\n" << RESET;
        }
    }

    void setHikes() {
        cout << CYAN << "\n[INPUT] Estimated monthly hike percentages:\n" << RESET;
        salhike = readNonNegative("Salary hike (%): ");
        exphike = readNonNegative("Expense hike (%): ");
        savinc  = readNonNegative("Savings hike (%): ");
        invhike = readNonNegative("Investment hike (%): ");

        if (savinc > salhike * 2 || invhike > salhike * 2) {
            cout << RED << "Unrealistic! Savings/Investment hike cannot exceed twice the salary hike.\n" << RESET;
            hikesSet = false;
            return;
        }
        hikesSet = true;

        double ns  = salary  * (1 + salhike / 100);
        double ne  = expense * (1 + exphike / 100);
        double nsv = savings * (1 + savinc  / 100);
        double nin = invest  * (1 + invhike / 100);

        cout << YELLOW << fixed << setprecision(2)
             << "\n[ESTIMATE] Next month:\n"
             << "Salary: ₹"     << ns  << "\n"
             << "Expenses: ₹"   << ne  << "\n"
             << "Savings: ₹"    << nsv << "\n"
             << "Investments: ₹"<< nin << RESET << "\n";
    }

    void futureProjection() const {
        if (!hikesSet) {
            cout << RED << "Set hike percentages first (menu option 4).\n" << RESET;
            return;
        }
        int m;
        cout << CYAN << "\n[PROJECTION] Enter months to simulate: " << RESET;
        cin >> m;
        double monthlyInfl = inflationRate / 1200.0; 
        double salaryGrowth     = salary * pow(1 + salhike / 100.0, m);
        double salaryAfterTax   = salaryGrowth * (1 - taxRate / 100.0);
        double savingsGrowth    = savings * pow(1 + savinc / 100.0, m);
        double investGrowth     = invest * pow(1 + invhike / 100.0, m) * pow(1 + expectedReturn / 100.0, m / 12.0);

        double inflationFactor = pow(1 + monthlyInfl, m);
        double realSavings = savingsGrowth / inflationFactor;
        double realInvest  = investGrowth  / inflationFactor;

        double grossMonthly = salaryGrowth / m;
        double taxMonthly = grossMonthly * (taxRate / 100.0);
        double totalTax = taxMonthly * m;
        double monthlyLeftover = grossMonthly - taxMonthly - (expense * pow(1 + exphike / 100.0, m)) - savingsGrowth / m - investGrowth / m;
        double totalLeftover = monthlyLeftover * m;

        cout << GREEN << fixed << setprecision(2)
             << "\n[FORECAST] After " << m << " months:\n"
             << "Gross Salary: ₹"       << salaryGrowth    << "\n"
             << "Net Salary (after tax): ₹" << salaryAfterTax << "\n"
             << "Savings (nominal): ₹"   << savingsGrowth   << "\n"
             << "Savings (real): ₹"      << realSavings     << "\n"
             << "Investments (nominal): ₹"<< investGrowth    << "\n"
             << "Investments (real): ₹"  << realInvest      << "\n"
             << "Total Tax Paid (est.): ₹" << totalTax << "\n"
             << "Monthly Leftover: ₹" << monthlyLeftover << "\n"
             << "Total Leftover After All Deductions: ₹" << totalLeftover << RESET << "\n";
    }

    void detailedMonthlyProjection() const {
        if (!hikesSet) {
            cout << RED << "Set hike percentages first (menu option 5).\n" << RESET;
            return;
        }

        int months;
        cout << CYAN << "\n[DETAILED SIMULATION] Enter months to simulate: " << RESET;
        cin >> months;

        double currentSalary = salary;
        double totalSavings = 0, totalInvest = 0, totalTax = 0, totalLeftover = 0;

        for (int i = 1; i <= months; ++i) {
            currentSalary *= (1 + salhike / 100.0);
            double tax = currentSalary * (taxRate / 100.0);
            double monthlySavings = currentSalary * (savings / salary);
            double monthlyInvest  = currentSalary * (invest / salary);
            double monthlyExpense = expense * pow(1 + exphike / 100.0, i);
            double leftover = currentSalary - tax - monthlySavings - monthlyInvest - monthlyExpense;

            totalTax += tax;
            totalSavings += monthlySavings;
            totalInvest  += monthlyInvest;
            totalLeftover += leftover;
        }

        totalInvest *= pow(1 + expectedReturn / 100.0, months / 12.0);
        double monthlyInflation = inflationRate / 1200.0;
        double inflationFactor = pow(1 + monthlyInflation, months);
        double realSavings = totalSavings / inflationFactor;
        double realInvest  = totalInvest  / inflationFactor;

        cout << GREEN << fixed << setprecision(2)
             << "\n[PROJECTED TOTAL AFTER " << months << " MONTHS]\n"
             << "Nominal Savings: ₹" << totalSavings << "\n"
             << "Nominal Investments: ₹" << totalInvest << "\n"
             << "Real Savings (adjusted): ₹" << realSavings << "\n"
             << "Real Investments (adjusted): ₹" << realInvest << "\n"
             << "Real Net Balance: ₹" << realSavings + realInvest << "\n"
             << "Total Tax Paid: ₹" << totalTax << "\n"
             << "Total Leftover After All Deductions: ₹" << totalLeftover
             << RESET << "\n";
    }
};

int main() {
    emp e;
    int choice;
    cout << YELLOW << "\n=== PERSONAL FINANCE ASSISTANT (v3) ===\n" << RESET;

    while (true) {
        cout << CYAN
             << "\n1. Enter Financial Data\n"
             << "2. Enter Tax & Inflation Rates\n"
             << "3. Show Current Snapshot\n"
             << "4. Run Health Check\n"
             << "5. Estimate Next Month (set hikes)\n"
             << "6. Future Projection (compound growth)\n"
             << "7. Detailed Future Simulation (real savings+investments)\n"
             << "8. Exit\n"
             << "Select option: " << RESET;
        cin >> choice;

        switch (choice) {
            case 1: e.setData(); break;
            case 2: e.setRates(); break;
            case 3: e.showData(); break;
            case 4: e.healthCheck(); break;
            case 5: e.setHikes(); break;
            case 6: e.futureProjection(); break;
            case 7: e.detailedMonthlyProjection(); break;
            case 8: cout << GREEN << "Goodbye!\n" << RESET; return 0;
            default: cout << RED << "Invalid choice.\n" << RESET;
        }
    }
}
