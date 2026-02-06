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
  	#loginForm{
  		text-align: center;
  	}
  	#op_sch,#op_maj{
  		text-align: center;
  		width:170px;
  	}
  	#yanzheng{
  		width: 80px;		 
		height: 20px;	
		font-size: 10px;	 
		border: 1px solid;		 
		text-align: center;		 
		line-height: 20px;
		background-image: url("<%=basePath%>/img/img1.jpg");
		display: block;
		margin-left: 50%;
		margin-right: auto;
  	}
  </style>
  <body>
  	<div style="text-align:center" id="format">
	  	<form action="/029_fourth/LoginController" method="post" id="loginForm" accept-charset="UTF-8">
			<pre>用户名：   <input type="text" id="name" name="name"></pre>
	    	<pre>密码   ：   <input type="text" id="psw" name="psw"></pre>
	    	<pre>所在学院：  <select id="op_sch">
	    		<option value="">请选择</option>
	    	</select></pre>
	    	<pre>所在专业：  <select id="op_maj"> 	
	    		<option value="">请选择</option>
	    	</select></pre>
	    	<pre>验证码：    <input type="text" id="vecode" name="vecode" maxlength="4" ></pre>
	    	<div id="yanzheng">请输入验证码</div>
	    	<br>
	    	<button type="button" id="logbtn">登录</button>
	    	<br>
	    	<span id="msg" style="font-size:12px;color:red"></span>
	    </form>
    </div>
  </body>

  <script type="text/javascript" src="<%=basePath%>/js/jquery-3.7.1.min.js"></script>
  <script type="text/javascript">
  		//下拉框两级联动
	    var school=new Array();
	    school["计算机学院"]=["计科","软工","智能"];
	    school["电光学院"]  =["电1","电2","电3"];
	    school["自动化学院"]=["自动化1","自动化2","自动化3"];
	    var getSchool=document.getElementById("op_sch");
	    var getMaj	 =document.getElementById("op_maj");
	    for(var i in school){
	    	getSchool.add(new Option(i,i),null);
	    }
	    getSchool.onchange=function(){
	    	getMaj.length=0;
	    	var sel=getSchool.value;
	    	for(var j in school[sel])
	    		getMaj.add(new Option(school[sel][j],school[sel][j]),null);
	    }
	    
	    //当点击验证码时,生成验证码
	    var code4='';
	    yanzheng.onclick=function(){
	    	code4=getCode();
	    	yanzheng.innerHTML=code4;
	    }
	    
  		//当logbtn事件触发时
  		$("#logbtn").click(function(){
  			 //获取输入用户名，密码，验证码信息
  			 var uname=$("#name").val();
  			 var upwd =$("#psw"	).val();
  			 var icode=$("#vecode").val();
  			 if(isEmpty(uname)){
  			 	$("#msg").html("用户名不可为空");
  			 	return;
  			 }			 	
  			 else if(isEmpty(upwd)){
  			 	$("#msg").html("密码不可为空");
  			 	return;
  			 }			 	
  			 else if(!correctForm(upwd)){
  			 	$("#msg").html("密码必须同时包含数字字母");
  			 	return;
  			 }	
  			 else if(!isEqual(icode,code4)){
  			 	$("#msg").html("验证码错误");	
  			 	return;
  			 }			 		 
  			 else $("#loginForm").submit();
  		})
  		
  		//判断用户名，密码是否为空
  		function isEmpty(str){
  			if(str==null||str.trim()=="") return true;
  			return false;
  		}
  		//判断密码格式
  		function correctForm(str){
  			var pattern=/^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d]+$/;
  			if(pattern.exec(str)) return true;
  			return false;
  		}
  		//判断是否相等
  		function isEqual(str1,str2){
  			if(str1==str2) return true;
  			return false;
  		}
  		//生成随机数
  		function getRandom(min, max) {
	  		return Math.floor(Math.random() * (max - min + 1)) + min;
		}
		//生成随机4位 	
  		function getCode(){
  			var str='1234567890abcdefghigklmnopqrstuvwxyzABCDEFGHIGKLMNOPQRSTUVWXYZ';
	  		var code='';
	  		for(var i=1;i<=4;i++){
	  			var n=getRandom(0,str.length-1);
	  			code+=str[n];
	  		}
	  		return code;
  		}
  
  </script>
</html>
