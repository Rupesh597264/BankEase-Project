#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function Prototypes
unsigned long simpleHash(char *str); // Hashing function for password security
void displayMenu();                 // Displays the main menu
void createAccount();               // Creates a new customer account
void login();                       // Allows an existing user to log in
void passwordHashing(char *password, unsigned long *hashedPassword); // Hashes user passwords
int authenticateUser(int accountNumber, unsigned long passwordHash); // Validates user credentials
void manageAccount(int accountIndex); // Manages post-login operations
void displayStatements(int accountIndex); // Displays transaction history
void fundTransfer(int accountIndex);     // Transfers funds between accounts
void deposit(int accountIndex);          // Handles deposit operations
void withdraw(int accountIndex);         // Handles withdrawal operations
void Balance(int accountIndex);          // Displays account balance
void Credits(int accountIndex);          // Shows credits received by the account
void AccountDetails(int accountIndex);   // Displays account details
void applyInsurance(int accountIndex);   // Allows users to apply for insurance
void applyLoan(int accountIndex);        // Allows users to apply for loans
void manageCards(int accountIndex);      // Allows user to manage debit cards
void deleteAccount(int accountIndex);    // Allows user to delete account
void saveData();                         // Saves customer and transaction data to files
void loadData();                         // Loads customer and transaction data from files

// Structure, which holds customers information
typedef struct {
    int accountNumber;           // Unique account number
    char name[50];               // Customer's full name
    unsigned long passwordHash;  // Hashed password for security
    float balance;               // Current account balance
    char email[50];              // Customer's email address
    char contactNumber[15];      // Customer's contact number
    char aadharNumber[15];       // Customer's Aadhaar number
} Customer;

// Structure, which holds transaction details
typedef struct {
    int accountNumber;           // Account initiating the transaction
    int toAccountNumber;         // Recipient account (for transfers)
    char type[10];               // Type of transaction (e.g., Deposit, Withdraw)
    float amount;                // Transaction amount
    char timestamp[25];          // Date and time of the transaction
} Transaction;

// Global Arrays
Customer customers[1000];        // Array to store customer data (max 1000 customers)
Transaction transactions[5000];   // Array to store transaction records (max 5000 transactions)

// Counters for data arrays
int customerCount = 0;           // Current number of customers
int transactionCount = 0;        // Current number of transactions

// Function to get the current timestamp in "YYYY-MM-DD HH:MM:SS" format
void getCurrentTimestamp(char *buffer) {
    time_t t = time(NULL);                  // Get current time
    struct tm *tm_info = localtime(&t);    // Convert time to local time structure
    strftime(buffer, 25, "%Y-%m-%d %H:%M:%S", tm_info); // Format time
}

int main() {
    int choice;

    // Load existing customer and transaction data from files
    loadData();

    printf("'''''''''''''WELCOME TO Swift Bank''''''''''''''\n");
    printf("        Fast, Simple, Secure Banking!!       \n");
    while (1) {
        // Main menu options
        printf("\n1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break; // Create a new account
            case 2: login(); break;         // Log in to an existing account
            case 3: 
                printf("Thank you for trusting SwiftBank.\nWe appreciate your trust in us.\nExiting...\n");
                saveData();  // Save data before exiting
                exit(0); 
            default: 
                printf("Invalid choice.\nTry again.\n"); 
        }
    }
    return 0;
}

// Function to create a new customer account
void createAccount() {
    Customer c;                
    char password[50];         // Temporary variable to store user's password

    printf("Enter account number: ");
    scanf("%d", &c.accountNumber);

    printf("Enter name: ");
    getchar();  // Clear newline character from input buffer
    fgets(c.name, 50, stdin); 
    c.name[strcspn(c.name, "\n")] = 0; // Remove newline character from name

    // Check if the account number already exists
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].accountNumber == c.accountNumber) {
            printf("Account number already exists.\nPlease try again.\n");
            return;
        }
    }

   
    printf("Enter email: ");
    fgets(c.email, 50, stdin);
    c.email[strcspn(c.email, "\n")] = 0;

    printf("Enter contact number: ");
    fgets(c.contactNumber, 15, stdin);
    c.contactNumber[strcspn(c.contactNumber, "\n")] = 0;

    printf("Enter Aadhar number: ");
    fgets(c.aadharNumber, 15, stdin);
    c.aadharNumber[strcspn(c.aadharNumber, "\n")] = 0;

    // Collect and hash the user's password
    printf("Enter password: ");
    fgets(password, 50, stdin); 
    password[strcspn(password, "\n")] = 0; 
    passwordHashing(password, &c.passwordHash);

    // Initialize the account balance
    c.balance = 0.0; 

    // Add the new customer to the customers array
    customers[customerCount++] = c;
    printf("Account created successfully!\n");
}

// Function to hash a user's password
void passwordHashing(char *password, unsigned long *hashedPassword) {
    *hashedPassword = simpleHash(password); // Call the hashing function
}

// Function to authenticate a user's login credentials
int authenticateUser(int accountNumber, unsigned long passwordHash) {
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].accountNumber == accountNumber) { // Match account number
            if (customers[i].passwordHash == passwordHash) { // Match password 
                return i; // Return index of authenticated user
            }
        }
    }
    return -1; // Return -1 if authentication fails
}

// Simple hashing function to secure passwords
unsigned long simpleHash(char *str) {
    unsigned long hash = 5381; // Initialize hash value
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // Hash computation
    }
    return hash;
}

// Function to log in an existing user
void login() {
    int accountNumber;
    char password[50];

    printf("Enter your account number: ");
    scanf("%d", &accountNumber);

    printf("Enter your password: ");
    getchar(); 
    fgets(password, 50, stdin); 
    password[strcspn(password, "\n")] = 0; 

    // Hash the entered password
    unsigned long passwordHash;
    passwordHashing(password, &passwordHash);

    // Authenticate the user
    int userIndex = authenticateUser(accountNumber, passwordHash);
    if (userIndex == -1) {
        printf("Invalid account number or password.\n");
        return;
    }

    printf("Login successful!\nWelcome %s.\n", customers[userIndex].name);
    manageAccount(userIndex); // Proceed to account management
}

// Function to manage account operations after login
void manageAccount(int accountIndex) {
    int choice;
    while (1) {
        // Display account management menu
        printf("\n     Manage Account     \n");
        printf("1. View Statement\n");
        printf("2. Fund Transfer\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. View Balance\n");
        printf("6. View Credits (from other accounts)\n");
        printf("7. View Account Details\n");
        printf("8. Apply for Insurance\n");
        printf("9. Apply for Loan\n");
        printf("10. Manage Cards\n");
        printf("11. Delete Account\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Execute user's choice
        switch (choice) {
            case 1: displayStatements(accountIndex); break; // View transaction history
            case 2: fundTransfer(accountIndex); break;     // Transfer funds to another account
            case 3: deposit(accountIndex); break;          // Deposit funds
            case 4: withdraw(accountIndex); break;         // Withdraw funds
            case 5: Balance(accountIndex); break;          // Check account balance
            case 6: Credits(accountIndex); break;          // View credits received
            case 7: AccountDetails(accountIndex); break;   // View account details
            case 8: applyInsurance(accountIndex); break;   // Apply for insurance
            case 9: applyLoan(accountIndex); break;        // Apply for a loan
            case 10: manageCards(accountIndex); break; // Manage cards feature
            case 11: deleteAccount(accountIndex); return; // Delete account feature
            case 12: 
                printf("Thank you for managing your account with SwiftBank.\n");
                printf("We appreciate your trust in us.\nExiting...\n"); 
                return; // Exit account management
            default: 
                printf("Invalid choice.\nTry again.\n"); 
        }
    }
}

// Function to display transaction history for an account
void displayStatements(int accountIndex) {
    printf("\nTransaction History for Account Number %d - %s\n", customers[accountIndex].accountNumber, customers[accountIndex].name);
    printf("----\n");

    // Iterate over transaction records and display relevant ones
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].accountNumber == customers[accountIndex].accountNumber) {
            printf("%s: %s of %.2f\n", transactions[i].timestamp, transactions[i].type, transactions[i].amount);
        }
    }
    printf("-------------------------------------------------\n");
}

// Function to transfer funds to another account
void fundTransfer(int accountIndex) {
    int toAccountNumber;
    float amount;
    printf("Enter the account number to transfer to: ");
    scanf("%d", &toAccountNumber);
    printf("Enter amount to transfer: ");
    scanf("%f", &amount);

    // Validate transfer amount
    if (amount <= 0) {
        printf("Invalid amount.\nAmount must be greater than zero.\n");
        return;
    }
    if (amount > customers[accountIndex].balance) {
        printf("Insufficient balance.\n");
        return;
    }

    // Find recipient account
    int toAccountIndex = -1;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].accountNumber == toAccountNumber) {
            toAccountIndex = i;
            break;
        }
    }
    if (toAccountIndex == -1) {
        printf("Recipient account not found.\n");
        return;
    }

    // Perform fund transfer
    customers[accountIndex].balance -= amount;
    customers[toAccountIndex].balance += amount;

    // Record the transaction
    Transaction t1 = {customers[accountIndex].accountNumber, toAccountNumber, "Transfer", amount};
    getCurrentTimestamp(t1.timestamp);
    transactions[transactionCount++] = t1;

    printf("Transfer successful.\nYour current balance: %.2f\n", customers[accountIndex].balance);
}

// Function to deposit money into an account
void deposit(int accountIndex) {
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    // Validate deposit amount
    if (amount <= 0) {
        printf("Invalid amount.\nAmount must be greater than zero.\n");
        return;
    }

    // Update account balance
    customers[accountIndex].balance += amount;

    // Record the deposit transaction
    Transaction t = {customers[accountIndex].accountNumber, 0, "Deposit", amount};
    getCurrentTimestamp(t.timestamp); // Get the current timestamp
    transactions[transactionCount++] = t; // Add transaction to the transaction array

    printf("Deposit successful.\nYour current balance: %.2f\n", customers[accountIndex].balance);
}

// Function to withdraw money from an account
void withdraw(int accountIndex) {
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    // Validate withdrawal amount
    if (amount <= 0) {
        printf("Invalid amount.\nAmount must be greater than zero.\n");
        return;
    }
    if (amount > customers[accountIndex].balance) {
        printf("Insufficient balance.\n");
        return;
    }

    // Update account balance
    customers[accountIndex].balance -= amount;

    // Record the withdrawal transaction
    Transaction t = {customers[accountIndex].accountNumber, 0, "Withdraw", amount};
    getCurrentTimestamp(t.timestamp); // Get the current timestamp
    transactions[transactionCount++] = t; // Add transaction to the transaction array

    printf("Withdrawal successful.\nYour current balance: %.2f\n", customers[accountIndex].balance);
}

// Function to display the current balance of an account
void Balance(int accountIndex) {
    printf("Your current balance: %.2f\n", customers[accountIndex].balance); // Display balance
}

// Function to display all credits received by an account
void Credits(int accountIndex) {
    printf("\nCredits Received for Account Number %d - %s\n", 
           customers[accountIndex].accountNumber, 
           customers[accountIndex].name);

    printf("----\n");

    // searching all transactions to find credits for the account
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].toAccountNumber == customers[accountIndex].accountNumber) {
            printf("%s: Credit of %.2f from Account %d\n", 
                   transactions[i].timestamp, 
                   transactions[i].amount, 
                   transactions[i].accountNumber);
        }
    }
    printf("-------------------------------------------------\n");
}

// Function to display account details
void AccountDetails(int accountIndex) {
    printf("\n     Account Details:     \n");
    printf("Account Number: %d\n", customers[accountIndex].accountNumber); // Display account number
    printf("Name: %s\n", customers[accountIndex].name);                   // Display customer's name
    printf("Email: %s\n", customers[accountIndex].email);                 // Display customer's email
    printf("Contact Number: %s\n", customers[accountIndex].contactNumber); // Display contact number
    printf("Aadhar Number: %s\n", customers[accountIndex].aadharNumber); // Display Aadhaar number
    printf("Balance: %.2f\n", customers[accountIndex].balance);          // Display account balance
}

// Function to manage insurance-related tasks
void applyInsurance(int accountIndex) {
    printf("\n     Insurance Management     \n");
    printf("1. View Insurance Details\n");
    printf("2. Apply for Insurance\n");
    printf("3. Exit\n");
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            // Display available insurance options
            printf("Insurance options include:\n");
            printf("1. Home Insurance\n");
            printf("2. Term Life Insurance\n");
            printf("3. Motor Insurance\n");
            printf("4. General Insurance\n");
            break;
        case 2:
            // Allow user to apply for a specific insurance category
            printf("Choose an insurance category to apply:\n");
            printf("1. Home Insurance\n");
            printf("2. Term Life Insurance\n");
            printf("3. Motor Insurance\n");
            printf("4. General Insurance\n");
            int insuranceChoice;
            scanf("%d", &insuranceChoice);
            if (insuranceChoice >= 1 && insuranceChoice <= 4) {
                printf("Insurance application successful.\n");
            } else {
                printf("Invalid choice.\n"); // Handle invalid input
            }
            break;
        case 3:
            printf("Exiting Insurance Management.\n");
            break;
        default:
            printf("Invalid choice.\n"); // Handle invalid input
    }
}

// Function to manage loan-related tasks
void applyLoan(int accountIndex) {
    printf("\n     Loan Management     \n");
    printf("1. View Loan Details\n");
    printf("2. Apply for Loan\n");
    printf("3. Exit\n");
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            // Display available loan categories
            printf("Loan Categories\n");
            printf("1. Home Loan\n");
            printf("2. Car Loan\n");
            printf("3. Education Loan\n");
            printf("4. Business Loan\n");
            printf("5. Loan Against Property\n");
            break;
        case 2:
            // Allow user to apply for a specific loan
            printf("Choose a loan category to apply:\n");
            printf("1. Home Loan\n");
            printf("2. Car Loan\n");
            printf("3. Education Loan\n");
            printf("4. Business Loan\n");
            printf("5. Loan Against Property\n");
            int loanChoice;
            float loanAmount;
            scanf("%d", &loanChoice);
            if (loanChoice >= 1 && loanChoice <= 5) {
                printf("Enter loan amount (up to 50,00,000): ");
                scanf("%f", &loanAmount);
                if (loanAmount > 0 && loanAmount <= 5000000) {
                    printf("Loan application successful for amount %.2f.\n", loanAmount);
                } else {
                    printf("Invalid loan amount.\n"); // Handle invalid loan amount
                }
            } else {
                printf("Invalid loan type.\n"); // Handle invalid loan type
            }
            break;
        case 3:
            printf("Exiting Loan Management.\n");
            break;
        default:
            printf("Invalid choice.\n"); // Handle invalid input
    }
}

// Function to manage debit cards (view, request, block, set limit)
void manageCards(int accountIndex) {
    int choice;
    static float debitCardLimit = 500000.0; // Default limit for the debit card

    printf("\n     Manage Debit Cards     \n");
    printf("1. View Debit Card Details\n");
    printf("2. Request New Debit Card\n");
    printf("3. Set Debit Card Limit\n");
    printf("4. Block Debit Card\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            // View details of the debit card
            printf("\nYou have a Debit Card linked to your account.\n");
            printf("Card Number: XXXX-XXXX-XXXX-1234\n"); // Placeholder card number
            printf("Status: Active\n");
            printf("Current Limit: %.2f\n", debitCardLimit); // Display current limit
            break;
        case 2:
            // Request a new debit card
            printf("\nRequesting a new Debit Card...\n");
            printf("Your Debit Card request is successful.\nIt will be delivered to your registered address.\n");
            break;
        case 3:
            // Set a new limit for the debit card
            printf("\nSet a New Debit Card Limit\n");
            printf("Enter the new limit : ");
            float newLimit;
            scanf("%f", &newLimit);
            if (newLimit > 0) {
                debitCardLimit = newLimit;
                printf("Your Debit Card limit has been updated to %.2f\n", debitCardLimit);
            } else {
                printf("Invalid limit.\nThe limit must be greater than zero.\n");
            }
            break;
        case 4:
            // Block the debit card
            printf("\nBlocking your Debit Card...\n");
            printf("Your Debit Card has been successfully blocked.\nPlease contact customer support for assistance.\n");
            break;    
        case 5:
            // Exit from the manage debit cards menu
            printf("Exiting Debit Card Management.\n");
            break;
        default:
            // Handle invalid input
            printf("Invalid choice.\n");
    }
}

// Function to delete an account
void deleteAccount(int accountIndex) {
    char confirmation[10];
    printf("\n     Delete Account     \n");
    printf("Are you sure you want to delete your account?\n(yes/no): ");
    scanf("%s", confirmation);

    if (strcmp(confirmation, "yes") == 0 || strcmp(confirmation, "YES") == 0) {
        // Shift all subsequent accounts to fill the gap
        for (int i = accountIndex; i < customerCount - 1; i++) {
            customers[i] = customers[i + 1];
        }
        customerCount--;

        // Remove associated transactions
        int newTransactionCount = 0;
        for (int i = 0; i < transactionCount; i++) {
            if (transactions[i].accountNumber != customers[accountIndex].accountNumber) {
                transactions[newTransactionCount++] = transactions[i];
            }
        }
        transactionCount = newTransactionCount;

        printf("Your account has been successfully deleted.\n");
    } else {
        printf("Account deletion cancelled.\n");
    }
}

// Function to save customer and transaction data to files
void saveData() {
    // Save customer data
    FILE *customerFile = fopen("customers.dat", "w");
    if (customerFile) {
        fwrite(customers, sizeof(Customer), customerCount, customerFile); // Write customer data
        fclose(customerFile);
    }

    // Save transaction data
    FILE *transactionFile = fopen("transactions.dat", "w");
    if (transactionFile) {
        fwrite(transactions, sizeof(Transaction), transactionCount, transactionFile); // Write transaction data
        fclose(transactionFile);
    }
}

// Function to load customer and transaction data from files
void loadData() {
    // Load customer data
    FILE *customerFile = fopen("customers.dat", "r");
    if (customerFile) {
        customerCount = fread(customers, sizeof(Customer), 1000, customerFile); // Read customer data
        fclose(customerFile);
    }

    // Load transaction data
    FILE *transactionFile = fopen("transactions.dat", "r");
    if (transactionFile) {
        transactionCount = fread(transactions, sizeof(Transaction), 5000, transactionFile); // Read transaction data
        fclose(transactionFile);
    }
}
