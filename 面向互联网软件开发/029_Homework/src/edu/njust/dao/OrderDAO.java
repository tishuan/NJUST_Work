package edu.njust.dao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.HashMap;
import java.util.Map;

import edu.njust.entity.Reservation;


public class OrderDAO {
    private static final String URL = "jdbc:mysql://localhost:3306/029_reservation";
    private static final String USER = "root";
    private static final String PASSWORD = "th145690.";
    
    public static Connection getConnection() {
        Connection connection = null;
        try {
            // 加载数据库驱动
            Class.forName("com.mysql.cj.jdbc.Driver");
            
            // 建立数据库连接
            String url = "jdbc:mysql://localhost:3306/029_reservation";
            String username = "root";
            String password = "th145690.";
            connection = DriverManager.getConnection(url, username, password);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        
        return connection;
    }

    public Map<String, Reservation> getAllOrders() {
    	Map<String, Reservation> orders = new HashMap<>();
        try (Connection connection = DriverManager.getConnection(URL, USER, PASSWORD)) {
            String sql = "SELECT * FROM reservation";
            PreparedStatement preparedStatement = connection.prepareStatement(sql);
            ResultSet resultSet = preparedStatement.executeQuery();
            while (resultSet.next()) {
            	String oId = resultSet.getString("oId");
                String uId = resultSet.getString("uId");
                Timestamp oTime = resultSet.getTimestamp("oTime");
                double oTotal=resultSet.getDouble("oTotal");
                String oStatus = resultSet.getString("oStatus");
                orders.put(oId,new Reservation(oId, uId, oTime,oTotal,oStatus));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return orders;
    }
    public Reservation getOrderById(String id) {
    	Reservation order = null;
        String sql = "SELECT * FROM reservation WHERE oId=?";
        try (Connection connection = getConnection();
             PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setString(1, id);
            ResultSet resultSet = statement.executeQuery();
            if (resultSet.next()) {
            	order = new Reservation();
            	order.setoId(resultSet.getString("oId"));
            	order.setuId(resultSet.getString("uId"));
            	//
            	Timestamp oTime=resultSet.getTimestamp("oTime");
            	DateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        		//将当前系统时间转换为不显示毫秒情况，保存为string类型在dateNow中
        		String date= df.format(oTime);
            	order.setoTime(date);
            	order.setoTotal(resultSet.getDouble("oTotal"));
            	order.setoStatus(resultSet.getString("oStatus"));
                System.out.println(order.getoId()+" "+order.getuId()+" "+order.getoTime());
            }
        } catch (SQLException e) {
        	System.out.println("不成功");
            e.printStackTrace();
        }
        return order;
    }
}
