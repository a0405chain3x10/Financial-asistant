// financial_assistant_ui.cpp
// Author: Aditya Soni
// Contributor (UI Enhancement): Ghadge Pranjal Ramchandra
// Description: Combined version with improved terminal UI and original logic

#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <thread>
#include <chrono>
using namespace std;

// ====== COLOR CODES ======
const string R = "\033[31m";  // Red
const string G = "\033[32m";  // Green
const string Y = "\033[33m";  // Yellow
const string C = "\033[36m";  // Cyan
const string X = "\033[0m";   // Reset

// ====== UI FUNCTIONS ======
void printHeader(const string &title) {
    cout << C << "\n=============================================\n" << X;
    cout << "           " << title << "\n";
    cout << C << "=============================================\n" << X;
}

void printFooter() {
    cout << C << "=============================================\n" << X;
}

void showLoading(const string &message) {
    cout << C << message;
    for (int i = 0; i < 3; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(350));
    }
    cout << X << endl;
}

double getnum(const string &msg) {
    double v;
    while (true) {
        cout << msg;
        if (cin >> v && v >= 0) return v;
        cout << R << "Enter a value >= 0\n" << X;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ====== CLASS DEFINITION ======
class person {
    string nm;
    int ag{};
    double sal{}, exp{}, sav{}, inv{};
    double sh{}, eh{}, svh{}, ivh{};
    double tax{}, infl{};
    bool hikeset{false};

public:
    void setdata() {
        printHeader("ENTER PERSONAL DETAILS");
        cout << "Name: "; cin >> nm;
        ag = (int)getnum("Age: ");
        sal = getnum("Salary: ");
        exp = getnum("Expense: ");
        sav = getnum("Savings: ");
        inv = getnum("Investment: ");
        printFooter();
    }

    void setrates() {
        printHeader("TAX AND INFLATION DETAILS");
        tax  = getnum("Tax (%): ");
        infl = getnum("Inflation (%): ");
        printFooter();
    }

    void show() {
        printHeader("CURRENT FINANCIAL SUMMARY");
        cout << fixed << setprecision(2)
             << "Name: " << nm
             << "\nAge: " << ag
             << "\nSalary: ₹" << sal
             << "\nExpense: ₹" << exp
             << "\nSavings: ₹" << sav
             << "\nInvestment: ₹" << inv
             << "\nBalance: ₹" << sal - exp - sav - inv << endl;
        printFooter();
    }

    void check() {
        printHeader("FINANCIAL HEALTH CHECK");
        if (exp > sal) {
            cout << R << "⚠ Expenses exceed salary by ₹" << exp - sal << "\n" << X;
        } else if (sal - exp < sav) {
            cout << R << "⚠ Savings too high for leftover balance.\n" << X;
        } else if (sal - exp - sav < inv) {
            cout << R << "⚠ Investments too high for available balance.\n" << X;
        } else {
            cout << G << "✓ Your financial condition looks healthy.\n" << X;
        }
        printFooter();
    }

    void sethike() {
        printHeader("SET MONTHLY HIKES (%)");
        sh  = getnum("Salary: ");
        eh  = getnum("Expense: ");
        svh = getnum("Savings: ");
        ivh = getnum("Investment: ");

        if (svh > sh * 2 || ivh > sh * 2) {
            cout << R << "Too high hike. Please recheck values.\n" << X;
            hikeset = false;
            return;
        }
        hikeset = true;

        cout << Y << fixed << setprecision(2)
             << "\nProjected Next Month:\n"
             << "Salary: ₹" << sal * (1 + sh / 100)
             << "\nExpense: ₹" << exp * (1 + eh / 100)
             << "\nSavings: ₹" << sav * (1 + svh / 100)
             << "\nInvestment: ₹" << inv * (1 + ivh / 100) << X << endl;
        printFooter();
    }

    void proj() {
        if (!hikeset) {
            cout << R << "Set hikes before projection.\n" << X;
            return;
        }
        int m;
        printHeader("SIMPLE PROJECTION");
        cout << "Enter number of months: ";
        cin >> m;

        double mi = infl / 1200.0;
        double salf = sal * pow(1 + sh / 100.0, m);
        double salnet = salf * (1 - tax / 100.0);
        double savf = sav * pow(1 + svh / 100.0, m);
        double invf = inv * pow(1 + ivh / 100.0, m);
        double inflf = pow(1 + mi, m);
        double rs = savf / inflf;
        double ri = invf / inflf;

        showLoading("Calculating projection");
        cout << G << fixed << setprecision(2)
             << "\nAfter " << m << " months:\n"
             << "Gross Salary: ₹" << salf
             << "\nNet Salary: ₹" << salnet
             << "\nSavings (Nominal): ₹" << savf
             << "\nSavings (Real): ₹" << rs
             << "\nInvestments (Nominal): ₹" << invf
             << "\nInvestments (Real): ₹" << ri << X << endl;
        printFooter();
    }

    void projall() {
        if (!hikeset) {
            cout << R << "Set hikes before projection.\n" << X;
            return;
        }
        int m;
        printHeader("DETAILED PROJECTION");
        cout << "Enter number of months: ";
        cin >> m;

        double cursal = sal;
        double totsav = 0, totinv = 0;

        for (int i = 1; i <= m; ++i) {
            cursal *= (1 + sh / 100.0);
            double msav = cursal * (sav / sal);
            double minv = cursal * (inv / sal);
            totsav += msav;
            totinv += minv;
        }

        double mi = infl / 1200.0;
        double inflf = pow(1 + mi, m);
        double rs = totsav / inflf;
        double ri = totinv / inflf;

        showLoading("Generating full projection");
        cout << G << fixed << setprecision(2)
             << "\nTotal after " << m << " months:\n"
             << "Nominal Savings: ₹" << totsav
             << "\nNominal Investments: ₹" << totinv
             << "\nReal Savings: ₹" << rs
             << "\nReal Investments: ₹" << ri
             << "\nTotal (Real Value): ₹" << rs + ri << X << endl;
        printFooter();
    }
};

// ====== MAIN PROGRAM ======
int main() {
    person p;
    int ch;

    while (true) {
        printHeader("PERSONAL FINANCIAL ASSISTANT");
        cout << "1. Enter Data\n";
        cout << "2. Set Tax & Inflation\n";
        cout << "3. Show Details\n";
        cout << "4. Financial Check\n";
        cout << "5. Set Hikes\n";
        cout << "6. Simple Projection\n";
        cout << "7. Full Projection\n";
        cout << "8. Exit\n";
        printFooter();

        cout << "Enter your choice: ";
        cin >> ch;

        switch (ch) {
            case 1: p.setdata(); break;
            case 2: p.setrates(); break;
            case 3: p.show(); break;
            case 4: p.check(); break;
            case 5: p.sethike(); break;
            case 6: p.proj(); break;
            case 7: p.projall(); break;
            case 8:
                showLoading("Saving and exiting");
                cout << G << "Goodbye! Stay financially smart!" << X << endl;
                return 0;
            default:
                cout << R << "Invalid choice. Try again.\n" << X;
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
        system("cls"); // use "clear" for Linux/Mac
    }
}
