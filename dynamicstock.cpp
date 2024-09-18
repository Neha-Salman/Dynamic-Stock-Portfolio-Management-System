#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits> 

using namespace std;

const string FILENAME = "portfolio_data.txt";

class Stock {
    private:
        string symbol;         
        double price;         
        double initialPrice;   
        double previousPrice;  
        int quantity;          

    public:
        Stock(const string& symbol, double price, int quantity) : symbol(symbol), price(price), initialPrice(price), previousPrice(price), quantity(quantity) {}

        const string& getSymbol() const { 
            return symbol; 
        }
        double getPrice() const { 
            return price; 
        }
        void setPrice(double newPrice) { 
            price = newPrice; 
        }
        double getInitialPrice() const { 
            return initialPrice;
        }
        double getPreviousPrice() const { 
            return previousPrice; 
        }
        void setPreviousPrice(double newPrice) { 
            previousPrice = newPrice; 
        }
        int getQuantity() const { 
            return quantity; 
        }
        void setQuantity(int newQuantity) { 
            quantity = newQuantity; 
        }
};

class Portfolio {
    private:
        vector<Stock> stocks;
        double overallProfitLoss;

        void loadFromFile();                    

    public:
        Portfolio() : overallProfitLoss(0) {
            loadFromFile();
        }
        void saveToFile() const;
        void addStock(const Stock& stock) { 
            stocks.push_back(stock); 
        }
        Stock* findStock(const string& symbol);
        void updateProfitLoss(double profitLoss) { 
            overallProfitLoss += profitLoss; 
        }
        double getOverallProfitLoss() const { 
            return overallProfitLoss; 
        }
        double getTotalValue() const;
        const vector<Stock>& getStocks() const { 
            return stocks; 
        }
};

void Portfolio::saveToFile() const {
    ofstream file(FILENAME);
    if (!file) {
        throw runtime_error("Error: Unable to open file for saving portfolio data.");
    }
    file << stocks.size() << endl;
    file << overallProfitLoss << endl;
    for (const auto& stock : stocks) {
        file << left << setw(12) << stock.getSymbol();
        file << right << fixed << setprecision(2) << setw(10) << stock.getPrice();
        file << setw(10) << stock.getInitialPrice();
        file << setw(5) << stock.getQuantity() << endl;
    }
    cout << "Portfolio data saved successfully.\n";
}

void Portfolio::loadFromFile() {
    ifstream file(FILENAME);
    if (!file) {
        cout << "No previous portfolio data found. Starting with an empty portfolio.\n";
        return;
    }
    int numOfStocks;
    file >> numOfStocks >> overallProfitLoss;
    for (int i = 0; i < numOfStocks; ++i) {
        string symbol;
        double price, initialPrice;
        int quantity;
        file >> symbol >> price >> initialPrice >> quantity;
        stocks.emplace_back(symbol, price, quantity);
    }
    cout << "Portfolio data loaded successfully.\n";
}

Stock* Portfolio::findStock(const string& symbol) {
    for (auto& stock : stocks) {
        if (stock.getSymbol() == symbol) {
            return &stock;
        }
    }
    return nullptr;
}

double Portfolio::getTotalValue() const {
    double totalValue = 0;
    for (const auto& stock : stocks) {
        totalValue += stock.getPrice() * stock.getQuantity();
    }
    return totalValue;
}

class User {
    private:
        string username;
        string password;

    public:
        User(const string& username, const string& password) : username(username), password(password) {}

        static User createUser() {
            string username, password, confirm;
            cout << "Create a new username: ";
            cin >> username;
            cout << "Create a new password: ";
            cin >> password;
            cout << "Confirm your password: ";
            cin >> confirm;
            if (password != confirm) {
                throw runtime_error("Password confirmation failed.");
            }
            return User(username, password);
        }
        bool authenticate(const string& usernameInput, const string& passwordInput) const {
            return usernameInput == username && passwordInput == password;
        }
        void logout() const {
            cout << "Logging out user: " << username << endl;
        }
};

class Trader {
    private:
        Portfolio portfolio;
        User user;

        bool isValidSymbol(const string& symbol) const { 
            return symbol.length() <= 5; 
        }
        bool isValidQuantity(int quantity) const {
            return quantity > 0; 
        }
        bool isValidPrice(double price) const { 
            return price > 0; 
        }
        void fluctuatePrice(double& price) const;

    public:
        Trader(const User& user) : user(user) {}

        void buyStock();
        void sellStock();
        void checkPortfolio();
        void savePortfolio() { 
            portfolio.saveToFile(); 
        }
        void deletePortfolioFile();
        bool authenticateUser() const;
        void logout() { 
            user.logout(); 
        }
};

void Trader::fluctuatePrice(double& price) const {
    srand(time(nullptr));
    double change = (rand() % 11 - 5) / 100.0;
    price *= (1 + change);
}

void Trader::buyStock() {
    string symbol;
    double price;
    int quantity;

    cout << "Enter stock symbol: ";
    cin >> symbol;
    if (!isValidSymbol(symbol)) {
        throw runtime_error("Invalid stock symbol.");
    }
    cout << "Enter stock price: ";
    cin >> price;
    if (!isValidPrice(price)) {
        throw runtime_error("Invalid stock price.");
    }
    cout << "Enter quantity: ";
    cin >> quantity;
    if (!isValidQuantity(quantity)) {
        throw runtime_error("Invalid quantity.");
    }
    portfolio.addStock(Stock(symbol, price, quantity));
    cout << quantity << " shares of " << symbol << " at PKR " << price << " each bought successfully.\n";
}

void Trader::sellStock() {
    string symbol;
    int quantity;
    double sellingPrice;

    cout << "Enter stock symbol: ";
    cin >> symbol;

    Stock* stock = portfolio.findStock(symbol);
    if (!stock) {
        throw runtime_error("Stock not found.");
    }
    cout << "Enter quantity to sell: ";
    cin >> quantity;
    if (!isValidQuantity(quantity) || stock->getQuantity() < quantity) {
        throw runtime_error("Invalid input for selling stocks.");
    }
    cout << "Enter selling price: ";
    cin >> sellingPrice;
    if (!isValidPrice(sellingPrice)) {
        throw runtime_error("Invalid selling price.");
    }
    double profitLoss = (sellingPrice - stock->getInitialPrice()) * quantity;
    cout << quantity << " shares of " << symbol << " at PKR " << sellingPrice << " each sold successfully.\n";

    if (profitLoss > 0) {
        cout << "Profit: PKR " << profitLoss << ".\n";
    } 
    else if (profitLoss < 0) {
        cout << "Loss: PKR " << -profitLoss << ".\n";
    } 
    else {
        cout << "No profit or loss.\n";
    }
    portfolio.updateProfitLoss(profitLoss);
    stock->setQuantity(stock->getQuantity() - quantity);
}

void Trader::checkPortfolio() {
    cout << "\n===== Portfolio Summary =====\n";
    for (const auto& stock : portfolio.getStocks()) {
        double currentPrice = stock.getPrice();
        fluctuatePrice(currentPrice);
        cout << "Stock: " << stock.getSymbol() << ", Price: " << currentPrice << ", Quantity: " << stock.getQuantity() << endl;
        if (currentPrice > stock.getPreviousPrice()) {
            cout << "Price Up: Consider selling or holding this stock." << endl;
        } 
        else if (currentPrice < stock.getPreviousPrice()) {
            cout << "Price Down: Consider buying or holding off on selling this stock." << endl;
        } 
        else {
            cout << "Price Stable: Price remains stable. Monitor for future changes." << endl;
        }
        const_cast<Stock&>(stock).setPreviousPrice(currentPrice);
        cout << endl;
    }
    cout << "Overall Profit/Loss: PKR " << portfolio.getOverallProfitLoss() << endl;
    cout << "Portfolio Value: PKR " << portfolio.getTotalValue() << endl;
}

void Trader::deletePortfolioFile() {
    if (remove(FILENAME.c_str()) == 0) {
        cout << "Portfolio file deleted successfully.\n";
    }
    else {
        cerr << "Unable to delete portfolio file.\n";
    }
}

bool Trader::authenticateUser() const {
    string username, password;

    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    return user.authenticate(username, password);
}

int displayMenu() {
    int choice;
    while (true) {
        cout << "\n~~!!---- CTRL_FREAKS STOCK EXCHANGE(CSE) Menu ----!!~~\n";
        cout << "1. Buy Stock\n";
        cout << "2. Sell Stock\n";
        cout << "3. Check Portfolio\n";
        cout << "4. Save Portfolio\n";
        cout << "5. Delete Portfolio File\n";
        cout << "6. Logout\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        if (cin >> choice && choice >= 1 && choice <= 7) {
            return choice;
        }
        else {
            cout << "Invalid choice. Please enter a valid option.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main() {
    try {
        User currentUser = User::createUser();
        Trader trader(currentUser);

        while (!trader.authenticateUser()) {
            cout << "Authentication failed. Please try again.\n";
        }

        int choice;
        do {
            choice = displayMenu();

            switch (choice) {
                case 1:
                    trader.buyStock();
                    break;
                case 2:
                    trader.sellStock();
                    break;
                case 3:
                    trader.checkPortfolio();
                    break;
                case 4:
                    trader.savePortfolio();
                    break;
                case 5:
                    trader.deletePortfolioFile();
                    break;
                case 6:
                    trader.logout();
                    break;
                case 7:
                    cout << "Exiting the program. Goodbye!\n";
                    break;
            }
        } 
        while (choice != 7);
    } 
    catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}

