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
  <style type="text/css">
  	  a{
  	 	display: inline-block;
        margin-right: 20px; /* 设置右边距 */
  	  	text-align: center;	  	
  	  }
  	  #table3{
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
	  #CourseForm{
	  	text-align: center;
	  }
  </style>
  <body onload="setCheckboxValue()">
  <%@ page import="edu.njust.entity.Course" %>
   <form action="/029_fourth/CourseController" method="post" id="CourseForm" accept-charset="UTF-8">
   	 <input type="hidden" name="action" value="del">

    	<table id="table3">
			<tr>
				<th></th>
    			<th>课程号</th>
    			<th>课程名称</th>
    			<th>课程人数</th>
    			<th>课程性质</th>
    		</tr>
			<% 
	            Object obj = request.getSession().getAttribute("courseList");	            
			    if (obj instanceof List) {
			        List<Course> courses = (List<Course>) obj;
			        for (Course course : courses) {
			            %>
			            <tr>
			            	<td><input type="checkbox" id="info" name="Ids" value=""><label for="info"></label></td>
				            <td><%= course.getcId() %></td>
				            <td><%= course.getcName() %></td>
				            <td><%= course.getcNum() %></td>
				            <td><%= course.getcType() %></td>
			       		 </tr>
			         <% } %>
				<% } %>
	             	       
		</table>	
    	<br>
    	 <!-- <button onclick="deleteSelectedRows()">退选</button>  -->
    	 <!-- <button type="submit">退选</button>-->
    	 <button onclick="handle()">退选</button>
    </form>
  </body>
  <script type="text/javascript">
  	function deleteSelectedRows() {
	    // 获取所有复选框元素
	    var checkboxes = document.querySelectorAll('input[type="checkbox"]');	
	    // 遍历复选框元素
	    for (var i = 0; i < checkboxes.length; i++) {
	        var checkbox = checkboxes[i];
	        // 检查复选框是否被选中
	        if (checkbox.checked) {
	            // 获取复选框所在的行元素
	            var row = checkbox.closest('tr');	
	            // 从表格中删除该行元素
	            row.remove();
	        }
	    }	
	    // 显示提示信息
	    alert('已成功删除选中课程！');
	}
	function setCheckboxValue() {
       var rows = document.getElementsByTagName("tr");
       for (var i = 1; i < rows.length; i++) {
           var checkbox = rows[i].querySelector("input[type='checkbox']");
           var cId = rows[i].querySelector("td:nth-child(2)").innerText;
           checkbox.value = cId;
       }
   }
    function submitForm() {
		// 提交表单的代码
		document.getElementById("CourseForm").submit();
	}
  	function handle(){
		submitForm(); 
		deleteSelectedRows(); 
  	}
  </script>
</html>
