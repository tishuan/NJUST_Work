package edu.njust.servlet;

import java.io.IOException;


import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
//import javax.servlet.http.HttpSession;

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

		System.out.println("发送get请求");
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
		 	System.out.println("发送post请求");
	        String username = req.getParameter("name");
	        String password = req.getParameter("psw");
	        Login login=new Login();
	        login.setName(username);
	        login.setPwd(password);

	        LoginService loginService = new LoginService();
	        LoginResult result = loginService.login(username, password);

	        //HttpSession session = req.getSession();  
	        String contextPath = req.getContextPath();

	        if (result.isSuccess()) {
	        	//session.setAttribute("status", "1");// "1" 表示登录成功，可以是更复杂对象
	        	req.getSession().setAttribute("username",username);//姓名
	        	req.getSession().setAttribute("password",password);//密码
		        //resp.sendRedirect("http://localhost:8080/029/jsp/courseMng.jsp");
		        //获取工程上下文路径(工程名字)
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
