package edu.njust.entity;

public class User {
	private String uId;
	private String uName;
	private String uPw;
	public User() {}
	public User(String uId,String uName,String uPw) {
		this.uId=uId;
		this.uName=uName;
		this.uPw=uPw;
	}
	public String getuName() {
		return uName;
	}
	public void setuName(String uName) {
		this.uName = uName;
	}
	public String getuId() {
		return uId;
	}
	public void setuId(String uId) {
		this.uId = uId;
	}
	public String getuPw() {
		return uPw;
	}
	public void setuPw(String uPw) {
		this.uPw = uPw;
	}
}
