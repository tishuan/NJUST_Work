package edu.njust.service;

import java.util.Map;
import edu.njust.dao.UserDAO;

import edu.njust.entity.User;

public class UserService {
	private UserDAO userDAO=new UserDAO();
	public Map<String, User> getAllUser() {
        return userDAO.getAllUsers();
    }
	public User getUserById(String id) {
        return userDAO.getUserById(id);
    }
}
