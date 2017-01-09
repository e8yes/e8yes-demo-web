<%-- 
    Document   : loginjsp
    Created on : Jan 6, 2017, 10:41:50 PM
    Author     : davis
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>

<!DOCTYPE html>
<html>
    <head>
        
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Welcome to Webchat</title>
    </head>
    <body>
        <h1>Webchat <%= request.getSession().getAttribute("version_string") %></h1>
        
        <form action=<%=request.getContextPath() + "/Login"%> method="POST">
            <div>User ID: </div>
            <input type="text" name="user_id" value="" />
            <div>Password: </div>
            <input type="password" name="user_password" value="" />
            <div></div>
            <input type="submit" value="Login" />
        </form>
            
        <a href=<%=request.getContextPath() + "/join.jsp"%>>Join Webchat Now</a>
        
        <p>Copylefted by Chifeng, 2017</p>
    </body>
</html>
