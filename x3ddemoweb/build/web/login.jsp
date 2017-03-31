<%-- 
    Document   : loginjsp
    Created on : Jan 6, 2017, 10:41:50 PM
    Author     : davis, luan
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>

<!DOCTYPE html>
<html>
    <head>
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/bootstrap.min.css">
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/font-awesome.css">
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/jquery-ui.css">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js"></script>
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/js/jquery-3.1.1.js">
        <!--<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">-->
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js"></script>
        <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/animate.css">

        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/meteor.css">
        <script src="<%=request.getContextPath()%>/js/jquery-1.12.4.js"></script>
        <script src="<%=request.getContextPath()%>/js/jquery-ui.js"></script>
        <script src="<%=request.getContextPath()%>/js/about.js"></script>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Webchat</title>
    </head>  

    <%@include file="footer_modal.jsp" %>
    <%--<%@include file="about_tag.jsp" %>--%>


    <body class="background background-hk">
        <%@include file="heading.jsp" %>
        <form action=<%=request.getContextPath() + "/Login"%> method="POST">
            <div class="container container-down animated fadeInDown fasterAnimation">
                <div class="col-md-4 col-md-offset-4 form-group">
                    <div class="input-group input-group-sm">
                        <span class="input-group-addon"><i class="fa fa-user" aria-hidden="true"></i></span>
                        <input type="text" name="user_id" class="form-control" placeholder="Enter user ID...">
                    </div>
                </div>
                <br/>
                <div class="col-md-4 col-md-offset-4 form-group">
                    <div class="input-group input-group-sm">
                        <span class="input-group-addon"><i class="fa fa-key" aria-hidden="true"></i></span>
                        <input type="password" name="user_password" class="form-control" placeholder="Enter password...">
                    </div>
                </div>
                <br/>
                <div class="col-md-4 col-md-offset-4 form-group">
                    <button type="submit" class="btn btn-md btn-login">Login</button>
                    <div class="col-md-4 col-md-offset-4 form-group join-result-a"><a href=<%=request.getContextPath() + "/join.jsp"%>><u>sign up now</u></a></div>
                </div>
            </div>
        </form>
    </body>
</html>
