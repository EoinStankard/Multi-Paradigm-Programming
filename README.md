# Multi-Paradigm-Programming
## INSTRUCTIONS TO RUN APPLICATION:

JAVA:
1. open "..\G00300390 - Eoin Stankard - Assignment 1\java" on command line
2. Type command "java Shop" to run

C:
1. open "..\G00300390 - Eoin Stankard - Assignment 1\c" on command line
2. Type Command "gcc Shop.c" and press enter
2. Type command "a.exe" to run

PYTHON:
1. open "..\G00300390 - Eoin Stankard - Assignment 1\python" on command line
2. Type command "python shop.py" to run


## WHEN APPLICATION IS RUNNING:
All three applications will run in the same way giving the same Menus
When you run the application you will be asked to choose a scenario,There are 7 scenarios

	1. Valid Checkout - This is where the customer can purchase everything in his list
	2. Not enough stock - Customer will not be able to buy a certain item on their list as there are not enough in stock
	3. Insufficient Funds - Customer does not have enough money
	4. Empty Basked - Customer has no items in basket
	5. LIVE MODE - You interact with termimal, You will be asked for a budget and then asked to add items to basket
		- Enter 'exit' to cancel order
		- Enter 'checkout' to pay
		- Enter product name to add to basket 'e.g Bread, Coke Can'
	6. Shop Details - Prints shop cash, Products and quantity
	7. Exit - Exits Application

## STOCK CSV
1. You can add more items if necessary
2. Go to "..\G00300390 - Eoin Stankard - Assignment 1\stock.csv"
3. open with notepad
	-First row is shop cash : "1000.2,,"- If you want to update just change the number
	-All other rows are products
	-eg "Coke Can,1.1,100" in the format "Product name,price,quantity"
	- If you want to add product just add to bottom of list
	-eg:
		1000.2,,
		Coke Can,1.1,100
		New Product,99,999
