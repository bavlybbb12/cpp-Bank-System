#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Bank system.h"

bankaccount* find_account(std::vector <bankaccount*>& accounts, int id) {
    
    for (bankaccount* acc : accounts) {
        if (acc->id_getter() == id) return acc;
    }
    return nullptr;
}

bool is_id_taken(const std::vector<bankaccount*>& accounts, int id) {
    
    for (bankaccount* acc : accounts) {
        if (acc->id_getter() == id) return true;
    }
    return false;
}

void save_data(const std::vector<bankaccount*>& accounts, std::string filename) {
    
    std::ofstream file(filename);
    if (!file) { std::cout << "Error: Could not open file.\n"; return; }

    for (bankaccount* acc : accounts) {
        acc->save(file);
    }
    file.close();
    std::cout << "Database saved to " << filename << "\n";
}

void load_data(std::vector<bankaccount*>& accounts, std::string filename) {
    
    std::ifstream file(filename);
    if (!file) { std::cout << "File not found. Starting fresh.\n"; return; }
    if (file.peek() == std::ifstream::traits_type::eof()) return;

    int type, id;
    std::string name;
    double balance;

    while (file >> type) {
        file >> id >> name >> balance;
        if (type == 1) {
            double rate; file >> rate;
            accounts.push_back(new saving_account(id, name, balance, rate));
        }
        else if (type == 2) {
            double fee; file >> fee;
            accounts.push_back(new checking_account(id, name, balance, fee));
        }
    }
    file.close();
    std::cout << "Database loaded.\n";
}

void transfer_money(std::vector<bankaccount*>& accounts, bankaccount* sender) {
    
    std::cout << "\n--- WIRE TRANSFER ---\n";
    std::cout << "Current Balance: " << sender->get_balance() << "\n";

    std::cout << "Enter Recipient ID: ";
    int dest_id;
    std::cin >> dest_id;

    if (dest_id == sender->id_getter()) {
        std::cout << "Error: You cannot transfer money to yourself.\n";
        return;
    }

    bankaccount* receiver = find_account(accounts, dest_id);
    if (receiver == nullptr) {
        std::cout << "Error: Recipient account not found.\n";
        return;
    }

    double amount;
    std::cout << "Enter Amount to Transfer: ";
    std::cin >> amount;

    std::cout << "Processing transaction...\n";

    if (sender->withdraw(amount)) {
        receiver->deposit(amount);
        std::cout << ">>> TRANSFER COMPLETED SUCCESSFULY . Sent " << amount << " to ID " << dest_id << ".\n";
    }
    else {
        std::cout << ">>> TRANSFER FAILED. Transaction cancelled.\n";
    }
}

void delete_account(std::vector <bankaccount*>& accounts) {
    
    if (accounts.empty()) { std::cout << "No accounts to delete.\n"; return; }

    int id;
    std::cout << "Enter ID to delete: "; std::cin >> id;

    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i]->id_getter() == id) {
            std::cout << "Deleting account " << accounts[i]->id_getter() << "... Confirm? (1=Yes, 0=No): ";
            int confirm; std::cin >> confirm;
            if (confirm == 1) {
                delete accounts[i];
                accounts.erase(accounts.begin() + i);
                std::cout << "Account deleted.\n";
            }
            else if ( confirm != 0) {
                std::cout << "Invalid confirmation. Operation cancelled.\n";
            }
            else {
                std::cout << "Operation cancelled.\n";
			}
            return;
        }
    }
    std::cout << "Account not found.\n";
}

void update_interest(std::vector <bankaccount*>& accounts) {
    
    int id; std::cout << "Enter ID: "; std::cin >> id;
    bankaccount* acc = find_account(accounts, id);
    if (acc) {
        double r; std::cout << "Enter new Rate: "; std::cin >> r;
        acc->interest_setter(r);
    }
    else std::cout << "Account not found.\n";
}

void update_fee(std::vector <bankaccount*>& accounts) {
    
    int id; std::cout << "Enter ID: "; std::cin >> id;
    bankaccount* acc = find_account(accounts, id);
    if (acc) {
        double f; std::cout << "Enter new Fee: "; std::cin >> f;
        acc->fee_setter(f);
    }
    else std::cout << "Account not found.\n";
}

void add_account(std::vector<bankaccount*>& accounts) {
    
    std::cout << "\n--- New Account ---\n1. Savings account \n2. Checkingaccount \n Select: ";
    int type; std::cin >> type;
    if (type != 1 && type != 2) return;

    int id;
    std::cout << "Enter ID: "; std::cin >> id;

    if (is_id_taken(accounts, id)) {
        std::cout << "CRITICAL ERROR: Account ID " << id << " is already in use!\n";
        return;
    }

    std::string name;
    double bal;
    std::cout << "Holder Name: "; std::cin >> name;
    std::cout << "Initial Balance: "; std::cin >> bal;

    if (type == 1) {
        double r; std::cout << "Interest Rate: "; std::cin >> r;
        accounts.push_back(new saving_account(id, name, bal, r));
    }
    else {
        double f; std::cout << "Trans. Fee: "; std::cin >> f;
        accounts.push_back(new checking_account(id, name, bal, f));
    }
    std::cout << "Account created successfully.\n";
}

void admin_menu(std::vector<bankaccount*>& accounts) {
    std::string pwd;
    std::cout << "Admin Password: "; std::cin >> pwd;
    if (pwd != "admin123") return;

    while (true) {
        std::cout << "\n[ADMIN PANEL]\n1. Add Account\n2. Delete Account\n3. Update Interest\n4. Update Fee\n5. Exit\n> ";
        int c; std::cin >> c;
        if (c == 1) add_account(accounts);
        else if (c == 2) delete_account(accounts);
        else if (c == 3) update_interest(accounts);
        else if (c == 4) update_fee(accounts);
        else break;
    }
}

void user_menu(std::vector<bankaccount*>& accounts) {
    
    std::cout << "Login ID: ";
    int id; std::cin >> id;
    bankaccount* acc = find_account(accounts, id);
    if (!acc) { std::cout << "User not found.\n"; return; }

    while (true) {
        std::cout << "\n[USER DASHBOARD: " << id << "]\n";
        std::cout << "1. Deposit\n2. Withdraw\n3. Show Info\n4. Transfer Money\n5. Logout\n> ";
        int c; std::cin >> c;

        if (c == 1) { double a; std::cout << "Amount: "; std::cin >> a; acc->deposit(a); }
        else if (c == 2) { double a; std::cout << "Amount: "; std::cin >> a; acc->withdraw(a); }
        else if (c == 3) acc->show_info();
        else if (c == 4) transfer_money(accounts, acc);
        else break;
    }
}