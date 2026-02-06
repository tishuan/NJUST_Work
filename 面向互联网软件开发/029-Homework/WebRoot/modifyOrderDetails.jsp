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
  	<%  
  		String str=(String)request.getAttribute("str"); 
  		Reservation reserva=(Reservation)request.getAttribute("reserva"); 
  	%>
     <h2>订单状态更改页面</h2>
 
    <form action="OrderController" method="post">
    	<input type="hidden" name="action" value="change">
        <table border="1">
            <tr>
            <th>用户ID</th>
            <td><input type="text" name="orderId" value="reseva.getuId()" readonly></td>
        </tr>
        <tr>
            <th>订单ID</th>
            <td><input type="text" name="userId" value="reseva.getoId()" readonly></td>
        </tr>
        <tr>
            <th>订单时间</th>
            <td><input type="text" name="productName" value="reseva.getoTime()" readonly></td>
        </tr>
        <tr>
            <th>预订商品列表</th>
            <td><input type="text" name="productName" value="str" readonly></td>
        </tr>
        <tr>
            <th>订单总价</th>
            <td><input type="text" name="quantity" value="reseva.getoTotal()" readonly></td></td>
        </tr>
        <tr>
            <th>订单状态</th>
            <td><select name="status">
                        <option value="进行中" reseva.getostatus() == '进行中' ? 'selected' : ''}>进行中</option>
                        <option value="已撤销" reseva.getostatus() == '已撤销' ? 'selected' : ''}>已撤销</option>
                        <option value="已完成" reseva.getostatus() == '已完成' ? 'selected' : ''}>已完成</option>
                 </select></td>
        </tr>
        </table>
        <input type="submit" value="确认变更">
        <input type="button" value="取消变更" onclick="location.href='showOrderDetails.jsp'">
    </form>
  </body>
</html>
