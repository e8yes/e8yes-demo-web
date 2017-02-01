<%-- 
    Document   : join
    Created on : Jan 6, 2017, 10:42:41 PM
    Author     : davis
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
        <title>Join Webchat</title>
    </head>
    
    <%@include file="about_dialog.jsp" %>
    <%@include file="about_tag.jsp" %>
    
    <body class="background background-hk">
        <%@include file="heading.jsp" %>
        <div class="container container-transparent">
            <form action=<%=request.getContextPath() + "/Join"%> method="POST">
                <div class="form-input">
                    <input type="text" name="user_name" value="" placeholder="Enter an alias" />
                </div>
                <div class="form-input">
                    <input type="password" name="user_password" value="" placeholder="Enter password" />
                </div>
                
                <input type="submit" value="Join" class="btn" />
            </form>
        </div>
    </body>
</html>
