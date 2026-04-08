#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTS 16
#define FILENAME "inventory.txt"

typedef struct {
    char product_name[32];
    char department[16];
    int quantity;
    int SKU;
    float price;
} product;

void load_inventory(product products[], int *count);
void save_inventory(product products[], int count);
void add_product(product products[], int *count);
void remove_product(product products[], int *count);
void change_price(product products[], int count);
void update_quantity(product products[], int count);
void display_inventory(product products[], int count);
int find_product_by_sku(product products[], int count, int sku);

int main() {
    product products[MAX_PRODUCTS];
    int count = 0;
    char choice;

    load_inventory(products, &count);

    do {
        printf("\n[A]dd an item\n");
        printf("[R]emove an item\n");
        printf("[C]hange price\n");
        printf("[D]isplay inventory\n");
        printf("[U]pdate quantity\n");
        printf("[Q]uit\n");
        printf("What do you want to do? ");
        scanf(" %c", &choice);

        switch(choice) {
            //Accounts for both upper & lower case inputs from the user
            case 'A': case 'a':
                add_product(products, &count);
                break;
            case 'R': case 'r':
                remove_product(products, &count);
                break;
            case 'C': case 'c':
                change_price(products, count);
                break;
            case 'U': case 'u':
                update_quantity(products, count);
                break;
            case 'D': case 'd':
                display_inventory(products, count);
                break;
            case 'Q': case 'q':
                save_inventory(products, count);
                printf("Inventory saved. Goodbye!\n");
                break;
            default:
                printf("Invalid option.\n");
        }

    } while(choice != 'Q' && choice != 'q');

    return 0;
}
void load_inventory(product products[], int *count) {
    FILE *file = fopen(FILENAME, "r");

    if (file == NULL) {
        file = fopen(FILENAME, "w");
        fclose(file);
        return;
    }

    while (*count < MAX_PRODUCTS &&fscanf(file, "%31[^\n]\n", products[*count].product_name) == 1) 
    {

        fscanf(file, "%15[^\n]\n", products[*count].department);
        fscanf(file, "%d\n", &products[*count].quantity);
        fscanf(file, "%d\n", &products[*count].SKU);
        fscanf(file, "%f\n", &products[*count].price);

        (*count)++;
    }

    fclose(file);
}

void save_inventory(product products[], int count) {
    FILE *file = fopen(FILENAME, "w");

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n", products[i].product_name);
        fprintf(file, "%s\n", products[i].department);
        fprintf(file, "%d\n", products[i].quantity);
        fprintf(file, "%d\n", products[i].SKU);
        fprintf(file, "%f\n", products[i].price);
    }

    fclose(file);
}
void add_product(product products[], int *count) {
    if (*count >= MAX_PRODUCTS) {
        printf("Inventory full.\n");
        return;
    }

    printf("Enter product name: ");
    scanf(" %31[^\n]", products[*count].product_name);

    printf("Enter department: ");
    scanf(" %15[^\n]", products[*count].department);

    printf("Enter SKU: ");
    scanf("%d", &products[*count].SKU);

    printf("Enter quantity: ");
    scanf("%d", &products[*count].quantity);

    printf("Enter price: ");
    scanf("%f", &products[*count].price);

    (*count)++;
}

void remove_product(product products[], int *count) {
    int sku;
    printf("Enter SKU to remove: ");
    scanf("%d", &sku);

    int index = find_product_by_sku(products, *count, sku);

    if (index == -1) {
        printf("Product not found.\n");
        return;
    }

    for (int i = index; i < *count - 1; i++) {
        products[i] = products[i + 1];
    }

    (*count)--;
    printf("Product removed.\n");
}

void change_price(product products[], int count) {
    int sku;
    printf("For which SKU? ");
    scanf("%d", &sku);

    int index = find_product_by_sku(products, count, sku);

    if (index == -1) {
        printf("Product not found.\n");
        return;
    }

    printf("Old price: %.2f\n", products[index].price);
    printf("New price: ");
    scanf("%f", &products[index].price);
}

void update_quantity(product products[], int count) {
    int sku;
    printf("For which SKU? ");
    scanf("%d", &sku);

    int index = find_product_by_sku(products, count, sku);

    if (index == -1) {
        printf("Product not found.\n");
        return;
    }

    printf("Old quantity: %d\n", products[index].quantity);
    printf("New quantity: ");
    scanf("%d", &products[index].quantity);
}

void display_inventory(product products[], int count) {
    for (int i = 0; i < count; i++) {

        if (strcmp(products[i].department, "grocery") == 0 &&
            products[i].quantity < 2) {
            printf("Order more of this: ************************\n");
        }

        printf("Item Name: %-30s\n", products[i].product_name);
        printf("SKU: %-10d\n", products[i].SKU);
        printf("Quantity: %-10d\n", products[i].quantity);
        printf("Price: $%-10.2f\n", products[i].price);
        printf("--------------------------------------------\n");
    }
}

int find_product_by_sku(product products[], int count, int sku) {
    for (int i = 0; i < count; i++) {
        if (products[i].SKU == sku)
            return i;
    }
    return -1;
}