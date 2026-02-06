package edu.njust.dao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

import edu.njust.entity.Course;

public class CourseDAO {
	private static final String URL = "jdbc:mysql://localhost:3306/029_jdbc";
    private static final String USER = "root";
    private static final String PASSWORD = "th145690.";
    
    public static Connection getConnection() {
        Connection connection = null;
        try {
            // 加载数据库驱动
            Class.forName("com.mysql.cj.jdbc.Driver");
            
            // 建立数据库连接
            String url = "jdbc:mysql://localhost:3306/029_jdbc";
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
    //获取所有课程信息
    public Map<String, Course> getAllCourses() {
    	Map<String, Course> courses = new HashMap<>();
        try (Connection connection = DriverManager.getConnection(URL, USER, PASSWORD)) {
            String sql = "SELECT * FROM course";
            PreparedStatement preparedStatement = connection.prepareStatement(sql);
            ResultSet resultSet = preparedStatement.executeQuery();
            while (resultSet.next()) {
                String id = resultSet.getString("cId");
                String name = resultSet.getString("cName");
                String num = resultSet.getString("cNum");
                String type = resultSet.getString("cType");
                courses.put(id,new Course(id,name,num,type));
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return courses;
    }
    // 向数据库表course中插入课程信息
    public void insertCourse(Course course) {
        String sql = "INSERT INTO course (cId, cName, cNum,cType) VALUES (?, ?, ?,?)";
        try (Connection connection = getConnection();
             PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setString(1, course.getcId());
            statement.setString(2, course.getcName());
            statement.setString(3, course.getcNum());
            statement.setString(4, course.getcType());
            statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    // 更新数据库表course中的课程信息
    public void updateCourse(Course course) {
        String sql = "UPDATE course SET cName=?, cNum=?,cType=? WHERE cId=?";
        try (Connection connection = getConnection();
            PreparedStatement statement = connection.prepareStatement(sql)) {
        	 statement.setString(1, course.getcId());
             statement.setString(2, course.getcName());
             statement.setString(3, course.getcNum());
             statement.setString(4, course.getcType());
            statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    // 从数据库表course中删除指定ID的课程信息
    public void deleteCourse(String id) {
        String sql = "DELETE FROM course WHERE cId=?";
        try (Connection connection = getConnection();
             PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setString(1, id);
            statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    // 从数据库表course中根据ID获取课程信息
    public Course getCourseById(String id) {
        Course course = null;
        String sql = "SELECT * FROM course WHERE cId=?";
        try (Connection connection = getConnection();
             PreparedStatement statement = connection.prepareStatement(sql)) {
            statement.setString(1, id);
            ResultSet resultSet = statement.executeQuery();
            if (resultSet.next()) {
                course = new Course();
                course.setcId(resultSet.getString("cId"));
                course.setcName(resultSet.getString("cName"));
                course.setcNum(resultSet.getString("cNum"));
                course.setcType(resultSet.getString("cType"));
                System.out.println(course.getcId()+" "+course.getcName()+" "+course.getcNum());
            }
        } catch (SQLException e) {
        	System.out.println("不成功");
            e.printStackTrace();
        }
        return course;
    }
}
