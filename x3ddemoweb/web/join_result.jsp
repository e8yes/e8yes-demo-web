<%-- 
    Document   : join_result
    Created on : Jan 6, 2017, 11:37:20 PM
    Author     : davis
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Webchat</title>
    </head>
    <body>
        <h1>You are now a webchatter! Log in with the user ID <%=request.getAttribute("user_id").toString() %> and start making friends!</h1>
        <a href=<%=request.getContextPath() + "/login.jsp"%>>Start Webchat</a>
    </body>
</html>
