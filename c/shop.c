/*
Name: Eoin Stankard - G00300390
MULTI-PARADIGM-PROGRAMMING
Shop Assignment - C Procedural Programming
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*****************************************************************************************************
 * Shop dataclasses - These will be pointed to when creating Shop,Customer 
                      and productstock
*****************************************************************************************************/
struct Product {
	char* name;
	double price;
};

struct ProductStock {
	struct Product product;
	int quantity;
};

struct Shop {
	double cash;
	struct ProductStock stock[20];
	int index;
};

struct Customer {
	char* name;
	double budget;
	struct ProductStock shoppingList[10];
	int index;
};

/*****************************************************************************************************
 * The function below will update the shop stock if a customer makes a successful purchase
*****************************************************************************************************/
struct Shop updateStock(struct Shop s, struct Customer c){
	int i=0;
	while (i < s.index)
	{
		char quantityRemaining;
        for(int x = 0; x < c.index; x++){
			if(strcmp(c.shoppingList[x].product.name,s.stock[i].product.name) == 0){
				quantityRemaining = s.stock[i].quantity - c.shoppingList[x].quantity;
                s.stock[i].quantity = quantityRemaining;
			}
		}
        i++;
	}
    return s;
}

/*****************************************************************************************************
 * Checks the shop for a product and prints the quantity
*****************************************************************************************************/
void checkStock(struct Shop s){
	printf("\n*****SHOP STOCK****\n");
	for(int i = 0; i <= s.index; i++)
	{
		printf("PRODUCT NAME: %s  \nPRODUCT QUANTITY:%d\n\n",s.stock[i].product.name,s.stock[i].quantity);
	}
}

/*****************************************************************************************************
 * Prints the products name and price
*****************************************************************************************************/
void printProduct(struct Product p)
{
	printf("PRODUCT NAME: %s \nPRODUCT PRICE: $%.2f\n", p.name, p.price);
}

/*****************************************************************************************************
 * The below function is called and takes the data that is read in from the customer csv
 * This fucntion then does error checking to see if the customer can have a successful order
 * It checks if the quantity of a product the customer wants is possible
 * It will print a message if successful or not
 * Its will also check for things like empty basket and insufficient funds
*****************************************************************************************************/
struct Shop printCustomer(struct Customer c,struct Shop s)
{
    printf("\n------------------------------ CUSTOMER ------------------------------\n");    
    
	printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: $%.2f\n", c.name, c.budget);
    printf("\n------------------------------ PRODUCTS ------------------------------\n");
	double total = 0.0;
    double cost = 0.0;
	for(int i = 0; i < c.index; i++)
	{
		printProduct(c.shoppingList[i].product);
		printf("%s ORDERS %d OF ABOVE PRODUCT\n", c.name, c.shoppingList[i].quantity);
        for(int x =0; x < s.index; x++){
            if(strcmp(c.shoppingList[i].product.name,s.stock[x].product.name) == 0){
				if(c.shoppingList[i].quantity <= s.stock[x].quantity){
                    printf("***Successful Order***\n");
                    cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price;
                    total = total + cost;
                }else if(c.shoppingList[i].quantity > s.stock[x].quantity){
                    c.shoppingList[i].quantity = 0;
				    printf("***UnSuccessful Order - Only %d in stock***\n",s.stock[x].quantity);
			    }
			}
        }		
        printf("----------------------------------------------------------------------\n");
	}
	double change = c.budget - total;
	
    printf("----------------------------- TOTAL BILL -----------------------------\n");

	printf("The total cost will be $%.2f\n",total);

    if(c.budget >= total && c.index >0 && total > 0.00){
        s.cash = s.cash + total;
        printf("----------------------------------------------------------------------\n");
        printf("                    ***TRANSACTION CONFIRMED***\n\n");
        printf("The change given is $%.2f\nShop Balance is $%.2f\n",change,s.cash);
        printf("----------------------------------------------------------------------\n");
        s = updateStock(s,c);
    }else if(c.index == 0){
        printf("----------------------------------------------------------------------\n");
        printf("         ***TRANSACTION CANCELLED - No Items in Basket***\n\n");
        printf("Shop Balance is $%.2f\n",s.cash);
        printf("----------------------------------------------------------------------\n");

    }else if(total == 0.00){
        printf("----------------------------------------------------------------------\n");
        printf("         ***TRANSACTION CANCELLED - No Items in Basket***\n\n");
        printf("Shop Balance is $%.2f\n",s.cash);
        printf("----------------------------------------------------------------------\n");

    }else{
        printf("----------------------------------------------------------------------\n");
        printf("         ***TRANSACTION CANCELLED - Insufficient Funds***\n\n");
        printf("Shop Balance is $%.2f\n",s.cash);
        printf("----------------------------------------------------------------------\n");
    }
    total=0.0;
    cost=0.0;
    return s;
}

/*****************************************************************************************************
 * The function below will open the Stock.csv file and import all the data it contains
 * First value read will be the shop cash
 * The next values will be the Product, Price and Quantity in Stock
*****************************************************************************************************/
struct Shop createAndStockShop()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("../stock.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

	read = getline(&line, &len, fp);
	float cash = atof(line);
	
	struct Shop shop = { cash };

    while ((read = getline(&line, &len, fp)) != -1) {
		char *n = strtok(line, ",");
		char *p = strtok(NULL, ",");
		char *q = strtok(NULL, ",");
		int quantity = atoi(q);
		double price = atof(p);
		char *name = malloc(sizeof(char) * 50);
		strcpy(name, n);
		struct Product product = { name, price };
		struct ProductStock stockItem = { product, quantity };
		shop.stock[shop.index++] = stockItem;
    }
	
	return shop;
}

/*****************************************************************************************************
 * The function below will open the Customer csv file and import all the data it contains
 * first line will contain customers name and budget
 * The lines following will contain the wanted products and quantity
*****************************************************************************************************/
struct Customer read_Customer(struct Shop s,char *csv){
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(csv, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

	read = getline(&line, &len, fp);
	char *n = strtok(line, ",");
	char *b = strtok(NULL, ",");
	double budget = atof(b);
	char *custName = malloc(sizeof(char) * 50);
	strcpy(custName, n);
	struct Customer customer = { custName,budget};

	while ((read = getline(&line, &len, fp)) != -1) {
		char *n = strtok(line, ",");
		char *q = strtok(NULL, ",");
		int quantity = atoi(q);
		char *name = malloc(sizeof(char) * 50);
		strcpy(name, n);
		int code = 0;
		
		for(int i = 0; i < s.index; i++){
			if(strcmp(name,s.stock[i].product.name) == 0){
				struct Product product = { name ,s.stock[i].product.price};
				struct ProductStock shoppinglist = { product, quantity };
				customer.shoppingList[customer.index++] = shoppinglist;
			}

		}
    }
	return customer;
}

/*****************************************************************************************************
 * The below function prints the shop details including Shop cash, 
 * Products and product quantity in stock
*****************************************************************************************************/
void printShop(struct Shop s)
{
    printf("\n---------------------------- SHOP DETAILS ----------------------------\n");
	printf("Shop has $%.2f in cash\n", s.cash);
    printf("----------------------------------------------------------------------\n");
	for (int i = 0; i < s.index; i++)
	{
		printProduct(s.stock[i].product);
		printf("The shop has %d of the above\n", s.stock[i].quantity);
        printf("----------------------------------------------------------------------\n");
	}
}

/*****************************************************************************************************
 * The below function is the live mode where the user can interact with the terminal
 * It will first ask for customer budget
 * Then display a menu with instructions#
 * It will work in the same way as reading in a csv file
*****************************************************************************************************/
struct Shop liveMode(struct Shop s){
    
    int choice = -1;
    char productName[20];
    double budget;
    int products[s.index];
    int status = 0;
    int x =0;
    char b[10];
    printf("\n------------------------------ LIVE MODE -----------------------------\n"); 
    printf("Welcome to the Struct Shop\n");
    printf("Please Enter Budget: ");
    fgets(b, 200, stdin);
    budget = atof(b);
    printf("\n- Enter 'exit' to cancel order\n- Enter 'checkout' to pay\n- Enter product name to add to basket 'e.g Bread, Coke Can'\n\n");
    
    struct Customer c = {"LIVE", budget};
    while(productName != "exit"){
        printf("\rProduct Name: ");
        fflush(stdout);
        fgets(productName,20,stdin);
        productName[strcspn(productName, "\n")] = 0;
        if(strcmp(productName,"exit") == 0){
            printf("\nThank you! Please visit again\n\n");
            printf("----------------------------------------------------------------------\n");
            break;
        }else if(strcmp(productName,"checkout") == 0){
            s = printCustomer(c,s);
            printf("\nThank you! Please visit again\n");
            //printShop(s);
            printf("----------------------------------------------------------------------\n");
            break;
        }else{
        for(x =0; x < s.index; x++){
            if(strcmp(productName,s.stock[x].product.name) == 0){
                printf("\rPlease enter Quantity: ");
                char q[200];
                fgets(q, 200, stdin);
                int quant = atoi(q);
                if(quant>0){
                    struct Product product = {s.stock[x].product.name,s.stock[x].product.price };
	                struct ProductStock shoppinglist = { product, quant };
				    c.shoppingList[c.index++] = shoppinglist;
                    printf("\r**Product added to basket**\n\n",quant);
                    fflush(stdout);
                    status =1;
                }else if (quant == 0){
                    printf("\rQuantity: **Cannot add %d items**\n",quant);
                    status =1;
                }else{
                    printf("\rQuantity: **Invalid Quantity Input**\n",quant);
                    status =1;
                }
			}
            if(strcmp(productName,s.stock[x].product.name) != 0 && x == s.index-1 && status != 1 && strcmp(productName,"")!=0){
                printf("\r**Sorry we dont stock %s**\n\n",productName);  
            }
        }
        }
        status = 0;
    }
    return s;
}

/*****************************************************************************************************
 * Prints the menu giving the user the option to choose which customer it would like to see
*****************************************************************************************************/
void printMenu(){
    printf("------------------------------ MAIN MENU -----------------------------");
    printf("\n\nPlease choose scenario\n1. Valid Checkout\n2. Not Enough stock\n3. Insufficient Funds\n4. Empty Basket\n5. LIVE Mode");
    printf("\n6. Shop Details\n7. Exit\n\n");
}

/*****************************************************************************************************
 * Function that creates the shop and all customers
*****************************************************************************************************/
void mainMenu(struct Shop shop){
    struct Customer customerOne = read_Customer(shop,"../customer1-pass.csv");
    struct Customer customerTwo = read_Customer(shop,"../customer2-fail-tooManyItems.csv");
    struct Customer customerThree = read_Customer(shop,"../customer3-fail-InsufficientFunds.csv");
    struct Customer customerFour = read_Customer(shop,"../customer4-noItemsInBasket.csv");
    printMenu();
    int run =1;
    char c[200];
    while(run ==1){
        printf("\rChoice (1-7): ");
        fgets(c, 200, stdin);
        int choice = atoi(c);
        if (choice == 1){
	        shop = printCustomer(customerOne , shop);
            printMenu();
        }else if(choice == 2){
            shop = printCustomer(customerTwo , shop);
            printMenu();
        }else if(choice == 3){
            shop = printCustomer(customerThree , shop);
            printMenu();
        }else if(choice == 4){
            shop = printCustomer(customerFour , shop);
            printMenu();
        }else if(choice == 5){
            shop = liveMode(shop);
            printMenu();
        }else if(choice == 6){
            printShop(shop);
            printMenu();
        }else if(choice == 7){
            printf("\r\n-------------------------------- EXIT --------------------------------\n");
            break;
        }else{
            printf("***Invalid Choice***\n\n");
        }
    }

}

int main(void) 
{
	struct Shop shop = createAndStockShop();
    printShop(shop);
    mainMenu(shop);

    return 0;
}