<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>029</title>
    
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
       <form action="CourseController" method="post">
       		<input type="hidden" name="action" value="add">
	        <label for="courseName">课程名称：</label>
	        <input type="text" id="courseName" name="courseName" ><br>
	        <label for="courseCapacity">选课人数：</label>
	        <input type="number" id="courseCapacity" name="courseCapacity" ><br>
	        <label for="courseType">课程性质：</label>
			<select id="courseType" name="courseType" >
			    <option value="必修">必修</option>
			    <option value="选修">选修</option>
			</select><br>	        
	        <!--  <input type="text" id="courseType" name="courseType" ><br>-->
	        <input type="submit" value="添加课程">
    	</form>
  </body>
</html>
