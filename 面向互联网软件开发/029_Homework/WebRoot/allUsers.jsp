<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>Homework_029</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->

  </head>
  
  <body>
  <%@ page import="edu.njust.entity.Reservation" %>
  <%@ page import="edu.njust.entity.User" %>
    <table border="1">
        <tr>
            <th>用户ID</th>
            <th>用户名</th>
            <th>商品订单</th>
        </tr>
        <% 
	        Object obj = request.getAttribute("orderList");
	        Object obj2 = request.getAttribute("UserList");	
	        if(obj==null) System.out.println("订单为空");
	        if(obj2==null) System.out.println("用户为空");
	        if (obj instanceof List && obj2 instanceof List) {
	        	List<Reservation> orderList=(List<Reservation>)obj;
	        	List<User> UserList=(List<User>)obj2;
	        	for(User user:UserList){%>
	        		<tr>
		        	<td><%= user.getuId() %></td>
	                <td><%= user.getuName() %></td>
	        		<% for (Reservation reservation : orderList) {
		                if(user.getuId().equals(reservation.getuId())){ %>
		                	<td><%= reservation.getoId() %>、</td> 
		                	<% System.out.println("uId"+reservation.getoId());
		                }
		           }
		           %><tr><%
	        	}
	        }
	        else{
	        	System.out.println("类型不匹配");
	        }
	    %>
  
    </table>
  </body>
</html>
