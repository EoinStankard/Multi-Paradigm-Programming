import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Customer {
	
	private String name;
	private double budget;
	private ArrayList<ProductStock> shoppingList;
	/*****************************************************************************************************
	 * The function below will open the Customer csv file and import all the data it contains
	 * first line will contain customers name and budget
	 * The lines following will contain the wanted products and quantity
	*****************************************************************************************************/
	public Customer(String fileName) {
		shoppingList = new ArrayList<>();
		List<String> lines = Collections.emptyList();
		try {
			lines = Files.readAllLines(Paths.get(fileName), StandardCharsets.UTF_8);
			String[] firstLine = lines.get(0).split(",");
			name = firstLine[0];
			budget = Double.parseDouble(firstLine[1]);
			// i am removing at index 0 as it is the only one treated differently
			lines.remove(0);
			for (String line : lines) {
				String[] arr = line.split(",");
				String name = arr[0];
				int quantity = Integer.parseInt(arr[1].trim());
				Product p = new Product(name,0);
				//DiscountedProductStock s = new DiscountedProductStock(p, quantity, 2, 0.1);
				ProductStock s = new ProductStock(p, quantity);
				shoppingList.add(s);
			}
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}
	public Customer(String live,double liveBudget){
		name = live;
		budget = liveBudget;
		shoppingList = new ArrayList<>();

	}
	public String getName() {
		return name;
	}


	public double getBudget() {
		return budget;
	}

	public void setShoppingList(ProductStock s) {
		shoppingList.add(s);
	}

	public ArrayList<ProductStock> getShoppingList() {
		return shoppingList;
	}
	

	@Override
	public String toString() {
		String ret ="------------------------------ CUSTOMER ------------------------------\n"+ "CUSTOMER NAME: "+name+"\nCUSTOMER BUDGET: \u0024"+budget+"\n";
		return ret;
	}
}
