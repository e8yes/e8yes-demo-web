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
        <link rel="stylesheet" type="text/css" href="css/jquery-ui.css">
        <script src="js/jquery-1.12.4.js"></script>
        <script src="js/jquery-ui.js"></script>
        <script>
            $(function () {
                $("#dialog").dialog({
                    autoOpen: false,
                    show: {
                        effect: "fade",
                        duration: 500
                        
                    },
                    hide: {
                        effect: "fade",
                        duration: 500
                    }

                });

                $("#opener").on("click", function () {
                    $("#dialog").dialog("open");
                });
            });
            
        </script>
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
                padding-top: 1.9%;
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
                font-size: 12px;
                position: relative;
                left:92%;
            }
            h1 {
                position: relative;
                font-size: 35px;
                margin-top: 0;
                padding-left: 8%;
                font-family: 'FontAwesome', Apple LiGothic;
            }
            .ui-dialog{
                background: #73a09f;
                
            }
            </style>
        </head>  
        <a id="opener" class="about"><i class="fa fa-question-circle-o" aria-hidden="true"></i> About WebChat</a>
        <div id="dialog" title="About WebChat">
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
            <p style="font-size: 10pt; margin-top: 5%; text-align: right; margin-right: 5%;">Copylefted by Chifeng, 2017</p>

        </div>
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
