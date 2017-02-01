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
        <script src="<%=request.getContextPath()%>/js/meteor.js"></script>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    </head>  
    
    <a id="opener" class="about"><i class="fa fa-question-circle-o" aria-hidden="true"></i> About WebChat</a>
    <div id="dialog" title="About WebChat">
        <div>Just as it seems, </div>
        <p class='text-font'>
            This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. 
        </p>
        <p class='text-font'>
            This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
        </p>
        <p class='text-font'>
            You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
        </p>
        <p>You may obtain the entirety of the program over 
            <a href="https://github.com/DaviesX/webchat">here</a>
        </p>
        <a href="https://opensource.org/licenses/gpl-2.0.php">GPL2 -- Free of beer; Free of compliance.</a>
        <p class='text-font2'>Copylefted by Chifeng, 2017</p>
    </div>
    <body class="background">
        <h1>
            <i class="fa fa-comments-o" aria-hidden="true"></i>Webchat <%=request.getSession().getAttribute("version_string")%>
        </h1>
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
