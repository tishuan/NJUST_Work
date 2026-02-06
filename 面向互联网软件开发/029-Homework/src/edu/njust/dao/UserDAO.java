package edu.njust.dao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import edu.njust.entity.User;


public class UserDAO {

    private static final String URL = "jdbc:mysql://localhost:3306/029_reservation?characterEncoding=utf8";
    private static final String USER = "root";
    private static final String PASSWORD = "th145690.";

    public static Connection getConnection() {
        Connection connection = null;
        try {
            // 加载数据库驱动
            Class.forName("com.mysql.cj.jdbc.Driver");
            
            // 建立数据库连接
            String url = "jdbc:mysql://localhost:3306/029_reservation?characterEncoding=utf-8";
            String username = "root";
            String password = "th145690.";
            connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/029_reservation?characterEncoding=utf-8", "root", "th145690.");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        
        return connection;
    }
    public Map<String, User> getAllUsers() {
    	Map<String, User> users = new HashMap<>();
        try (Connection connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/029_reservation?characterEncoding=utf-8", "root", "th145690.")) {
            String sql = "SELECT * FROM user";
            PreparedStatement preparedStatement = connection.prepareStatement(sql);
            ResultSet resultSet = preparedStatement.executeQuery();
            while (resultSet.next()) {
                String uId = resultSet.getString("uId");
                String uName = resultSet.getString("uName");
                String uPw = resultSet.getString("uPw");
                users.put(uId,new User(uId, uName, uPw));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return users;
    }
    public User getUserById(String id) {
    	User user = null;
        String sql = "SELECT * FROM user WHERE uId=?";
        try (Connection connection = getConnection();
             PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setString(1, id);
            ResultSet resultSet = statement.executeQuery();
            if (resultSet.next()) {
            	user = new User();
            	user.setuId(resultSet.getString("uId"));
            	user.setuName(resultSet.getString("uName"));
            	user.setuPw(resultSet.getString("uPw"));
                System.out.println("用户: "+user.getuId()+" "+user.getuName()+" "+user.getuPw());
            }
        } catch (SQLException e) {
        	System.out.println("不成功");
            e.printStackTrace();
        }
        return user;
    }
}
