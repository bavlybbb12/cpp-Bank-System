#include <iostream>
#include <vector>
#include <string>
#include "Bank system.h" 
#include "Menus.h"

int main() {
    std::vector<bankaccount*> accounts;

    std::cout << "--- BANK BOOT SEQUENCE ---\n";
    std::cout << "1. Load Dummy Data\n2. Load from File\n3. New Database\n> ";
    int start_choice;
    std::cin >> start_choice;

    if (start_choice == 1) {
        accounts.push_back(new saving_account(101, "Alice", 500.0, 0.1));
        accounts.push_back(new checking_account(201, "Bob", 1000.0, 5));
        accounts.push_back(new saving_account(102, "larry", 2000.0, 0.1));
        accounts.push_back(new saving_account(103, "Mark", 500.0, 0.1));
        accounts.push_back(new saving_account(104, "David", 10000.0, 0.1));
        accounts.push_back(new checking_account(202, "Jacob", 0, 5));
        accounts.push_back(new checking_account(203, "Magdy", 5000.0, 5));
        accounts.push_back(new checking_account(204, "Harry", 2555.5, 5));

        std::cout << "Dummy data loaded successfuly.\n";
    }
    else if (start_choice == 2) {
        std::string fname;
        std::cout << "Filename: "; std::cin >> fname;
        load_data(accounts, fname);
    }

    bool running = true;
    while (running) {
        std::cout << "\n=== MAIN SYSTEM ===\n";
        std::cout << "1. Admin Portal\n2. User Portal\n3. Save & Shutdown\n4. Shutdown without saving\n> ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            admin_menu(accounts);
        }
        else if (choice == 2) {
            user_menu(accounts);
        }
        else if (choice == 3) {
            std::string fname;
            std::cout << "Save Filename: "; std::cin >> fname;
            save_data(accounts, fname);
            running = false;
        }
        else if (choice == 4) {
            running = false;
		}
        else {
            std::cout << "Invalid selection.\n";
        }
    }

    for (auto acc : accounts) { delete acc; }
    accounts.clear();
    std::cout << "System Halt.\n";

    return 0;
}