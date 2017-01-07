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
        <form action=<%=request.getContextPath() + "/Join"%> method="POST">
            <div>Password</div>
            <input type="text" name="user_password" />
            <input type="submit" value="Join" />
        </form>
    </body>
</html>
