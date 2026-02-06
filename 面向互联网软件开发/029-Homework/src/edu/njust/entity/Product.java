package edu.njust.entity;

public class Product {
	private String pId;
	private String pName;
	private double pPrice;
	private int pCount;
	public Product() {
		
	}
	public Product(String pId,String pName,double pPrice,int pCount) {
		this.setpId(pId);
		this.setpName(pName);
		this.setpPrice(pPrice);
		this.setpCount(pCount);
	}
	public String getpId() {
		return pId;
	}
	public void setpId(String pId) {
		this.pId = pId;
	}
	public String getpName() {
		return pName;
	}
	public void setpName(String pName) {
		this.pName = pName;
	}
	public double getpPrice() {
		return pPrice;
	}
	public void setpPrice(double pPrice) {
		this.pPrice = pPrice;
	}
	public int getpCount() {
		return pCount;
	}
	public void setpCount(int pCount) {
		this.pCount = pCount;
	}
}
