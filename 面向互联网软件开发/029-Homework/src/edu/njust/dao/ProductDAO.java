package edu.njust.dao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import edu.njust.entity.Product;

public class ProductDAO {
	private static final String URL = "jdbc:mysql://localhost:3306/029_reservation?characterEncoding=utf8";
    private static final String USER = "root";
    private static final String PASSWORD = "th145690.";
    
    public static Connection getConnection() {
        Connection connection = null;
        try {
            // 加载数据库驱动
            Class.forName("com.mysql.cj.jdbc.Driver");
            
            // 建立数据库连接
            String url = "jdbc:mysql://localhost:3306/029_reservation?characterEncoding=utf8";
            String username = "root";
            String password = "th145690.";
            connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/029_reservation?characterEncoding=utf-8", "root", "th145690.");
            System.out.println("连接成功！");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        
        return connection;
    }
    //获取所有Product信息
    public Map<String, Product> getAllProducts() {
    	Map<String, Product> products = new HashMap<>();
        try (Connection connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/029_reservation?characterEncoding=utf-8", "root", "th145690.")) {
            String sql = "SELECT * FROM product";
            PreparedStatement preparedStatement = connection.prepareStatement(sql);
            ResultSet resultSet = preparedStatement.executeQuery();
            while (resultSet.next()) {
            	String pId = resultSet.getString("pId");
                String pName = resultSet.getString("pName");
                double pPrice=resultSet.getDouble("pPrice");
                int pCount=resultSet.getInt("pCount");
                products.put(pId,new Product(pId, pName,pPrice,pCount));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return products;
    }

    public Product getProductById(String id) {
    	Product product = null;
        String sql = "SELECT * FROM product WHERE pId=?";
        try (Connection connection = getConnection();
             PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setString(1, id);
            ResultSet resultSet = statement.executeQuery();
            if (resultSet.next()) {
            	product = new Product();
            	product.setpId(resultSet.getString("pId"));
            	product.setpName(resultSet.getString("pName"));
            	product.setpPrice(resultSet.getDouble("pPrice"));
            	product.setpCount(resultSet.getInt("pCount"));
                System.out.println(product.getpId()+" "+product.getpName()+" "+product.getpCount());
            }
        } catch (SQLException e) {
        	System.out.println("不成功");
            e.printStackTrace();
        }
        return product;
    }
}
