#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Account {
    string name;
    int pan;
    int aadhaar;
    int accNo;
    string password;
    double balance;
};

// Function to get next account number (start from 10000)
int getNextAccountNumber() {
    ifstream file("accounts.txt");
    Account temp;
    int lastAccNo = 9999;
    while (file >> temp.name >> temp.pan >> temp.aadhaar >> temp.accNo >> temp.password >> temp.balance) {
        if (temp.accNo > lastAccNo)
            lastAccNo = temp.accNo;
    }
    file.close();
    return lastAccNo + 1;
}

// Create new account
void createAccount() {
    Account acc;
    cout << "\n--- Create Account ---\n";
    cout << "Enter Full Name: ";
    cin.ignore();
    getline(cin, acc.name);

    cout << "Enter 5-digit PAN Number: ";
    cin >> acc.pan;
    cout << "Enter 5-digit Aadhaar Number: ";
    cin >> acc.aadhaar;

    cout << "Create Password: ";
    cin >> acc.password;

    acc.accNo = getNextAccountNumber();
    acc.balance = 0;

    ofstream file("accounts.txt", ios::app);
    file << acc.name << " " << acc.pan << " " << acc.aadhaar << " "
         << acc.accNo << " " << acc.password << " " << acc.balance << endl;
    file.close();

    cout << "\nAccount Created Successfully!\n";
    cout << "Your Account Number is: " << acc.accNo << "\n";
}

// Find account by account number and password
int findAccount(int accNo, string password, Account &acc) {
    ifstream file("accounts.txt");
    while (file >> acc.name >> acc.pan >> acc.aadhaar >> acc.accNo >> acc.password >> acc.balance) {
        if (acc.accNo == accNo && acc.password == password) {
            file.close();
            return 1;
        }
    }
    file.close();
    return 0;
}

// Update account info in file
void updateAccount(Account acc) {
    ifstream file("accounts.txt");
    ofstream temp("temp.txt");

    Account current;
    while (file >> current.name >> current.pan >> current.aadhaar >> current.accNo >> current.password >> current.balance) {
        if (current.accNo == acc.accNo)
            temp << acc.name << " " << acc.pan << " " << acc.aadhaar << " " << acc.accNo << " "
                 << acc.password << " " << acc.balance << endl;
        else
            temp << current.name << " " << current.pan << " " << current.aadhaar << " " << current.accNo << " "
                 << current.password << " " << current.balance << endl;
    }

    file.close();
    temp.close();

    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}

// Deposit money
void deposit(Account &acc) {
    double amount;
    cout << "Enter amount to deposit: ";
    cin >> amount;
    if (amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    acc.balance += amount;
    updateAccount(acc);

    ofstream stmt("statements.txt", ios::app);
    stmt << acc.accNo << " Deposited +" << amount << " Balance: " << acc.balance << endl;
    stmt.close();

    cout << "Deposit Successful! Current Balance: " << acc.balance << endl;
}

// Withdraw money
void withdraw(Account &acc) {
    double amount;
    cout << "Enter amount to withdraw: ";
    cin >> amount;
    if (amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    if (amount > acc.balance) {
        cout << "Insufficient balance!\n";
        return;
    }
    acc.balance -= amount;
    updateAccount(acc);

    ofstream stmt("statements.txt", ios::app);
    stmt << acc.accNo << " Withdraw -" << amount << " Balance: " << acc.balance << endl;
    stmt.close();

    cout << "Withdrawal Successful! Current Balance: " << acc.balance << endl;
}

// Check balance
void checkBalance(Account &acc) {
    cout << "Current Balance: " << acc.balance << endl;
}

// Show statement
void showStatement(Account &acc) {
    cout << "\n--- Account Statement ---\n";
    ifstream stmt("statements.txt");
    string line;
    int found = 0;
    while (getline(stmt, line)) {
        if (line.find(to_string(acc.accNo)) != string::npos) {
            cout << line << endl;
            found = 1;
        }
    }
    if (!found)
        cout << "No transactions yet.\n";
    stmt.close();
}

// Login
void login() {
    int accNo;
    string password;
    cout << "\n--- Login ---\n";
    cout << "Enter Account Number: ";
    cin >> accNo;
    cout << "Enter Password: ";
    cin >> password;

    Account acc;
    if (findAccount(accNo, password, acc)) {
        cout << "\nWelcome, " << acc.name << "!\n";
        int choice;
        do {
            cout << "\n1. Deposit\n2. Withdraw\n3. Check Balance\n4. Statement\n5. Logout\nEnter choice: ";
            cin >> choice;
            if (choice == 1)
                deposit(acc);
            else if (choice == 2)
                withdraw(acc);
            else if (choice == 3)
                checkBalance(acc);
            else if (choice == 4)
                showStatement(acc);
            else if (choice == 5)
                cout << "Logged out.\n";
            else
                cout << "Invalid choice!\n";
        } while (choice != 5);
    } else {
        cout << "Invalid Account Number or Password!\n";
    }
}

int main() {
    int choice;
    do {
        cout << "\n=== Banking Management System ===\n";
        cout << "1. Create Account\n2. Login\n3. Exit\nEnter choice: ";
        cin >> choice;

        if (choice == 1)
            createAccount();
        else if (choice == 2)
            login();
        else if (choice == 3)
            cout << "Thank you for using the system!\n";
        else
            cout << "Invalid choice!\n";

    } while (choice != 3);

    return 0;
}
