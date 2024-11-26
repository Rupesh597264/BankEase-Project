#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void displayMenu();
void createAccount();
void manageaccount();
void displayStatements();
void deposit();
void withdraw();
void transfer();

int main() {
    int choice;
    printf("Welcome to BankEase.\nA platform which provides you all facilities related to banking.");
    displayMenu();
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch (choice) {
    case 1: createAccount(); break;
    case 2: manageaccount(); break;
    case 3: printf("Exiting...\n"); break;
    default: printf("Invalid choice. Try again.\n");
    }
    return 0;
}

void displayMenu() {
    printf("\n       BankEase       \n");
    printf("1. Create Account\n");
    printf("2. Manage Account\n");
    printf("3. Exit\n");
}

typedef struct {
    int accountNumber;
    char name[50];
    float balance;
} Customer;

typedef struct {
    int accountNumber;
    char type[10]; // deposit or withdraw
    float amount;
} Transaction;

Customer customers[100];
Transaction transactions[100];
int customerCount = 0;
int transactionCount = 0;

void createAccount() {
    Customer c;
    printf("Enter account number: ");
    scanf("%d", &c.accountNumber);
    printf("Enter name: ");
    scanf("%s", c.name);
    c.balance = 0.0;
    customers[customerCount++] = c;
    printf("Account created successfully!\n");
}
