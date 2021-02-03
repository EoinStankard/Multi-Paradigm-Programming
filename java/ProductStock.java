public  class ProductStock {
	private Product product;
	int quantity;
	
	public ProductStock(Product product, int quantity) {
		this.product = product;
		this.quantity = quantity;
	}

	public Product getProduct() {
		return product;
	}
	
	public double getUnitPrice() {
		return product.getPrice();
	}
	
	public String getName(){
		return product.getName();
	}
	
	public int getQuantity() {
		return quantity;
	}
	
	public int setQuantity(int quant) {
		quantity = quant;
		return quantity;
	}
	public double getCost(){
		return product.getPrice() * quantity;
	}

	@Override
	public String toString() {
		String str = "PRODUCT NAME: "+product.getName()+"\nPRODUCT PRICE: \u0024"+product.getPrice();
		str = str +"\nThe Shop has "+quantity+" of the above";
		return str;
	}

}
