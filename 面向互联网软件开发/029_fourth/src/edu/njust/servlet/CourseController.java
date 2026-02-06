package edu.njust.servlet;

import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
//import javax.swing.JOptionPane;

import edu.njust.entity.Course;
import edu.njust.entity.CourseResult;
import edu.njust.service.CourseService;

public class CourseController extends HttpServlet {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	/**
		 * Constructor of the object.
		 */
	public CourseController() {
		super();
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

		response.setContentType("text/html");
		PrintWriter out = response.getWriter();
		out.println("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
		out.println("<HTML>");
		out.println("  <HEAD><TITLE>A Servlet</TITLE></HEAD>");
		out.println("  <BODY>");
		out.print("    This is ");
		out.print(this.getClass());
		out.println(", using the GET method");
		out.println("  </BODY>");
		out.println("</HTML>");
		out.flush();
		out.close();
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
	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		 System.out.println("course发送post");
         request.setCharacterEncoding("UTF-8");
         response.setContentType("text/html;charset=UTF-8");	
		 Course course = new Course();
         //设置course对象的属性值，例如：
	 	 //course.setcId("1");
         course.setcName(request.getParameter("courseName"));
         //course.setcNum(Integer.parseInt(request.getParameter("num")));
         course.setcNum(request.getParameter("courseCapacity"));
         course.setcType(request.getParameter("courseType"));

        // 调用CourseService类的相应方法处理业务逻辑
        CourseService courseService = new CourseService();
        CourseResult result = courseService.addCourse(course);//添加结果
     
        String contextPath = request.getContextPath();
        
        String action = request.getParameter("action");
        
        // 根据处理结果，将用户重定向到相应的页面（courseMng.jsp或courseFailure.jsp）
        if ("add".equals(action)) {
        	System.out.println("enter add");
		    if (result.isSuccess()) {
		    	//JOptionPane.showMessageDialog(null,result.getMessage(), "提示", JOptionPane.INFORMATION_MESSAGE);
		        response.sendRedirect(contextPath +"/jsp/courseMng.jsp");
		    } else {
		        request.getSession().setAttribute("errorCode", result.getErrorCode());
		        request.getSession().setAttribute("errorMsg", result.getMessage());
		        //request.getRequestDispatcher("/jsp/courseFailure.jsp").forward(request, response);
		        response.sendRedirect(contextPath +"/jsp/courseFailure.jsp");
		    }		
        }
        else if("del".equals(action)) {
        	// 获取选中的课程ID列表
            String[] Ids = request.getParameterValues("Ids");
            CourseResult delresult=new CourseResult();
            delresult.setSuccess(true);
            if(Ids==null) {
            	delresult.setErrorCode("1");
            	delresult.setSuccess(false);
            	delresult.setMessage("未选中课程");
            }
            // 根据操作结果跳转到相应的页面
            
            else {
            	//先判断是否全部都能执行删除操作
            	for(String id:Ids) {
                	CourseResult temp = courseService.deleteCourse(id);//删除结果
 	           	    if(!temp.isSuccess()) {
 	           	    	delresult=temp;
 	           	    }
	            }
	        }       
            
            if (delresult.isSuccess()) {
         		//JOptionPane.showMessageDialog(null,delresult.getMessage(), "提示", JOptionPane.INFORMATION_MESSAGE);
 		        response.sendRedirect(contextPath +"/jsp/courseMng.jsp");
 		    } else {
 		        request.getSession().setAttribute("errorCode", delresult.getErrorCode());
 		        request.getSession().setAttribute("errorMsg", delresult.getMessage());
 		        //request.getRequestDispatcher("/jsp/courseFailure.jsp").forward(request, response);
 		       response.sendRedirect(contextPath +"/jsp/courseFailure.jsp");
 		    }	
            
       }

   }
}       

