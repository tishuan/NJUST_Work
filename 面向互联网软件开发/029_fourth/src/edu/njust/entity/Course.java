package edu.njust.entity;

public class Course {
	private String cId;
	private String cName;
	private String cNum;
	private String cType;
	public Course() {}
	public Course(String cId,String cName,String cNum,String cType) {
		this.cId=cId;
		this.cName=cName;
		this.cNum=cNum;
		this.cType=cType;
	}
	public String getcId() {
		return cId;
	}
	public void setcId(String cId) {
		this.cId = cId;
	}
	public String getcName() {
		return cName;
	}
	public void setcName(String cName) {
		this.cName = cName;
	}
	public String getcNum() {
		return cNum;
	}
	public void setcNum(String cNum) {
		this.cNum = cNum;
	}
	public String getcType() {
		return cType;
	}
	public void setcType(String cType) {
		this.cType = cType;
	}
}
