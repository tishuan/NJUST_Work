package edu.njust.servlet;

import java.io.IOException;
import java.util.ArrayList;
//import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
//import javax.servlet.http.HttpSession;

import edu.njust.dao.CourseDAO;
//import edu.njust.dao.LoginDAO;
import edu.njust.entity.Course;
import edu.njust.entity.Login;
import edu.njust.entity.LoginResult;
import edu.njust.service.LoginService;


public class LoginController extends HttpServlet {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	/**
		 * Constructor of the object.
		 */
	public LoginController() {
		super();
	}

	/**
		 * Destruction of the servlet. <br>
		 */
	public void destroy() {
		super.destroy(); // Just puts "destroy" string in log
		// Put your code here
	}

	/**
		 * The doGet method of the servlet. <br>
		 *
		 * This method is called when a form has its tag value method equals to get.
		 * 
		 * @param request the request send by the client to the server
		 * @param response the response send by the server to the client
		 * @throws ServletException if an error occurred
		 * @throws IOException if an error occurred
		 */
	public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

		System.out.println("get");
	}

	/**
		 * The doPost method of the servlet. <br>
		 *
		 * This method is called when a form has its tag value method equals to post.
		 * 
		 * @param request the request send by the client to the server
		 * @param response the response send by the server to the client
		 * @throws ServletException if an error occurred
		 * @throws IOException if an error occurred
		 */

	 protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
		 	System.out.println("发送post");
	        req.setCharacterEncoding("UTF-8");
	        resp.setContentType("text/html;charset=UTF-8");
	        String username = req.getParameter("name");
	        String password = req.getParameter("psw");
	        System.out.println(username);
	        System.out.println(password);
	        Login login=new Login();
	        login.setName(username);
	        login.setPwd(password);

	        LoginService loginService = new LoginService();
	        LoginResult result = loginService.login(username, password);

	        //HttpSession session = req.getSession();  
	        String contextPath = req.getContextPath();
	        //获取课程信息
	        CourseDAO coursedao=new CourseDAO();
	        Map<String,Course>courses=coursedao.getAllCourses();
	        List<Course> courseList = new ArrayList<>(courses.values());
	        if(courseList.isEmpty()) {
	        	System.out.println("为空");
	        }
	        if (result.isSuccess()) {
	        	//session.setAttribute("status", "1");
	        	req.getSession().setAttribute("username",username);
	        	req.getSession().setAttribute("password",password);
		        //resp.sendRedirect("http://localhost:8080/029/jsp/courseMng.jsp");	        	
	        	//
	        	// 将课程信息存储在request对象中
	            req.getSession().setAttribute("courseList", courseList);
		        resp.sendRedirect(contextPath + "/jsp/courseMng.jsp");
		  
		        //resp.sendRedirect("/029/jsp/courseMng.jsp?username="+username+"&password="+password);
	            //req.getRequestDispatcher("courseMng.jsp").forward(req, resp);
	        } else {
	        	req.setAttribute("error_code", result.getErrorCode());
	            req.setAttribute("errorMsg", result.getMessage());
	            req.getRequestDispatcher("/jsp/loginFailure.jsp").forward(req, resp);
	        }
	    }

	/**
		 * Initialization of the servlet. <br>
		 *
		 * @throws ServletException if an error occurs
		 */
	public void init() throws ServletException {
		// Put your code here
	}

}
