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
     <h2>订单详情页面</h2>
     <table border="1">
        <tr>
            <th>用户ID</th>
            <td><%= reserva.getuId() %></td>
        </tr>
        <tr>
            <th>订单ID</th>
            <td><%= reserva.getoId() %></td>
        </tr>
        <tr>
            <th>订单时间</th>
            <td><%= reserva.getoTime() %></td>
        </tr>
        <tr>
            <th>预订商品列表</th>
            <td><%= str %></td>
        </tr>
        <tr>
            <th>订单总价</th>
            <td><%= reserva.getoTotal() %></td>
        </tr>
        <tr>
            <th>订单状态</th>
            <td><%= reserva.getoStatus() %></td>
        </tr>
    </table>
    <button onclick="changeOrder()">变更订单</button>
    <script>
        function changeOrder() {
            if (document.querySelector("table td:nth-child(2)").innerText === "进行中") {
                window.location.href = "modifyOrderDetails.jsp";
            } else {
                alert("无法变更");
            }
        }
    </script>
  </body>
</html>
