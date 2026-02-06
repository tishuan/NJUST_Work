<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%
	String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>ThirdWork_029</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->

  </head>
  
    <style type="text/css">
  	  #couMng{
  	  	text-align: center;
  	  	font-size:15px;
  	  }
  	  a{
  	 	display: inline-block;
        margin-right: 20px; /* 设置右边距 */
  	  	text-align: center;	  	
  	  }
  	  #table1{
  	  	width:500px;
  	  	height:300px;
  	  	border: 1px solid black;
  	  	border-bottom: 1px solid black;
  	  	border-collapse: collapse;
  	  	text-align:center;
  	  	margin:auto;
  	  }
  	  table{
		border-right:1px solid black;		
		border-bottom:1px solid black;		
	  }		
	  td,th{		
		border-left:1px solid black;		
		border-top:1px solid black;		
	  }
  </style>
  <body>
  		<%@ page import="edu.njust.dao.UserDAO" %>
		<%
			String name=(String)request.getSession().getAttribute("username"); //名字
			String pass=(String)request.getSession().getAttribute("password");//密码
			String schid=UserDAO.findUserByUsername(name);//学号
		%>
		
       <div id="couMng">
    	<br>
    	<pre>姓名: <%=name%></pre>
    	<pre>学号：<%=schid%></pre>   	
    	<br>
    	
    	<table id="table1">
    		<tr>
    			<th>是否选择</th>
    			<th>序号</th>
    			<th>课程名称</th>
    			<th>分数</th>
    		</tr>
    		<tr>
    			<td><input type="checkbox" id="info1" name="info1"><label for="info1"></label></td>
    			<td>1</td>
    			<td>javaEE</td>
    			<td>100</td>
    		</tr>
    		<tr>
    			<td><input type="checkbox" id="info2" name="info2"><label for="info2"></label></td>
    			<td>2</td>
    			<td>C++</td>
    			<td>90</td>
    		</tr>
    		<tr>
    			<td><input type="checkbox" id="info3" name="info3"><label for="info3"></label></td>
    			<td>3</td>
    			<td>软件课程设计</td>
    			<td>80</td>
    		</tr>   		
    	</table>
    	<br>
    	<div id="alink">
    		<a href="http://localhost:8080/029-ThirdWork/jsp/page.jsp">
    			<button type="button">退选</button>
    		</a>
    		<a href="http://localhost:8080/029-ThirdWork/jsp/page.jsp">
    			<button type="button">课程管理</button>
    		</a>
    	</div>
    </div>
  </body>
</html>
