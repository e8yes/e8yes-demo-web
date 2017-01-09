<%-- 
    Document   : join
    Created on : Jan 6, 2017, 10:42:41 PM
    Author     : davis
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Join Webchat</title>
    </head>
    <body>
        <h1>Webchat <%= request.getSession().getAttribute("version_string") %></h1>
        
        <div style="text-align: center; margin-top: 5%;">
            <form action=<%=request.getContextPath() + "/Join"%> method="POST">
                <div>
                    <div style="display: inline-block;" ><i>Alias</i></div>
                    <input type="text" name="user_name" value="" />
                </div>
                <div>
                    <div style="display: inline-block">Password</div>
                    <input type="password" name="user_password" value="" />
                </div>
                
                <input type="submit" value="Join" />
            </form>
        </div>
    </body>
</html>
