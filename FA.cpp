#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <bits/stdc++.h>
using namespace std;

const string R = "\033[31m";  
const string G = "\033[32m";  
const string Y = "\033[33m";  
const string C = "\033[36m";  
const string X = "\033[0m";   

double getnum(const string &msg) {
    double v;
    while (true) {
        cout << msg;
        if (cin >> v && v >= 0) return v;
        cout << R << "Enter >= 0\n" << X;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

class person {
    string nm;
    int ag{};
    double sal{}, exp{}, sav{}, inv{};
    double sh{}, eh{}, svh{}, ivh{};
    double tax{}, infl{};
    bool hikeset{false};

public:
    void setdata() {
        cout << C << "\nEnter details:\n" << X;
        cout << "Name: "; cin >> nm;
        ag = (int)getnum("Age: ");
        sal = getnum("Salary: ");
        exp = getnum("Expense: ");
        sav = getnum("Savings: ");
        inv = getnum("Invest: ");
    }

    void setrates() {
        cout << C << "\nEnter tax & inflation:\n" << X;
        tax  = getnum("Tax %: ");
        infl = getnum("Inflation %: ");
    }

    void show() {
        cout << Y << fixed << setprecision(2)
             << "\nName: " << nm
             << "\nAge: " << ag
             << "\nSalary: ₹" << sal
             << "\nExpense: ₹" << exp
             << "\nSavings: ₹" << sav
             << "\nInvest: ₹" << inv
             << "\nBalance: ₹" << sal - exp - sav - inv
             << X << "\n";
    }

    void check() {
        cout << C << "\nCheck:\n" << X;
        if (exp > sal) {
            cout << R << "Expenses exceed salary by ₹" << exp - sal << "\n" << X;
        } else if (sal - exp < sav) {
            cout << R << "Savings too high for leftover.\n" << X;
        } else if (sal - exp - sav < inv) {
            cout << R << "Investments too high.\n" << X;
        } else {
            cout << G << "Looks fine.\n" << X;
        }
    }

    void sethike() {
        cout << C << "\nEnter hikes %:\n" << X;
        sh  = getnum("Salary: ");
        eh  = getnum("Expense: ");
        svh = getnum("Savings: ");
        ivh = getnum("Invest: ");

        if (svh > sh * 2 || ivh > sh * 2) {
            cout << R << "Too big hike.\n" << X;
            hikeset = false;
            return;
        }
        hikeset = true;

        cout << Y << fixed << setprecision(2)
             << "\nNext month:\n"
             << "Salary: ₹" << sal * (1 + sh / 100)
             << "\nExpense: ₹" << exp * (1 + eh / 100)
             << "\nSavings: ₹" << sav * (1 + svh / 100)
             << "\nInvest: ₹" << inv * (1 + ivh / 100)
             << X << "\n";
    }

    void proj() {
        if (!hikeset) {
            cout << R << "Set hikes first.\n" << X;
            return;
        }
        int m;
        cout << C << "Months: " << X;
        cin >> m;

        double mi = infl / 1200.0;
        double salf = sal * pow(1 + sh / 100.0, m);
        double salnet = salf * (1 - tax / 100.0);
        double savf = sav * pow(1 + svh / 100.0, m);
        double invf = inv * pow(1 + ivh / 100.0, m);

        double inflf = pow(1 + mi, m);
        double rs = savf / inflf;
        double ri = invf / inflf;

        cout << G << fixed << setprecision(2)
             << "\nAfter " << m << " months:\n"
             << "Gross Sal: ₹" << salf
             << "\nNet Sal: ₹" << salnet
             << "\nSavings: ₹" << savf
             << "\nSavings real: ₹" << rs
             << "\nInvest: ₹" << invf
             << "\nInvest real: ₹" << ri
             << X << "\n";
    }

    void projall() {
        if (!hikeset) {
            cout << R << "Set hikes first.\n" << X;
            return;
        }
        int m;
        cout << C << "Months: " << X;
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

        cout << G << fixed << setprecision(2)
             << "\nTotal after " << m << " months:\n"
             << "Nominal Sav: ₹" << totsav
             << "\nNominal Inv: ₹" << totinv
             << "\nReal Sav: ₹" << rs
             << "\nReal Inv: ₹" << ri
             << "\nReal Total: ₹" << rs + ri
             << X << "\n";
    }
};

int main() {
    person p;
    int ch;
    cout << Y << "\n=== FINANCE TOOL ===\n" << X;
    while (true) {
        cout << C
             << "\n1. Data\n"
             << "2. Tax+Infl\n"
             << "3. Show\n"
             << "4. Check\n"
             << "5. Hikes\n"
             << "6. Proj simple\n"
             << "7. Proj all\n"
             << "8. Exit\n"
             << "Pick: " << X;
        cin >> ch;
        switch (ch) {
            case 1: p.setdata(); break;
            case 2: p.setrates(); break;
            case 3: p.show(); break;
            case 4: p.check(); break;
            case 5: p.sethike(); break;
            case 6: p.proj(); break;
            case 7: p.projall(); break;
            case 8: cout << G << "Bye!\n" << X; return 0;
            default: cout << R << "Invalid\n" << X;
        }
    }
}
