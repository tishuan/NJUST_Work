package edu.njust.service;

import java.util.Map;

import edu.njust.dao.OrderDAO;
import edu.njust.entity.Reservation;

public class OrderService {
	private OrderDAO orderDAO=new OrderDAO();
	public Map<String, Reservation> getAllOrder() {
        return orderDAO.getAllOrders();
    }
	public Reservation getOrderById(String id) {
        return orderDAO.getOrderById(id);
    }
}
