# Name: Eoin Stankard - G00300390
# MULTI-PARADIGM-PROGRAMMING
# Shop Assignment - Python Procedural Programming

from dataclasses import dataclass, field
from typing import List
import csv
#****************************************************************************************************
# Shop dataclasses - These will be pointed to when creating Shop,Customer 
#                    and productstock
#****************************************************************************************************
@dataclass
class Product:
    name: str
    price: float = 0.0

@dataclass 
class ProductStock:
    product: Product
    quantity: int

@dataclass 
class Shop:
    cash: float = 0.0
    stock: List[ProductStock] = field(default_factory=list)

@dataclass
class Customer:
    name: str = ""
    budget: float = 0.0
    shopping_list: List[ProductStock] = field(default_factory=list)

#****************************************************************************************************
# The function below will open the Stock.csv file and import all the data it contains
# First value read will be the shop cash
# The next values will be the Product, Price and Quantity in Stock
#****************************************************************************************************

def create_and_stock_shop():
    s = Shop()
    with open('../stock.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        first_row = next(csv_reader)
        s.cash = float(first_row[0])
        for row in csv_reader:
            p = Product(row[0], float(row[1]))
            ps = ProductStock(p, float(row[2]))
            s.stock.append(ps)
    return s

#****************************************************************************************************
# The function below will update the shop stock if a customer makes a successful purchase
#
#****************************************************************************************************
def updateStock(s,c):
    for item in c.shopping_list:
        for prod in s.stock:
            if item.product.name == prod.product.name:
                if prod.quantity >= item.quantity:
                    quantityremaining = prod.quantity - item.quantity
                    prod.quantity = quantityremaining

#****************************************************************************************************
# The function below will open the Customer csv file and import all the data it contains
# first line will contain customers name and budget
# The lines following will contain the wanted products and quantity
#****************************************************************************************************
def read_customer(file_path):
    with open(file_path) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        first_row = next(csv_reader)
        c = Customer(first_row[0], float(first_row[1]))
        for row in csv_reader:
            name = row[0]
            quantity = float(row[1])
            p = Product(name)
            ps = ProductStock(p, quantity)
            c.shopping_list.append(ps)
        return c 
        
#****************************************************************************************************
# Prints the products name and price
#
#****************************************************************************************************
def print_product(p):
    print(f'PRODUCT NAME: {p.name} \nPRODUCT PRICE: ${p.price}')

#****************************************************************************************************
# The below function is called and takes the data that is read in from the customer csv
# This fucntion then does error checking to see if the customer can have a successful order
# It checks if the quantity of a product the customer wants is possible
# It will print a message if successful or not
# Its will also check for things like empty basket and insufficient funds
#****************************************************************************************************
def print_customer(c,s):
    print("------------------------------ CUSTOMER ------------------------------")
    print(f'CUSTOMER NAME: {c.name} \nCUSTOMER BUDGET: ${c.budget}')
    print("------------------------------ PRODUCTS ------------------------------")
    total,cost=0.0,0.0
    for item in c.shopping_list:
        for prod in s.stock:
            if item.product.name == prod.product.name:
                print_product(prod.product)
                print(f'{c.name} ORDERS {int(item.quantity)} OF ABOVE PRODUCT')
                if prod.quantity >= item.quantity:
                    print("***Successful Order***")
                    cost = item.quantity * prod.product.price
                    total = total+cost
                elif prod.quantity < item.quantity:
                    print(f"***UnSuccessful Order - Only {int(prod.quantity)} in stock***")
                print("----------------------------------------------------------------------")

    print("----------------------------- TOTAL BILL -----------------------------")
    print(f"The total cost will be ${round(total,2)}")
    change = c.budget-total
    

    if c.budget >= total and total > 0.00:
        s.cash = s.cash + total
        print("----------------------------------------------------------------------")
        print("                    ***TRANSACTION CONFIRMED***\n")
        print(f"The change given is ${change}\nShop Balance is ${round(s.cash,2)}")
        print("----------------------------------------------------------------------\n")
        updateStock(s,c)
    elif total == 0.00:
        print("----------------------------------------------------------------------")
        print("         ***TRANSACTION CANCELLED - No Items in Basket***\n")
        print(f"Shop Balance is ${round(s.cash,2)}")
        print("----------------------------------------------------------------------\n")

    else:
        print("----------------------------------------------------------------------")
        print("         ***TRANSACTION CANCELLED - Insufficient Funds***\n")
        print(f"Shop Balance is ${round(s.cash,2)}")
        print("----------------------------------------------------------------------\n")
    

#****************************************************************************************************
# The below function prints the shop details including Shop cash, 
# Products and product quantity in stock
#****************************************************************************************************       
def print_shop(s):
    print("\n---------------------------- SHOP DETAILS ----------------------------")
    print(f'Shop has ${s.cash} in cash')
    print("----------------------------------------------------------------------")
    for item in s.stock:
        print_product(item.product)
        print(f'The Shop has {int(item.quantity)} of the above')
        print("----------------------------------------------------------------------")


#****************************************************************************************************
# The below function is the live mode where the user can interact with the terminal
# It will first ask for customer budget
# Then display a menu with instructions#
# It will work in the same way as reading in a csv file
#****************************************************************************************************
def liveMode(s):
    print("------------------------------ LIVE MODE -----------------------------") 
    print("Welcome to the Struct Shop")
    try:
        budget = input("Please Enter Budget: ")
        c = Customer("LIVE",float(budget))
    except ValueError:
        budget = 0.0
        c = Customer("LIVE",budget)
    print("\n- Enter 'exit' to cancel order\n- Enter 'checkout' to pay\n- Enter product name to add to basket 'e.g Bread, Coke Can'\n")
    
    productName = ''
    status=0
    while productName != "exit":
        productName = input("Product Name: ")
        if productName == "exit":
            print("\nThank you! Please visit again\n")
            print("----------------------------------------------------------------------")
        elif productName == "checkout":
            print_customer(c,s)
            print("Thank you! Please visit again\n")
            print("----------------------------------------------------------------------")
            productName = 'exit'
        else:
            for prod in s.stock:
                if productName == prod.product.name:
                    quantity = input("Please enter Quantity: ")
                    try:
                        if int(quantity) > 0:
                           print("**Product added to basket**\n")
                           product = Product(prod.product.name, prod.product.price)
                           shoppinglist = ProductStock( product, int(quantity) )
                           c.shopping_list.append(shoppinglist)
                           status =1
                        elif int(quantity) == 0:
                            print(f"Quantity: **Cannot add {quantity} items**\n")
                            status =1
                    except ValueError:
                        print("Quantity: **Invalid Quantity Input**\n")
                        status =1

            if productName not in s.stock and productName != '' and status !=1:
                print(f"**Sorry we dont stock {productName}**\n")  

        status=0

#****************************************************************************************************
# Prints the menu giving the user the option to choose which customer it would like to see
#
#****************************************************************************************************
def printMenu():
    print("\n------------------------------ MAIN MENU -----------------------------\n") 
    print("Please choose scenario\n1. Valid Checkout\n2. Not Enough stock\n3. Insufficient Funds\n4. Empty Basket\n5. LIVE Mode")
    print("6. Shop Details\n7. Exit\n")

#****************************************************************************************************
# Function that creates the shop and all customers
#
#****************************************************************************************************
def mainMenu():
    s = create_and_stock_shop()
    print_shop(s)
    c = read_customer("../customer1-pass.csv")
    c1 = read_customer("../customer2-fail-tooManyItems.csv")
    c2 = read_customer("../customer3-fail-InsufficientFunds.csv")
    c3 = read_customer("../customer4-noItemsInBasket.csv")
    printMenu()
    choice = 0
    while True:
        
        choice = input("Choice (1-7): ")
        if choice == "1":
            print_customer(c,s)
            printMenu()
        elif choice == "2":
            print_customer(c1,s)
            printMenu()
        elif choice == "3":
            print_customer(c2,s)
            printMenu()
        elif choice == "4":
            print_customer(c3,s)
            printMenu()
        elif choice == "5":
            liveMode(s)
            printMenu()
        elif choice == "6":
            print_shop(s)
            printMenu()
        elif choice == "7":
            print("-------------------------------- EXIT --------------------------------\n")
            break
        else:
            print("***Invalid Choice***\n")


mainMenu()