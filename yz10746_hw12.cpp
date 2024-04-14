// yz10746_hw12.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

const int maxnum = 1000;

class money {
private:
    double checkamount;

public:
    money():checkamount(0.0){}
    money(double checkamount) : checkamount(checkamount){}
    void setcheckamount(double a) {
        checkamount = a;
    }
    double getcheckamount()const {
        return checkamount;
    }
    friend istream& operator >>(istream& is, money& deposit) {
        is >> deposit.checkamount; 
        return is;
    };
};

class check {

private:
    int number;
    money amount;
    bool cashed;

public:
    check() :number(0), cashed(false) {}
    void setnumber(int n) {
        number = n;
    }
    void setamount(double a) {
        amount.setcheckamount(a);
    }
    void setcashed(bool c) {
        cashed = c;
    }
    int getnumber()const {
        return number;
    }
    double getamount()const {
        return amount.getcheckamount();
    }
    bool iscashed()const {
        return cashed;
    }
    friend istream& operator >>(istream& is, check& check) {
        double amt;
        int ifcashed;
        is >> check.number >> amt >> ifcashed;
        check.amount = amt;
        check.cashed = (ifcashed != 0);
        return is;
    };
    friend ostream& operator <<(ostream& os, const check& check) {
        os << "Check number: " << check.number << " with amount: $" << check.getamount() << endl;
        return os;
    }
};

void sort(check checks[], int count) {
    for (int i = 0;i < count - 1;i++) {
        for (int j = 0;j < count - i - 1;j++) {
            if (checks[j].getnumber() > checks[j + 1].getnumber()) {
                check temp = checks[j];
                checks[j] = checks[j + 1];
                checks[j + 1] = temp;
            }
        }
    }
}

int main()
{
    check checks[maxnum];
    money deposits[maxnum];

    int checkcount = 0, depositcount = 0;
    check checkinput;
    check cashedsum, uncashedsum;
    cout << "Please enter the check number, amount on check[exclude the dollar sign] and whether or not the check has been" << endl;
    cout << "    cashed(by typing 1(Yes) or 0(No)):" << endl;
    cout << endl;
    cout << "\t" << "  Example input: 9 for check number, 40.89 for amount and 1 for being cashed(9 40.89 1)" << endl;
    cout << "\t" << "  End output by entering 0 for check number, 0.00 for amount and 0 for being cashed (0 0.00 0)" << endl;
    while (cin >> checkinput && checkinput.getnumber() != 0) {
        checks[checkcount++] = checkinput;
        if (checkinput.iscashed()) {
            cashedsum.setamount(cashedsum.getamount() + checkinput.getamount());
        }
        else {
            uncashedsum.setamount(uncashedsum.getamount() + checkinput.getamount());
        }
    }
    cout << "The sum of the cashed checks is:" << endl;
    cout << "$" << cashedsum.getamount() << endl;
    cout << "The sum of the un-cashed checks is:" << endl;
    cout << "$" << uncashedsum.getamount() << endl;
    money depositinput, depositsum;
    cout << "Please enter the deposits (end deposit input by typing 0):" << endl;
    while (cin >> depositinput && depositinput.getcheckamount() != 0.0) {
        deposits[depositcount++] = depositinput;
        depositsum.setcheckamount(depositsum.getcheckamount() + depositinput.getcheckamount());
    }

    cout << "The sum of the deposits is:" << endl;
    cout << "$";
    cout << depositsum.getcheckamount() << endl;
    money oldbalance, newbalance;
    cout << "Please enter the prior balance amount:" << endl;
    cin >> oldbalance;
    cout << "Please enter the new balance amount according to account holder: " << endl;
    cin >> newbalance;
    cout << endl;

    money calculatebalance (oldbalance.getcheckamount() + depositsum.getcheckamount() - cashedsum.getamount());
    cout << "The balance according to the bank which includes only cleared checks is: $" << calculatebalance.getcheckamount() << endl;
    cout << "The difference between the account holders balance of $" << newbalance.getcheckamount() << " and bank balance of $" << calculatebalance.getcheckamount() << " is: $";
    if (newbalance.getcheckamount() > calculatebalance.getcheckamount()) {
        cout << newbalance.getcheckamount() - calculatebalance.getcheckamount() << endl;
    }
    else {
        cout << calculatebalance.getcheckamount() - newbalance.getcheckamount() << endl;
    }
    cout << endl;
    sort(checks, checkcount);
    cout << "The cashed checks are: " << endl;
    for (int i = 0; i < checkcount; i++) {
        if (checks[i].iscashed()) {
            cout << checks[i];
        }
    }
    cout << endl;
    cout << "The uncashed checks are: " << endl;
    for (int i = 0; i < checkcount; i++) {
        if (!checks[i].iscashed()) {
            cout << checks[i];
        }
    }
}


