<%-- 
    Document   : join_result
    Created on : Jan 6, 2017, 11:37:20 PM
    Author     : davis, luan
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/meteor.css">
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Webchat</title>
    </head>

    <body class="background background-hk">
        <h1>You are now a member of e8yes WebChat family! Log in with the user ID <%=request.getAttribute("user_id").toString()%> to start making friends on WebChat!</h1>
        <div class="join-result-a">
            <a class="result-font-a" href=<%=request.getContextPath() + "/login.jsp"%>>Start Webchat</a>
        </div>
    </body>
</html>
