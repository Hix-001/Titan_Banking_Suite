#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h> 
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cctype>
#include <fstream>

using namespace std;

string formatIndianCurrency(double amount) {
    stringstream ss;
    ss << fixed << setprecision(2) << amount;
    string str = ss.str();
    
    int dotPos = str.find('.');
    string intPart = str.substr(0, dotPos);
    string decPart = str.substr(dotPos);
    
    string formatted = "";
    int count = 0;
    
    for (int i = intPart.length() - 1; i >= 0; i--) {
        formatted += intPart[i];
        count++;
        if (i > 0 && count == 3) {
            formatted += ",";
        } else if (i > 0 && count > 3 && (count - 3) % 2 == 0) {
            formatted += ",";
        }
    }
    reverse(formatted.begin(), formatted.end());
    return formatted + decPart;
}

int getValidInt() {
    int val;
    while (!(cin >> val)) {
        cout << " !!! ERROR: Invalid input. Enter numbers only: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return val;
}

double getValidDouble() {
    double val;
    while (!(cin >> val)) {
        cout << " !!! ERROR: Invalid input. Enter numbers only: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return val;
}

string getValidName() {
    string n;
    while (true) {
        getline(cin, n);
        bool valid = true;
        if (n.empty()) continue;
        for (char c : n) {
            if (!isalpha(c) && c != ' ') {
                valid = false;
                break;
            }
        }
        if (valid) return n;
        cout << " !!! ERROR: Name must contain only letters. Try again: ";
    }
}

class BankAccount {
private:
    string accountHolder;
    int accountNumber;
    string pin; 
    double balance;

public:
    BankAccount() { accountNumber = -1; }

    void createAccount(string name, int acc, string p, double bal) {
        accountHolder = name;
        accountNumber = acc;
        pin = p;
        balance = bal;
    }

    int getAccountNumber() { return accountNumber; }
    bool authenticate(string p) { return p == pin; }
    double getBalance() { return balance; }
    string getPin() { return pin; }
    string getAccountHolder() { return accountHolder; }

    void deposit(double amt) {
        balance += amt;
        cout << "\n>>> SUCCESS: Deposited Rs. " << formatIndianCurrency(amt) << endl;
        cout << ">>> NEW BALANCE: Rs. " << formatIndianCurrency(balance) << endl;
    }

    void withdraw(double amt) {
        if (amt > balance) {
            cout << "\n!!! ERROR: INSUFFICIENT FUNDS !!!" << endl;
        } else {
            balance -= amt;
            cout << "\n>>> SUCCESS: Withdrew Rs. " << formatIndianCurrency(amt) << endl;
            cout << ">>> REMAINING BALANCE: Rs. " << formatIndianCurrency(balance) << endl;
        }
    }

    void display() {
        cout << "\n----------------------------------------" << endl;
        cout << " HOLDER  : " << accountHolder << endl;
        cout << " ACC NO  : " << accountNumber << endl;
        cout << " BALANCE : Rs. " << formatIndianCurrency(balance) << endl;
        cout << "----------------------------------------" << endl;
    }
};

void saveDatabase(BankAccount accounts[], int total) {
    ofstream outFile("titan_db.txt");
    outFile << total << endl;
    for (int i = 0; i < total; i++) {
        outFile << accounts[i].getAccountNumber() << " " << accounts[i].getPin() << " " << fixed << setprecision(2) << accounts[i].getBalance() << " " << accounts[i].getAccountHolder() << endl;
    }
    outFile.close();
}

void loadDatabase(BankAccount accounts[], int& total) {
    ifstream inFile("titan_db.txt");
    if (!inFile) return;
    inFile >> total;
    for (int i = 0; i < total; i++) {
        int a;
        string p, n;
        double b;
        inFile >> a >> p >> b;
        getline(inFile >> ws, n);
        accounts[i].createAccount(n, a, p, b);
    }
    inFile.close();
}

string getMaskedPin() {
    string p = "";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') { 
            if (p.length() > 0) {
                p.pop_back();
                cout << "\b \b";
            }
        } else {
            p += ch;
            cout << '*';
        }
    }
    cout << endl;
    return p;
}

void showBigLogo() {
    system("cls");
    cout << "\n  _______  _____  _______  _______  _     _ " << endl;
    cout << "     |       |       |     |_____|  | \\   | " << endl;
    cout << "     |       |       |     |     |  |  \\  | " << endl;
    cout << "     |     __|__     |     |     |  |   \\_| " << endl;
    cout << "\n             TITAN BANKING SUITE            " << endl;
    cout << "--------------------------------------------" << endl;
}	

int main() {
    BankAccount accounts[100];
    int totalAccounts = 0;
    
    loadDatabase(accounts, totalAccounts);

    int choice;
    while (true) {
        showBigLogo();
        cout << " 1. REGISTER NEW ACCOUNT" << endl;
        cout << " 2. SECURE LOGIN" << endl;
        cout << " 3. EXIT SYSTEM" << endl;
        cout << "--------------------------------------------" << endl;
        cout << " CHOICE: ";
        choice = getValidInt();

        if (choice == 1) {
            if (totalAccounts < 100) {
                string n; int a; double b; string p;
                cout << "\n[REGISTRATION MODE]" << endl;
                cout << " Full Name      : "; 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                n = getValidName();
                
                bool duplicate;
                do {
                    duplicate = false;
                    cout << " Account Number : "; 
                    a = getValidInt();
                    for(int i = 0; i < totalAccounts; i++) {
                        if(accounts[i].getAccountNumber() == a) {
                            cout << " !!! ERROR: Account Number already exists. Try a different one.\n";
                            duplicate = true;
                            break;
                        }
                    }
                } while(duplicate);

                cout << " Set 4-Digit PIN: "; 
                while (true) {
                    p = getMaskedPin();
                    bool valid = (p.length() == 4);
                    for (char c : p) {
                        if (!isdigit(c)) valid = false;
                    }
                    if (valid) break;
                    cout << " !!! ERROR: PIN must be exactly 4 numbers. Try again: ";
                }

                cout << " Initial Deposit (Rs.): "; 
                b = getValidDouble();
                
                accounts[totalAccounts].createAccount(n, a, p, b);
                totalAccounts++;
                saveDatabase(accounts, totalAccounts);
                
                cout << "\n>>> ACCOUNT CREATED SUCCESSFULLY!" << endl;
                cout << ">>> Thank you for choosing Titan Banking Suite!" << endl;
                cout << "\nPress any key to return to main menu...";
                _getch();

            } else {
                cout << "\n!!! DATABASE FULL !!!\nPress any key...";
                _getch();
            }
        } else if (choice == 2) {
            int a;
            cout << "\n[SECURE LOGIN]" << endl;
            cout << " Account No : "; 
            a = getValidInt();
            cout << " Enter PIN  : "; 
            string p = getMaskedPin();

            bool found = false;
            for (int i = 0; i < totalAccounts; i++) {
                if (accounts[i].getAccountNumber() == a && accounts[i].authenticate(p)) {
                    found = true;
                    int sub;
                    do {
                        cout << "\n--- USER DASHBOARD ---" << endl;
                        cout << " 1. DEPOSIT FUNDS" << endl;
                        cout << " 2. WITHDRAW FUNDS" << endl;
                        cout << " 3. VIEW BALANCE" << endl;
                        cout << " 4. DELETE ACCOUNT" << endl;
                        cout << " 5. SECURE LOGOUT" << endl;
                        cout << "----------------------" << endl;
                        cout << " ACTION: "; 
                        sub = getValidInt();
                        
                        if (sub == 1) {
                            cout << " Enter PIN to confirm Deposit: ";
                            string pConfirm = getMaskedPin();
                            if (accounts[i].authenticate(pConfirm)) {
                                double amt; 
                                cout << " Enter Amount to Deposit (Rs.): "; 
                                amt = getValidDouble();
                                accounts[i].deposit(amt);
                                saveDatabase(accounts, totalAccounts);
                            } else {
                                cout << "\n !!! ERROR: Incorrect PIN. Transaction Cancelled !!!\n";
                            }
                        } else if (sub == 2) {
                            cout << " Enter PIN to confirm Withdrawal: ";
                            string pConfirm = getMaskedPin();
                            if (accounts[i].authenticate(pConfirm)) {
                                double amt; 
                                cout << " Enter Amount to Withdraw (Rs.): "; 
                                amt = getValidDouble();
                                accounts[i].withdraw(amt);
                                saveDatabase(accounts, totalAccounts);
                            } else {
                                cout << "\n !!! ERROR: Incorrect PIN. Transaction Cancelled !!!\n";
                            }
                        } else if (sub == 3) {
                            accounts[i].display();
                        } else if (sub == 4) {
                            cout << " Enter PIN to confirm Deletion: ";
                            string pConfirm = getMaskedPin();
                            if (accounts[i].authenticate(pConfirm)) {
                                for (int j = i; j < totalAccounts - 1; j++) {
                                    accounts[j] = accounts[j + 1];
                                }
                                totalAccounts--;
                                saveDatabase(accounts, totalAccounts);
                                cout << "\n>>> ACCOUNT DELETED SUCCESSFULLY! <<<\nPress any key...";
                                _getch();
                                break;
                            } else {
                                cout << "\n !!! ERROR: Incorrect PIN. Deletion Cancelled !!!\n";
                            }
                        }
                    } while (sub != 5);
                }
            }
            if (!found) {
                cout << "\n !!! AUTHENTICATION FAILED !!!\nPress any key to return...";
                _getch();
            }
        } else if (choice == 3) {
            cout << "\nSHUTTING DOWN TITAN SYSTEMS..." << endl;
            break;
        } else if (choice == 9) {
            system("cls");
            cout << "\n[RESTRICTED AREA: ROOT ACCESS]\n";
            string aId;
            cout << " Enter Admin ID: ";
            cin >> aId;
            cout << " Enter Root PIN: ";
            string aPass = getMaskedPin();

            if (aId == "AIX" && aPass == "0219") {
                int adminChoice;
                do {
                    system("cls");
                    cout << "\n============================================" << endl;
                    cout << "           TITAN ROOT CONSOLE               " << endl;
                    cout << "============================================" << endl;
                    cout << " 1. View System Liquidity & Stats" << endl;
                    cout << " 2. View Complete Account Ledger" << endl;
                    cout << " 3. Terminate Root Session" << endl;
                    cout << "--------------------------------------------" << endl;
                    cout << " COMMAND: ";
                    adminChoice = getValidInt();

                    if (adminChoice == 1) {
                        double totalLiquidity = 0;
                        for(int i = 0; i < totalAccounts; i++) {
                            totalLiquidity += accounts[i].getBalance();
                        }
                        cout << "\n--- SYSTEM DIAGNOSTICS ---" << endl;
                        cout << " Active Accounts : " << totalAccounts << " / 1000" << endl;
                        cout << " Total Liquidity : Rs. " << formatIndianCurrency(totalLiquidity) << endl;
                        cout << "\nPress any key to return...";
                        _getch();
                    } else if (adminChoice == 2) {
                        cout << "\n--- MASTER ACCOUNT LEDGER ---" << endl;
                        if(totalAccounts == 0) {
                            cout << " No accounts registered in the database." << endl;
                        } else {
                            for(int i = 0; i < totalAccounts; i++) {
                                accounts[i].display();
                            }
                        }
                        cout << "\nPress any key to return...";
                        _getch();
                    }
                } while (adminChoice != 3);
            } else {
                cout << "\n !!! SECURITY BREACH DETECTED: ACCESS DENIED !!!\nPress any key...";
                _getch();
            }
        }
    }
    return 0;
}