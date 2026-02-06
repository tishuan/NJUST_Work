package edu.njust.servlet;


import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;


import edu.njust.entity.Reservation;
import edu.njust.service.OrderService;

/**
 * Servlet implementation class OrderController
 */
@WebServlet("/OrderController")
public class OrderController extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public OrderController() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		this.doPost(request, response);
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		request.setCharacterEncoding("UTF-8");
		response.setContentType("text/html;charset=UTF-8");
		String uId=request.getParameter("uId");
		String oId=request.getParameter("oId");
		OrderService orderService=new OrderService();
		//获取订单信息
		Reservation reserva=orderService.getOrderById(oId);
		String str=orderService.getDetailsByOId(oId);
		//
		String action = request.getParameter("action");
		if(action.equals("确认变更")) {
			if (reserva.getoStatus() != null && reserva.getoStatus().equals("已完成")) {
	            
	            request.setAttribute("message", "变更成功");
	            request.getRequestDispatcher("showOrderDetails.jsp").forward(request, response);
	        } else {
	            
	            request.setAttribute("error", "不能变更");
	            request.getRequestDispatcher("modifyOrderDetails.jsp").forward(request, response);
	        }
		}

		request.setAttribute("str", str);
        request.setAttribute("reserva", reserva);
        request.getRequestDispatcher("/showOrderDetails.jsp").forward(request, response);
	}

}
