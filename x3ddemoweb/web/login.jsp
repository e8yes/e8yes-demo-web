<%-- 
    Document   : loginjsp
    Created on : Jan 6, 2017, 10:41:50 PM
    Author     : davis, luan
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>

<!DOCTYPE html>
<html>
    <head>
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/font-awesome.css">
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/jquery-ui.css">
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/meteor.css">
        <script src="<%=request.getContextPath()%>/js/jquery-1.12.4.js"></script>
        <script src="<%=request.getContextPath()%>/js/jquery-ui.js"></script>
        <script src="<%=request.getContextPath()%>/js/about.js"></script>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Webchat</title>
    </head>  
    
    <%@include file="about_dialog.jsp" %>
    <%@include file="about_tag.jsp" %>
    
    <body class="background background-hk">
        <%@include file="heading.jsp" %>
        <div class="container">
            <form action=<%=request.getContextPath() + "/Login"%> method="POST">
                <div class="form-input">
                    <input type="text" name="user_id" placeholder="Enter user ID">
                </div>
                <div class="form-input">
                    <input type="password" name="user_password" placeholder="Enter Password">
                </div>
                <input type="submit" value="Login" class="btn">
            </form>
            <a href=<%=request.getContextPath() + "/join.jsp"%>>Sign up now</a>
        </div>
    </body>
</html>
