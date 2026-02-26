#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class bankaccount {
protected:
    int account_number;
    std::string holder_name;
    double balance;

public:
    bankaccount(int number, std::string name, double Balance) {
        account_number = number;
        holder_name = name;
        balance = Balance;
    }

    virtual void save(std::ofstream& file) const = 0;

    void deposit(double amount) {
        if (amount <= 0) {
            std::cout << "Error: Invalid amount.\n";
            return;
        }
        balance += amount;
        std::cout << "Success! Deposited: " << amount << " | New Balance: " << balance << "\n";
    }

    virtual void show_info() const {
        std::cout << "ID: " << account_number << "\nName: " << holder_name << "\nBalance: " << balance << "\n";
    }

    long long id_getter() const { return account_number; }

    double get_balance() const { return balance; }

    virtual bool withdraw(double amount) = 0;

    virtual void interest_setter(double interest) {
        std::cout << "Error: Not applicable for this account type.\n";
    }

    virtual void fee_setter(double fee) {
        std::cout << "Error: Not applicable for this account type.\n";
    }

    virtual ~bankaccount() {}
};

class saving_account : public bankaccount {
private:
    double interest_rate;

public:
    saving_account(int Number, std::string Name, double Balance, double interest)
        : bankaccount(Number, Name, Balance), interest_rate(interest) {
    }

    void save(std::ofstream& file) const override {
        file << "1 " << account_number << " " << holder_name << " " << balance << " " << interest_rate << "\n";
    }

    void interest_setter(double interest) override {
        interest_rate = interest;
        std::cout << "Interest rate updated to " << interest_rate << "\n";
    }

    bool withdraw(double amount) override {
        if (amount <= 0) {
            std::cout << "Error: Invalid amount.\n";
            return false;
        }
        if (amount > balance) {
            std::cout << "Error: Insufficient funds.\n";
            return false;
        }

        balance -= amount;
        std::cout << "Success! Withdrawn: " << amount << " | Remaining: " << balance << "\n";
        return true;
    }

    void show_info() const override {
        bankaccount::show_info();
        std::cout << "Interest Rate: " << interest_rate << "\n";
    }
};

class checking_account : public bankaccount {
private:
    double transaction_fee;

public:
    checking_account(int Number, std::string Name, double Balance, double Fee)
        : bankaccount(Number, Name, Balance), transaction_fee(Fee) {
    }

    void save(std::ofstream& file) const override {
        file << "2 " << account_number << " " << holder_name << " " << balance << " " << transaction_fee << "\n";
    }

    void fee_setter(double fee) override {
        transaction_fee = fee;
        std::cout << "Transaction Fee updated to " << transaction_fee << "\n";
    }

    bool withdraw(double amount) override {
        if (amount <= 0) {
            std::cout << "Error: Invalid amount.\n";
            return false;
        }
        if ((amount + transaction_fee) > balance) {
            std::cout << "Error: Insufficient funds (including fee).\n";
            return false;
        }

        balance -= (amount + transaction_fee);
        std::cout << "Success! Withdrawn: " << amount << " (Fee: " << transaction_fee << ") | Remaining: " << balance << "\n";
        return true;
    }

    void show_info() const override {
        bankaccount::show_info();
        std::cout << "Transaction Fee: " << transaction_fee << "\n";
    }
};