package edu.njust.entity;

import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.SimpleDateFormat;


public class Reservation {
	private String oId;
	private String uId;
	private String oTime;
	private double oTotal;
	private String oStatus;
	public Reservation(){}
	public Reservation(String oId,String uId,Timestamp oTime,double oTotal,String oStatus){
		this.setoId(oId);
		this.setuId(uId);
		this.setoTotal(oTotal);
		this.setoStatus(oStatus);		
		//定义格式，不显示毫秒
		DateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		//将当前系统时间转换为不显示毫秒情况，保存为string类型在dateNow中
		String date= df.format(oTime);
		this.setoTime(date);
	}
	public String getoId() {
		return oId;
	}
	public void setoId(String oId) {
		this.oId = oId;
	}
	public String getuId() {
		return uId;
	}
	public void setuId(String uId) {
		this.uId = uId;
	}
	public String getoTime() {
		return oTime;
	}
	public void setoTime(String oTime) {
		this.oTime = oTime;
	}
	public double getoTotal() {
		return oTotal;
	}
	public void setoTotal(double oTotal) {
		this.oTotal = oTotal;
	}
	public String getoStatus() {
		return oStatus;
	}
	public void setoStatus(String oStatus) {
		this.oStatus = oStatus;
	}
}
