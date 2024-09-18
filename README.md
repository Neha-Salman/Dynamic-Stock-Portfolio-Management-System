Portfolio Management System

Overview:
This C++ program simulates a basic stock trading system where users can manage their stock portfolios by buying, selling, and tracking their investments. The system allows users to:
- Buy and sell stocks based on current prices and quantities.
- Check the overall portfolio, including stock details, total value, and profit/loss.
- Save and load portfolio data from a file.
- Simulate stock price fluctuations to help users make buying or selling decisions.

Features:
- User Authentication: Users must create a username and password to access the system.
- Stock Management: Buy and sell stocks using a ticker symbol, price, and quantity.
- Profit/Loss Tracking: Automatically calculates profit or loss on stock sales based on the initial purchase price.
- Portfolio Persistence: Save portfolio data to a file and reload it when the program restarts.
- Price Fluctuation Simulation: Randomly fluctuates stock prices to simulate market dynamics.

File Structure:
portfolio_data.txt: Stores user portfolio data including stocks and profit/loss information.

How It Works:
1. User Authentication: A new user is required to create a username and password. Authentication checks are performed for each login attempt.
2. Buying Stocks: The user inputs the stock symbol, price, and quantity to add to their portfolio.
3. Selling Stocks: Users can sell stocks they own. The system calculates profit or loss based on the initial buying price.
4. Check Portfolio: Users can view details about their stocks, including price fluctuations and overall portfolio performance.
5. File Management: The portfolio data is saved to a file (portfolio_data.txt) which can be loaded when the program restarts.
6. Exiting: Users can log out, delete their portfolio file, or exit the program.

Error Handling:
- File I/O Errors: The program gracefully handles errors while saving/loading the portfolio data file.
- Invalid Input: If invalid stock symbols, prices, or quantities are provided, appropriate error messages are displayed.

Dependencies: 
Standard C++ libraries: <iostream>, <fstream>, <iomanip>, <vector>, <stdexcept>, <string>, <cstdlib>, <ctime>, <limits>.
