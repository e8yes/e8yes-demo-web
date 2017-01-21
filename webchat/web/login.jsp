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
        <style type="text/css">
            body {
                background-image: url("https://www.eventival.eu/files/174/editions/1007/films/251294/accessories/208388.jpg");
                background-repeat: no-repeat;  
                background-size: cover;
                background-attachment: fixed;
                background-position: center;
            }
            .container{
                width: 300px;
                height: 200px;
                text-align: center;
                background-color: rgba(52, 73, 94, 0.7);
                margin: 0 auto;
                margin-top: 2%;
            }
            .container img{
                width: 120px;
                height: 120px;
                margin-top: -30px;
            }
            
        </style>
    </head>
    <body>
        <h1>Webchat <%= request.getSession().getAttribute("version_string")%></h1>

        <div class="container">
            <img src="mitsuha.jpg">
            <form action=<%=request.getContextPath() + "/Login"%> method="POST">
                <div>
                    <div style="display: inline-block;">User ID: </div>
                    <input type="text" name="user_id" value="" />
                </div>
                <div>
                    <div style="display: inline-block;">Password: </div>
                    <input type="password" name="user_password" value="" />
                </div>
                <div></div>
                <input type="submit" value="Login" />
            </form>
            <a href=<%=request.getContextPath() + "/join.jsp"%>>Join Webchat Now</a>
        </div>

        <p style="font-size: 10pt; margin-top: 5%; text-align: right; margin-right: 5%;">Copylefted by Chifeng, 2017</p>

        <div>Just as it seems, </div>
        <p style="font-size: 8pt;">
            This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. 
        </p>
        <p style="font-size: 8pt;">
            This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
        </p>
        <p style="font-size: 8pt;">
            You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
        </p>

        <p>You may obtain the entirety of the program over <a href="https://github.com/DaviesX/webchat">here</a></p>

        <a href="https://opensource.org/licenses/gpl-2.0.php">GPL2 -- Free of beer; Free of compliance.</a>
    </body>
</html>
