package edu.njust.entity;

public class Login {
	private String name;
	private String pwd;
	private String school;
	private String major;
	private String id;
	
	public Login() {}
	public Login(String name,String pwd) {
		super();
		this.name=name;
		this.pwd=pwd;
	}
	public Login(String name,String pwd,String id) {
		super();
		this.name=name;
		this.pwd=pwd;
		this.setId(id);
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name=name;
	}
	public String getPwd() {
		return pwd;
	}
	public void setPwd(String pwd) {
		this.pwd=pwd;
	}
	public String getSchool() {
		return school;
	}
	public void setSchool(String school) {
		this.school=school;
	}
	public String getMajor() {
		return major;
	}
	public void setMajor(String major) {
		this.major = major;
	}
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
}
