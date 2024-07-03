#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

class Expense {
public:
    Expense(const std::string& description, double amount)
        : description(description), amount(amount) {}

    std::string getDescription() const { return description; }
    double getAmount() const { return amount; }

private:
    std::string description;
    double amount;
};

class User {
public:
    User(const std::string& username, const std::string& password)
        : username(username), password(password) {}

    std::string getUsername() const { return username; }
    bool checkPassword(const std::string& pwd) const { return password == pwd; }
    void addExpense(const Expense& expense) { expenses.push_back(expense); }
    void viewExpenses() const;
    void deleteExpense(const std::string& description);
    double getTotalExpenses() const;

private:
    std::string username;
    std::string password;
    std::vector<Expense> expenses;
};

void User::viewExpenses() const {
    std::cout << "Expenses for " << username << ":" << std::endl;
    for (const auto& expense : expenses) {
        std::cout << " - " << expense.getDescription() << ": $" << std::fixed << std::setprecision(2) << expense.getAmount() << std::endl;
    }
    std::cout << "Total Expenses: $" << std::fixed << std::setprecision(2) << getTotalExpenses() << std::endl;
}

void User::deleteExpense(const std::string& description) {
    expenses.erase(std::remove_if(expenses.begin(), expenses.end(),
        [&description](const Expense& expense) {
            return expense.getDescription() == description;
        }), expenses.end());
    std::cout << "Expense deleted successfully." << std::endl;
}

double User::getTotalExpenses() const {
    double total = 0;
    for (const auto& expense : expenses) {
        total += expense.getAmount();
    }
    return total;
}

class FinanceTracker {
public:
    bool registerUser(const std::string& username, const std::string& password);
    bool loginUser(const std::string& username, const std::string& password);
    void addExpense(const std::string& description, double amount);
    void viewExpenses() const;
    void deleteExpense(const std::string& description);

private:
    std::vector<User> users;
    User* currentUser = nullptr;
};

bool FinanceTracker::registerUser(const std::string& username, const std::string& password) {
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            std::cerr << "Username already taken. Please choose another username." << std::endl;
            return false;
        }
    }
    users.emplace_back(username, password);
    std::cout << "User registered successfully." << std::endl;
    return true;
}

bool FinanceTracker::loginUser(const std::string& username, const std::string& password) {
    for (auto& user : users) {
        if (user.getUsername() == username && user.checkPassword(password)) {
            currentUser = &user;
            std::cout << "Logged in successfully." << std::endl;
            return true;
        }
    }
    std::cerr << "Invalid username or password." << std::endl;
    return false;
}

void FinanceTracker::addExpense(const std::string& description, double amount) {
    if (currentUser) {
        currentUser->addExpense(Expense(description, amount));
        std::cout << "Expense added successfully." << std::endl;
    }
    else {
        std::cerr << "No user logged in." << std::endl;
    }
}

void FinanceTracker::viewExpenses() const {
    if (currentUser) {
        currentUser->viewExpenses();
    }
    else {
        std::cerr << "No user logged in." << std::endl;
    }
}

void FinanceTracker::deleteExpense(const std::string& description) {
    if (currentUser) {
        currentUser->deleteExpense(description);
    }
    else {
        std::cerr << "No user logged in." << std::endl;
    }
}

void printMenu() {
    std::cout << "1. Register" << std::endl;
    std::cout << "2. Login" << std::endl;
    std::cout << "3. Add Expense" << std::endl;
    std::cout << "4. View Expenses" << std::endl;
    std::cout << "5. Delete Expense" << std::endl;
    std::cout << "6. Quit" << std::endl;
    std::cout << "Choose an option: ";
}

int main() {
    FinanceTracker tracker;
    bool running = true;

    while (running) {
        printMenu();
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string username, password;
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            tracker.registerUser(username, password);
            break;
        }
        case 2: {
            std::string username, password;
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            tracker.loginUser(username, password);
            break;
        }
        case 3: {
            std::string description;
            double amount;
            std::cout << "Enter description: ";
            std::cin.ignore(); // to ignore leftover newline character
            std::getline(std::cin, description);
            std::cout << "Enter amount: ";
            std::cin >> amount;
            tracker.addExpense(description, amount);
            break;
        }
        case 4:
            tracker.viewExpenses();
            break;
        case 5: {
            std::string description;
            std::cout << "Enter description of expense to delete: ";
            std::cin.ignore(); // to ignore leftover newline character
            std::getline(std::cin, description);
            tracker.deleteExpense(description);
            break;
        }
        case 6:
            running = false;
            break;
        default:
            std::cerr << "Invalid option." << std::endl;
        }
    }

    return 0;
}
