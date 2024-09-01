#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct items {
    char item[20];
    float price;
    int qty;
};

struct orders {
    char customer[50];
    char date[50];
    int numOfItems;
    struct items itm[50];
};

// Function to generate the bill header
void generateBillHeader(char name[50], char date[30]) {
    printf("\n\n");
    printf("\t ADV. Restaurant");
    printf("\n\t ----------------- ");
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("------------------------------");
    printf("\nItems\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n------------------------------");
    printf("\n");
}

// Function to generate the bill body
void generateBillBody(char item[30], int qty, float price) {
    printf("%s\t\t", item);
    printf("%d\t\t", qty);
    printf("%.2f\t\t", qty * price);
    printf("\n");
}

// Function to generate the bill footer
void generateBillFooter(float total) {
    printf("\n");
    float dis = 0.1 * total;
    float netTotal = total - dis;
    float cgst = 0.09 * netTotal;
    float grandTotal = netTotal + 2 * cgst;
    printf("----------------------------------\n");
    printf("Sub Total \t\t\t%.2f", total);
    printf("\nDiscount @10%s\t\t\t%.2f", "%", dis);
    printf("\n\t\t\t-------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nCGST @9\t\t\t%.2f", cgst);
    printf("\nSGST @9\t\t\t%.2f", cgst);
    printf("\n-------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n-----------------------------\n");
}

int main() {
    float total;
    int opt, n, invoiceFound;
    struct orders ord;
    struct orders order;
    char saveBill = 'y';
    char name[50];
    FILE *fp;

    // Dashboard
    while (1) {
        printf("\t============== ADV. RESTAURANT ============");
        printf("\nPlease select your preferred operation\t");
        printf("\n\n1. Generate Invoice");
        printf("\n2. Show all Invoices");
        printf("\n3. Search Invoices");
        printf("\n4. Exit");
        printf("\n\nYour choice: ");
        scanf("%d", &opt);
        fgetc(stdin); // Consume newline

        switch (opt) {
            case 1:
                system("cls"); // Use "clear" on Unix-based systems
                printf("\nPlease Enter the name of the customer:\t");
                fgets(ord.customer, 50, stdin);
                ord.customer[strlen(ord.customer) - 1] = '\0'; // Remove newline
                strcpy(ord.date, __DATE__);
                printf("\nPlease Enter the number of items:\t ");
                scanf("%d", &n);
                ord.numOfItems = n;

                total = 0; // Reset total for new invoice

                for (int i = 0; i < n; i++) {
                    fgetc(stdin); // Consume newline
                    printf("\n\n");
                    printf("Please enter item %d:\t", i + 1);
                    fgets(ord.itm[i].item, 20, stdin);
                    ord.itm[i].item[strlen(ord.itm[i].item) - 1] = '\0'; // Remove newline
                    printf("\nPlease enter the quantity:\t");
                    scanf("%d", &ord.itm[i].qty);
                    printf("\nPlease enter the unit price:\t");
                    scanf("%f", &ord.itm[i].price);
                    total += ord.itm[i].qty * ord.itm[i].price;
                }

                generateBillHeader(ord.customer, ord.date);
                for (int i = 0; i < ord.numOfItems; i++) {
                    generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                }

                generateBillFooter(total);

                printf("\nDo you want to save the Invoice [y/n]:\t");
                scanf(" %c", &saveBill);

                if (saveBill == 'y') {
                    fp = fopen("RestaurantBill1.dat", "a+");
                    if (fp == NULL) {
                        printf("\nError opening file for saving.\n");
                        break;
                    }
                    fwrite(&ord, sizeof(struct orders), 1, fp);
                    if (ferror(fp) == 0) {
                        printf("\nSuccessfully saved");
                    } else {
                        printf("\nError saving");
                    }
                    fclose(fp);
                }
                break;

            case 2:
                system("cls"); // Use "clear" on Unix-based systems
                fp = fopen("RestaurantBill1.dat", "r");
                if (fp == NULL) {
                    printf("\nError opening file for reading.\n");
                    break;
                }
                printf("\n  ***** Your Previous Invoices *****\n");
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    total = 0; // Reset total for each invoice
                    generateBillHeader(order.customer, order.date);
                    for (int i = 0; i < order.numOfItems; i++) {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        total += order.itm[i].qty * order.itm[i].price;
                    }
                    generateBillFooter(total);
                }
                fclose(fp);
                break;

            case 3:
                printf("\nEnter the name of the customer:\t");
                fgets(name, 50, stdin);
                name[strlen(name) - 1] = '\0'; // Remove newline
                system("cls");
                fp = fopen("RestaurantBill1.dat", "r");
                if (fp == NULL) {
                    printf("\nError opening file for reading.\n");
                    break;
                }
                printf("\n  ***** Invoice of %s *****\n", name);
                invoiceFound = 0; // Reset invoiceFound for search
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    total = 0; // Reset total for each invoice found
                    if (strcmp(order.customer, name) == 0) {
                        generateBillHeader(order.customer, order.date);
                        for (int i = 0; i < order.numOfItems; i++) {
                            generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                            total += order.itm[i].qty * order.itm[i].price;
                        }
                        generateBillFooter(total);
                        invoiceFound = 1;
                        break; // Stop searching once the invoice is found
                    }
                }
                if (!invoiceFound) {
                    printf("Sorry, the invoice for %s does not exist.\n", name);
                }
                fclose(fp);
                break;

            case 4:
                printf("\nExiting...\n");
                return 0;

            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    return 0;
}
