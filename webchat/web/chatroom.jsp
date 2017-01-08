<%-- 
    Document   : chatroom
    Created on : Jan 6, 2017, 10:43:00 PM
    Author     : davis
--%>

<%@page import="java.util.ArrayList"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <script lang="javascript" src="js/jquery-3.1.1.js"></script>
        <title>Webchat User: <%=request.getAttribute("user_id").toString() %> </title>
    </head>
    <body>
        <h1>Webchat <%= request.getAttribute("version_string") %></h1>
        
        <h4>Add contact</h4>
        <div>
            <input id="t_friend_request_id" type="text" />
            <button id="b_send_request">Send request</button>
        </div>
        
         <h4>Friend requests</h4>
         <%
                ArrayList<Integer> friend_requests = (ArrayList<Integer>) request.getAttribute("friend_request_list");
                if (friend_requests == null || friend_requests.isEmpty()) {
                        out.println("<div>You don't have any friend request.</div>");
                } else {
                    for (Integer friend_request: friend_requests) {
                            out.println("<button name='b_friend_request' id='" + friend_request + "'" + ">" 
                                        + "from " + friend_request + "</button>");
                    }
                }
        %>
        
        <h4>My friends</h4>
        <div>
        <%
                ArrayList<Integer> friends = (ArrayList<Integer>) request.getAttribute("friend_list");
                if (friends == null || friends.isEmpty()) {
                        out.println("<div>You don't have any Webchat friend yet.</div>");
                } else {
                    for (Integer friend: friends) {
                            out.println("<button name=\"b_friend\" id=\"" + friend + "\"" + ">" 
                                        + friend + "</button>");
                    }
                }
        %>
        </div>

        <p>
            <button id="b_unfriend">Unfriend</button>
        </p>
        
        <h4 id="h_chat_title"></h4>
        <div>
            <textarea id="t_chat_history" cols="50" rows="10" readonly></textarea>
        </div>
        <div>
            <textarea id="t_msg_box" cols="50" rows="5"></textarea>
        </div>
        <button id="b_send">Send</button>
    </body>
    
    <script lang="javascript">
        var current_chat_target = null;
        
        function update_chat_title() {
            if (current_chat_target === null) {
                $("#h_chat_title").html("Click on one of your friend and start chatting!");
            } else {
                $("#h_chat_title").html("My chat with " + current_chat_target);
            }
        }
        
        function chat_poll() {
        }
        
        function fire_chat(message) {
            if (current_chat_target === null) {
                alert("Cannot send your message to nobody :)");
            } else {
                // Send chat content.
            }
        }
        
        function update_chat_history() {
            if (current_chat_target === null) {
                $("#t_chat_history").html("No history.");
            } else {
                // Request chat history.
            }
        }
        
        $("button[name=b_friend]").on("click", function(e) {
            current_chat_target = parseInt(e.target.id);
            update_chat_title();
            update_chat_history();
        });
        
        $("button[name=b_friend_request]").on("click", function(e) {
            if (confirm("Adding " + e.target.id + " as your friend?")) {
                // Confirm request.
                $.ajax({url: "FriendRequest?action=confirm&target=" + e.target.id, 
                    success: function(result) {
                    alert(result);
                    location.reload();
                }});
            } else {
                // Remove request.
                $.ajax({url: "FriendRequest?action=deny&target=" + e.target.id, 
                    success: function(result) {
                    alert(result);
                    location.reload();
                }});
            }
        });
        
        $("#b_send_request").on("click", function(e) {
            // Send friend request.
            var target_id = $("#t_friend_request_id").val();
            $.ajax({url: "FriendRequest?action=send&target=" + target_id, 
                    success: function(result) {
                    alert(result);
                    location.reload();
            }});
        });
        
        $("#b_unfriend").on("click", function(e) {
            var fd_id = prompt("Who would you wanna unfriend with?");
            if (fd_id !== null) {
                // Unfriend.
                $.ajax({url: "FriendRequest?action=remove&target=" + fd_id, 
                    success: function(result) {
                    alert(result);
                    location.reload();
                }});
            }
        });
        
        $("#b_send").on("click", function(e) {
            fire_chat(e.target.value);
        });
        
        update_chat_title();
        update_chat_history();
        chat_poll();
    </script>
</html>
