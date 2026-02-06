package edu.njust.servlet;

import java.io.IOException;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import edu.njust.entity.Reservation;
import edu.njust.entity.User;
import edu.njust.service.OrderService;
import edu.njust.service.UserService;

/**
 * Servlet implementation class UserController
 */
//@WebServlet("/UserController")
public class UserController extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UserController() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		//response.getWriter().append("Served at: ").append(request.getContextPath());
		this.doPost(request, response);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		request.setCharacterEncoding("UTF-8");
        response.setContentType("text/html;charset=UTF-8");
		String contextPath = request.getContextPath();
		//获取所有User-user
        UserService userService=new UserService();
        Map<String,User>user=userService.getAllUser();
        List<User> UserList = new ArrayList<>(user.values());
        if(UserList==null) {System.out.println("为空");}
        System.out.println("Bu为空");
        for(User us:UserList) {
        	System.out.println("user"+us.getuId());
        }
       //获取所有Order-orders
        OrderService orderService=new OrderService();
        Map<String,Reservation>orders=orderService.getAllOrder();
        List<Reservation> orderList = new ArrayList<>(orders.values());
        
        //
        List<Reservation> reservations = new ArrayList<>();
        Map<String, List<Reservation>> resultMap = new HashMap<>();
        for (User us : user.values()) {
        	List<Reservation> userReservations=new ArrayList<>();
        	for (Reservation reservation : reservations) {
                if(us.getuId().equals(reservation.getuId()))
                	userReservations.add(reservation);
            }
            resultMap.put(us.getuId(), userReservations);
        }
        //发送
        request.setAttribute("orderList", orderList);
        request.setAttribute("UserList", UserList);
        //response.sendRedirect(contextPath + "/allUsers.jsp");
        request.getRequestDispatcher("allUsers.jsp").forward(request, response);
        /*
        //获取User的所有订单
        OrderService orderservice=new OrderService();
        
        //将uId放在数组中 
        Set<String> keySet = user.keySet();
        String[] users=keySet.toArray(new String[0]);
      //存放获得的order
        List<Reservation> orders = new ArrayList<>();     

    	for(String id:) {
    		System.out.println("ids"+id);
    		OrderDAO orderdao=new OrderDAO();
    		Reservation temp=orderservice.getOrderById(id);
    		orders.add(temp);
    		//System.out.println("temp"+temp.getoId()+temp.getoItems());
    	}
    	*/
	}

}
