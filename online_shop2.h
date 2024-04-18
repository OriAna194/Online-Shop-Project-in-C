#ifndef ONLINE_SHOP_ONLINE_SHOP_H
#define ONLINE_SHOP_ONLINE_SHOP_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_NAME_LENGTH 256
#define MAX_LINE_LENGTH 256
#define MAX_SURNAME_LENGTH 100
#define MAX_FILENAME_LENGTH 256



struct Client{
    char *name;
    char *surname;
};


void createAccount(const char *name, const char *surname);
int checkUserExistence(const char *name, const char *surname);
void displayOptions();
void login(struct Client *client);
void view_placed_orders(const char* filename,const char* customer_name, const char* customer_surname);
void place_order(const char* filename,const char* orders_file, const char* customer_name, const char* customer_surname);
int is_product_available(const char* filename, const char* product_name);
int is_quantity_available(const char* filename, const char* product_name, int requested_quantity);
void update_product_quantity(const char* filename, const char* product_name, int ordered_quantity);
void sell_product(const char* filename);
void search_products_options();
void search_product_by_name_price(char* filename);
void search_product_by_name(char* filename);
void search_product_by_price(char* filename);


#endif //ONLINE_SHOP_ONLINE_SHOP_H
