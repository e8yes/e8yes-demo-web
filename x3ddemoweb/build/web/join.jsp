<%-- 
    Document   : join
    Created on : Jan 6, 2017, 10:42:41 PM
    Author     : davis, luan
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/font-awesome.css">
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/jquery-ui.css">
        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/animate.css">

        <link rel="stylesheet" type="text/css" href="<%=request.getContextPath()%>/css/meteor.css">
        <script src="<%=request.getContextPath()%>/js/jquery-1.12.4.js"></script>
        <script src="<%=request.getContextPath()%>/js/jquery-ui.js"></script>
        <script src="<%=request.getContextPath()%>/js/about.js"></script>
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js"></script>
        <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Join Webchat</title>
    </head>

    <%@include file="footer_modal.jsp" %>

    <body class="background background-hk">
        <%@include file="heading.jsp" %>
        <!--<div class="container container-transparent">-->
<!--            <form action=<%=request.getContextPath() + "/Join"%> method="POST">
                <div class="form-input">
                    <input type="text" name="user_name" value="" placeholder="Enter an alias" />
                </div>
                <div class="form-input">
                    <input type="password" name="user_password" value="" placeholder="Enter password" />
                </div>

                <input type="submit" value="Join" class="btn" />
            </form>-->

        <form action=<%=request.getContextPath() + "/Join"%> method="POST">
            <div class="container container-down animated fadeInDown fasterAnimation">
                <div class="col-md-4 col-md-offset-4 form-group">
                    <div class="input-group input-group-sm">
                        <span class="input-group-addon"><i class="fa fa-user" aria-hidden="true"></i></span>
                        <input type="text" name="user_name" class="form-control" value="" placeholder="Enter an alias...">
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
                    <button type="submit" class="btn btn-warning btn-join">Join the family!</button>
                </div>
            </div>
        </form>





        <!--</div>-->
    </body>
</html>
