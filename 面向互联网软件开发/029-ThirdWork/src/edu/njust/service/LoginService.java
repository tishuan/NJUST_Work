package edu.njust.service;

import edu.njust.dao.*;
import edu.njust.entity.LoginResult;
public class LoginService {
    public LoginResult login(String username, String password) {
        LoginDAO loginDAO = new LoginDAO();
        return loginDAO.checkUser(username, password);
    }
}
