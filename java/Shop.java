/*
Name: Eoin Stankard - G00300390
MULTI-PARADIGM-PROGRAMMING
Shop Assignment - JAVA OOP
*/

import java.io.*;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Shop {

	private double cash;
	private ArrayList<ProductStock> stock;
	/*****************************************************************************************************
 	 * The function below will open the Stock.csv file and import all the data it contains
	 * First value read will be the shop cash
 	 * The next values will be the Product, Price and Quantity in Stock
	*****************************************************************************************************/
	public Shop(String fileName) {
		stock = new ArrayList<>();
		List<String> lines = Collections.emptyList();
		try {
			lines = Files.readAllLines(Paths.get(fileName), StandardCharsets.UTF_8);
			String csh = lines.get(0);
			if(csh.contains(",")){
				csh = csh.replace(",", " ");
			}
			cash = Double.parseDouble(csh);
			// i am removing at index 0 as it is the only one treated differently
			lines.remove(0);
			for (String line : lines) {
				String[] arr = line.split(",");
				String name = arr[0];
				double price = Double.parseDouble(arr[1]);
				int quantity = Integer.parseInt(arr[2].trim());
				Product p = new Product(name, price);
				ProductStock s = new ProductStock(p, quantity);
				stock.add(s);
			}
		}
		catch (IOException e) {
			System.out.println("The program could not find the CSV file with the shop information in it");
			e.printStackTrace();
			System.exit(-1);
		}
	}
	
	/*****************************************************************************************************
	 * This function looks for a product within the productstock
	*****************************************************************************************************/
	private ProductStock find(String name){
		int x = 0;
		for(ProductStock ps : stock){
			if (name.equals(ps.getName())){
				return ps;
			}
			else if (!name.equals(ps.getName()) && x == stock.size()-1 ){
				System.out.println("**Sorry we dont stock "+name+"**\n");
			}
			x++;
		}
		return null;
	}

	/*****************************************************************************************************
 	 * The below function is the live mode where the user can interact with the terminal
 	 * It will first ask for customer budget
	 * Then display a menu with instructions#
	 * It will work in the same way as reading in a csv file
	****************************************************************************************************/
	public void liveMode(Shop shop){
		System.out.println("------------------------------ LIVE MODE -----------------------------");
		System.out.println("Welcome to the Struct Shop");
		Console console = System.console();
		String b = console.readLine("Please Enter Budget: ");
		double budget=0.0;
		try {
			budget = Double.parseDouble(b);
		}catch (Exception e) {}
		
		Customer live = new Customer("LIVE",budget);
		System.out.println("\n- Enter 'exit' to cancel order\n- Enter 'checkout' to pay\n- Enter product name to add to basket 'e.g Bread, Coke Can'\n"); 
		String productName="";
		boolean loop = true;
		
		while (loop){
			productName = console.readLine("Product Name: ");
			if(productName.equals("exit")){
				System.out.println("\nThank you! Please visit again");
				System.out.print("----------------------------------------------------------------------\n");
				loop = false;
			}else if (productName.equals("checkout")){
				shop.processOrder(live,shop);
				System.out.println("\nThank you! Please visit again");
				System.out.print("----------------------------------------------------------------------\n");
				loop = false;
			}else if (!productName.equals("")){
				try{
					ProductStock shopPS = find(productName);
					if(productName.equals(shopPS.getProduct().getName())){ 
						String quant =console.readLine("Please enter Quantity: ");
						try{
							int quantity = Integer.parseInt(quant);
							if (quantity>0){
								System.out.println("**Product added to basket**\n");
								Product p = new Product(productName,shopPS.getProduct().getPrice());
								ProductStock s = new ProductStock(p, quantity);
								live.setShoppingList(s);
							}else if(quantity == 0){
								System.out.println("**Quantity: **Cannot add 0 items**\n");
							}
						}catch(Exception f){System.out.println("Quantity: **Invalid Quantity Input**\n");}
					}
				}catch (Exception e){}
			}
		}
	}

	/*****************************************************************************************************
	 * The below function is called and takes the data that is read in from the customer csv
	 * This fucntion then does error checking to see if the customer can have a successful order
	 * It checks if the quantity of a product the customer wants is possible
	 * It will print a message if successful or not
	 * Its will also check for things like empty basket and insufficient funds
	*****************************************************************************************************/
	public void processOrder(Customer c, Shop s){
		System.out.println(c);  
		System.out.println("------------------------------ PRODUCTS ------------------------------");
		double costToCustomer = 0;
		ProductStock shopPS;
		// look through the customer order
		for(ProductStock ps : c.getShoppingList()){
			shopPS = find(ps.getName());
			// retreive the unit price
			double unitPrice = shopPS.getUnitPrice();
			// set the price on the product held by customer
			ps.getProduct().setPrice(unitPrice);
			// this way we can use the ps method to calc cost;
			System.out.println("PRODUCT NAME: "+ps.getName() +"\nPRODUCT PRICE: \u0024"+unitPrice+"\n"+ c.getName() +" ORDERS " + ps.getQuantity() + " OF ABOVE PRODUCT\n");
			if (shopPS.getQuantity() >= ps.getQuantity()){
				System.out.println("***Successful Order***");
				costToCustomer += ps.getCost();
			}else if (ps.getQuantity() > shopPS.getQuantity()){
				ps.setQuantity(0);
				System.out.println("**UnSuccessful Order - Only "+shopPS.getQuantity() +" in stock***");
			}
			System.out.println("----------------------------------------------------------------------");
		}
		System.out.println("----------------------------- TOTAL BILL -----------------------------");
		System.out.printf("The total cost will be \u0024%.2f\n",costToCustomer);
		double change  = c.getBudget() - costToCustomer;

		if (c.getBudget() >= costToCustomer && costToCustomer >0.00){
			cash = cash + costToCustomer;
			System.out.println("----------------------------------------------------------------------");
			System.out.println("                    ***TRANSACTION CONFIRMED***");
			System.out.printf("The change given is \u0024"+change+"\nShop Balance is \u0024%.2f\n",cash);
			System.out.println("----------------------------------------------------------------------");
			for(ProductStock ps : c.getShoppingList()){
				shopPS = find(ps.getName());
				shopPS.setQuantity(shopPS.getQuantity()- ps.getQuantity());
			}
		}else if(costToCustomer == 0.00){
			System.out.println("----------------------------------------------------------------------");
			System.out.println("         ***TRANSACTION CANCELLED - No Items in Basket***");
			System.out.printf("Shop Balance is \u0024%.2f\n",cash);
			System.out.println("----------------------------------------------------------------------");
		}else{
			System.out.println("----------------------------------------------------------------------");
			System.out.println("         ***TRANSACTION CANCELLED - Insufficient Funds***");
			System.out.printf("Shop Balance is \u0024%.2f\n",cash);
			System.out.println("----------------------------------------------------------------------");
		}
	}
	
	/*****************************************************************************************************
	 * Prints the menu giving the user the option to choose which customer it would like to see
	*****************************************************************************************************/
	public static void printMenu(){
		System.out.println("\n------------------------------ MAIN MENU -----------------------------");
    	System.out.println("\nPlease choose scenario\n1. Valid Checkout\n2. Not Enough stock\n3. Insufficient Funds\n4. Empty Basket\n5. LIVE Mode");
    	System.out.println("6. Shop Details\n7. Exit\n");
	}

	@Override
	public String toString() {
		System.out.println("\n---------------------------- SHOP DETAILS ----------------------------");
		System.out.println("Shop has \u0024"+cash+" in cash");
		System.out.print("----------------------------------------------------------------------\n");
		for(ProductStock ps : stock){
			System.out.println(ps);
        	System.out.print("----------------------------------------------------------------------\n");
		}
		return "";
	}
	
	/*****************************************************************************************************
	 * Function that creates the shop and all customers
	*****************************************************************************************************/
	public static void main(String[] args) throws Exception {
		//createCSV();
		Shop shop = new Shop("../stock.csv");
		System.out.println(shop);
		Customer john = new Customer("../customer1-pass.csv");
		Customer mary = new Customer("../customer2-fail-tooManyItems.csv");
		Customer tom = new Customer("../customer3-fail-InsufficientFunds.csv");
		Customer pat = new Customer("../customer4-noItemsInBasket.csv");
		printMenu();
		Console console = System.console();
		while(true){
			String choice = console.readLine("Choice (1-7): ");
			if(choice.equals("1")){
				shop.processOrder(john,shop);
				printMenu();
			}else if(choice.equals("2")){
				shop.processOrder(mary,shop);
				printMenu();
			}else if(choice.equals("3")){
				shop.processOrder(tom,shop);
				printMenu();
			}else if(choice.equals("4")){
				shop.processOrder(pat,shop);
				printMenu();
			}else if(choice.equals("5")){
				shop.liveMode(shop);
				printMenu();
			}else if(choice.equals("6")){
				System.out.println(shop);
				printMenu();
			}else if(choice.equals("7")){
				System.out.println("-------------------------------- EXIT --------------------------------\n");
				break;
			}else{
				System.out.println("***Invalid Choice***\n");
			}
		}
	}
}
