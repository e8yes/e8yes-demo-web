<%-- 
    Document   : loginjsp
    Created on : Jan 6, 2017, 10:41:50 PM
    Author     : davis
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>

<!DOCTYPE html>
<html>
    <head>
        <link rel="stylesheet" type="text/css" href="css/font-awesome.css">

        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <style type="text/css">
            body {
                background-image: url("man_meteor.jpg");
                background-repeat: no-repeat;  
                background-size: cover;
                background-attachment: fixed;
                background-position: center;
            }
            .container{
                width: 260px;
                height: 150px;
                text-align: center;
                background-color: rgba(52, 73, 94, 0.7);
                margin: 0 auto;
                margin-top: 1%;
                color: #9B59B6;
            }
            input{
                height: 20px;
                width: 140px;
                font-size: 12px;
                margin-top: 7%;
                background-color: #fff;
                padding-left: 8%;
            }
            .form-input::before{
                content: "\f2c0";
                position: absolute;
                font-family: "FontAwesome";
                font-size: 15px;
                padding-top: 1.7%;
                padding-left: 0.3%;
            }
            .form-input:nth-child(2)::before{
                content: "\f023";
                padding-left: 0.4%;
            }
            .btn-login{
                width: 100px; 
                padding-left: 10%;
                padding-right: 10%;
                border: none;
                border-radius: 4px;
                color: #fff;
                background-color: #2ECC71;
                cursor: pointer;
            }
            a{
                color:#fff;
                font-size: 11px;
            }
            .about{
                color:#42d7f4;
                font-size: 11px;
                padding-left: 93%;
            }
            h1 {
                position: relative;
                font-size: 35px;
                margin-top: 0;
                padding-left: 8%;
                font-family: 'FontAwesome', Apple LiGothic;
            }
        </style>
    </head>
    <a class="about" href=<%=request.getContextPath() + "/about.jsp"%>><i class="fa fa-question-circle-o" aria-hidden="true"></i> About WebChat</a>

    <body>
        <h1><i class="fa fa-comments-o" aria-hidden="true"></i> Webchat <%= request.getSession().getAttribute("version_string")%></h1>
        <div class="container">
            <form action=<%=request.getContextPath() + "/Login"%> method="POST">
                <div class="form-input">
                    <input type="text" name="user_id" placeholder="Enter Username">
                </div>
                <div class="form-input">
                    <input type="password" name="user_password" placeholder="Enter Password">
                </div>
                <input type="submit" value="Login" class="btn-login">
            </form>
            <a href=<%=request.getContextPath() + "/join.jsp"%>>Sign up now</a>
        </div>
    </body>
</html>
