#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "online_shop2.h"

void createAccount(const char *name, const char *surname) {
    FILE *file = fopen("C:\\faculta\\PP\\online_shop_2\\clients.csv", "a");
    if (file == NULL) {
        printf("Error: Unable to open the file.\n");
        return;
    }

    fprintf(file, "%s,%s\n", name, surname);
    fclose(file);
    printf("Account created successfully.\n");
}


int checkUserExistence(const char *name, const char *surname) {
    FILE *file = fopen("C:\\faculta\\PP\\online_shop_2\\clients.csv", "r");
    if (file == NULL) {
        printf("Error: Unable to open the file.\n");
        return 0;
    }

    char line[MAX_NAME_LENGTH + MAX_SURNAME_LENGTH + 2]; //+2 for comma and newline
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        if (strcmp(token, name) == 0) {
            token = strtok(NULL, ",");
            token[strcspn(token, "\n")] = '\0';
            if (strcmp(token, surname) == 0) {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

void displayOptions() {
    printf("1. Place Order\n");
    printf("2. Sell Product\n");
    printf("3. View Placed Orders\n");
    printf("4. Search Product\n");
    printf("5. Exit\n");
}



void login(struct Client *client) {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_SURNAME_LENGTH];

    printf("Enter your name: ");
    scanf("%s", name);
    printf("Enter your surname: ");
    scanf("%s", surname);

    if (checkUserExistence(name, surname)) {
        printf("Welcome, %s %s!\n", name, surname);
        client->name = malloc(strlen(name) + 1);
        strcpy(client->name, name);
        client->surname = malloc(strlen(surname) + 1);
        strcpy(client->surname, surname);


        int choice;
        do {
            displayOptions();
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    place_order("C:\\faculta\\PP\\online_shop_2\\products.txt","C:\\faculta\\PP\\online_shop_2\\orders_file.txt",client->name,client->surname);
                    break;
                case 2:
                    sell_product("C:\\faculta\\PP\\online_shop_2\\products.txt");
                    break;

                case 3:
                    view_placed_orders("C:\\faculta\\PP\\online_shop_2\\orders_file.txt",client->name,client->surname);
                    break;

                case 4:
                    search_products_options("C:\\faculta\\PP\\online_shop_2\\products.txt");
                    break;

                case 5:
                    printf("Exiting...\n");
                    break;

                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 5);
    } else {
        printf("User not found.\n");
    }
}

void view_placed_orders(const char* filename,const char* customer_name, const char* customer_surname) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    printf("Orders for %s %s:\n", customer_name, customer_surname);
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char name[MAX_NAME_LENGTH], surname[MAX_NAME_LENGTH], product[MAX_NAME_LENGTH];
        int quantity;
        sscanf(line, "%[^,],%[^,],%[^,],%d", name, surname, product, &quantity);
        if (strcmp(name, customer_name) == 0 && strcmp(surname, customer_surname) == 0) {
            printf("Product: %s, Quantity: %d\n", product, quantity);
        }
    }

    fclose(file);
}


void place_order(const char* filename,const char* orders_file, const char* customer_name, const char* customer_surname) {
    char product_name[MAX_NAME_LENGTH];
    int quantity;


    printf("Enter the name of the product:\n");
    fflush(stdin); // Clear the input buffer
    fgets(product_name, sizeof(product_name), stdin);
    product_name[strcspn(product_name, "\n")] = '\0';


    printf("Enter the quantity: ");
    scanf("%d", &quantity);
    if(quantity<0) {
        printf("Error: Please enter a positive quantity.\n");
        return;
    }



    if (is_product_available(filename, product_name) && is_quantity_available(filename, product_name, quantity)) {

        update_product_quantity(filename, product_name, quantity);
        printf("Order placed successfully.\n");

        FILE* file = fopen(orders_file, "a");
        if (file == NULL) {
            printf("Error: Unable to open file %s\n", orders_file);
            return;
        }


        fprintf(file, "%s,%s,%s,%d\n", customer_name,customer_surname, product_name, quantity);
        fclose(file);
    } else {
        printf("Order could not be placed. Product or quantity not available.\n");
    }
}


int is_product_available(const char* filename, const char* product_name) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char name[MAX_NAME_LENGTH];
        if (sscanf(line, "%[^,]", name) == 1) {
            if (strcmp(name, product_name) == 0) {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}


int is_quantity_available(const char* filename, const char* product_name, int requested_quantity) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char name[MAX_NAME_LENGTH];
        int quantity;
        if (sscanf(line, "%[^,],%d", name, &quantity) == 2) {
            if (strcmp(name, product_name) == 0 && quantity >= requested_quantity) {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}


void update_product_quantity(const char* filename, const char* product_name, int ordered_quantity) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    FILE* temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        printf("Error: Unable to create temporary file\n");
        fclose(file);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char name[MAX_NAME_LENGTH];
        int quantity;
        float price;
        if (sscanf(line, "%[^,],%d,%f", name, &quantity, &price) == 3) {
            if (strcmp(name, product_name) == 0) {
                quantity -= ordered_quantity;
                if (quantity < 0) {
                    quantity = 0; // Ensure quantity doesn't go negative
                }
                // Write the updated line to the temporary file
                fprintf(temp_file, "%s,%d,%.2f\n", name, quantity, price);
            } else {
                // Write the original line to the temporary file
                fprintf(temp_file, "%s,%d,%.2f\n", name, quantity, price);
            }
        }
    }

    fclose(file);
    fclose(temp_file);

    remove(filename);
    rename("temp.txt", filename);
}





void sell_product(const char* filename) {

    char product_name[100];
    int quantity;
    float price;

    printf("What do you want to sell?\n");
    printf("Enter the name of the product:\n");
    fflush(stdin); // Clear the input buffer
    fgets(product_name, sizeof(product_name), stdin);
    product_name[strcspn(product_name, "\n")] = '\0';

    printf("Enter the quantity:\n");
    scanf("%d",&quantity);
    printf("Enter entire selling price:\n");
    scanf("%f", &price);


    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    // Check if the file is not empty
    if (fgetc(file) != EOF) {
        fseek(file, -1, SEEK_END); // Move the file pointer to the end of the file
        // Check if the last character is not a newline
        if (fgetc(file) != '\n') {
            fprintf(file, "\n"); // Add a newline character
        }
    }
    rewind(file);

    float new_price;
    new_price=(price/quantity)+0.5*(price/quantity);
    fprintf(file, "%s,%d,%.2f\n", product_name, quantity, new_price);
    fclose(file);

    printf("Product sold successfully for the price: %.2f\n", price);
}






void search_products_options() {

    int choice;
    printf("If you want to search product by name and price press 1.\n");
    printf("If you want to search product by price press 2.\n");
    printf("If you want to search product by name press 3.\n");
    printf("Enter your choice: \n");
    scanf("%d", &choice);
    switch (choice) {

        case 1:
            search_product_by_name_price("C:\\faculta\\PP\\online_shop_2\\products.txt");
            break;
        case 2:
            search_product_by_price("C:\\faculta\\PP\\online_shop_2\\products.txt");
            break;
        case 3:
            search_product_by_name("C:\\faculta\\PP\\online_shop_2\\products.txt");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

void search_product_by_name_price(char* filename){
    char search_query[MAX_NAME_LENGTH];
    float minimum_price, maximum_price;
    printf("Enter product name or price:\n");
    fgets(search_query, sizeof(search_query), stdin);
    search_query[strcspn(search_query, "\n")] = '\0';
    scanf("%s", search_query);
    printf("Enter the minimum price:");
    scanf("%f", &minimum_price);
    printf("Enter the maximum price:");
    scanf("%f", &maximum_price);
    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    printf("Matching Products:\n");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {

        if (strstr(line, search_query) != NULL) {
            char name[MAX_NAME_LENGTH];
            int quantity;
            float price;
            if (sscanf(line, "%[^,],%d,%f", name, &quantity, &price) == 3) {
                if (price >= minimum_price && price <= maximum_price)
                    printf("Name: %s, Quantity: %d, Price: %.2f\n", name, quantity, price);
            }
        }
    }

    fclose(file);
}

void search_product_by_name(char* filename){
    char search_query[MAX_NAME_LENGTH];

    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter product name:\n");
    fgets(search_query, sizeof(search_query), stdin);
    search_query[strcspn(search_query, "\n")] = '\0';

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    printf("Matching Products:\n");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char name[MAX_NAME_LENGTH];
        int quantity;
        float price;
        if (sscanf(line, "%[^,],%d,%f", name, &quantity, &price) == 3) {
            if (strstr(name, search_query) != NULL) {
                printf("Name: %s, Quantity: %d, Price: %.2f\n", name, quantity, price);
            }
        }
    }

    fclose(file);
}



void search_product_by_price(char* filename){
    char search_query[MAX_NAME_LENGTH];
    float minimum_price, maximum_price;

    printf("Enter the minimum price:");
    scanf("%f", &minimum_price);
    printf("Enter the maximum price:");
    scanf("%f", &maximum_price);
    // Clear input buffer
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    printf("Matching Products:\n");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Check if the search query is found in the line
        if (strstr(line, search_query) != NULL) {
            char name[MAX_NAME_LENGTH];
            int quantity;
            float price;
            if (sscanf(line, "%[^,],%d,%f", name, &quantity, &price) == 3) {
                if (price >= minimum_price && price <= maximum_price)
                printf("Name: %s, Quantity: %d, Price: %.2f\n", name, quantity, price);
            }
        }
    }

    fclose(file);
}
