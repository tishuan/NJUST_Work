package edu.njust.dao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import edu.njust.entity.*;
public class LoginDAO {
    private static Map<String, Login> users = new HashMap<>();

    static {
    	//用户名 密码 学号
        users.put("user1", new Login("user1", "111a", "111"));
        users.put("user2", new Login("user2", "112b", "112"));
        users.put("user3", new Login("user3", "113c", "113"));
    }
    
    //
    private static final String URL = "jdbc:mysql://localhost:3306/029_jdbc";
    private static final String USER = "root";
    private static final String PASSWORD = "th145690.";

    public Map<String, Login> getAllUsers() {
    	Map<String, Login> users = new HashMap<>();
        try (Connection connection = DriverManager.getConnection(URL, USER, PASSWORD)) {
            String sql = "SELECT * FROM user";
            PreparedStatement preparedStatement = connection.prepareStatement(sql);
            ResultSet resultSet = preparedStatement.executeQuery();
            while (resultSet.next()) {
                String id = resultSet.getString("uId");
                String username = resultSet.getString("uName");
                String password = resultSet.getString("uPw");
                users.put(username,new Login(username, password, id));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return users;
    }

    public boolean checkLogin(String username, String password) {
        try (Connection connection = DriverManager.getConnection(URL, USER, PASSWORD)) {
            String sql = "SELECT * FROM user WHERE username = ? AND password = ?";
            PreparedStatement preparedStatement = connection.prepareStatement(sql);
            preparedStatement.setString(1, username);
            preparedStatement.setString(2, password);
            ResultSet resultSet = preparedStatement.executeQuery();
            return resultSet.next();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return false;
    }
    public String check() {
    	try {
    		Class.forName("com.mysql.cj.jdbc.Driver");
			return "success!";
			}
			catch(Exception e){
				e.printStackTrace();//捕获异常打印异常	
			return "false!";
			}
    }
/*    public boolean checkUser(String username, String password) {
        Login user = users.get(username);
        if (user != null && user.getPwd().equals(password)) {
            return true;
        }
        return false;
    }
*/   
   public LoginResult checkUser(String username, String password) {
	    LoginDAO logindao = new LoginDAO();
	    System.out.println(logindao.check());
	    users=logindao.getAllUsers();
	    Login user = users.get(username);
	    LoginResult loginresult=new LoginResult();
	    if (user != null && user.getPwd().equals(password)) {
	    	loginresult.setSuccess(true);
	    	loginresult.setMessage("登录成功");
	        return loginresult;
	    }
//	    loginresult.setSuccess(false);
//	    loginresult.setErrorCode("用户名密码错误");
	    if(user==null) {//用户不存在
	    	loginresult.setErrorCode("1");
	    	loginresult.setSuccess(false);
	    	loginresult.setMessage("用户不存在");
	    	return loginresult;
	    }
	    if(user!=null && !user.getPwd().equals(password)) {//用户存在但密码错误
	    	loginresult.setErrorCode("2");
	    	loginresult.setSuccess(false);
	    	loginresult.setMessage("密码错误");
	    	return loginresult;
	    }
	    return loginresult;
	}
    public static String findUserByUsername(String username) {
        // 从Map中获取用户记录
        Login user = users.get(username);
        // 如果找到了匹配的用户记录，则返回该用户的AppUser对象；否则返回null
        return user.getId();
    }
}