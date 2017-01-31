<%-- 
    Document   : chatroom
    Created on : Jan 6, 2017, 10:43:00 PM
    Author     : davis
--%>

<%@page import="java.util.HashMap"%>
<%@page import="java.util.ArrayList"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <script lang="javascript" src="js/jquery-3.1.1.js"></script>
        <title>Webchat@<%=request.getSession().getAttribute("alias") %> </title>
    </head>
    <body>
        <div>
        <!--<a style = "text-decoration: none; color: black;" href=<%=request.getContextPath()%>>-->
            <h1 style="display: inline-block;">Webchat <%= request.getSession().getAttribute("version_string") %></h1>
            <div style="float: right; margin-right: 2%; margin-top: 1%; font-size: 10pt; ">
                Page owner: <%=request.getSession().getAttribute("alias") %> 
                           (<%=request.getSession().getAttribute("user_id").toString() %>)
            </div>
        <!--</a>-->
        </div>
        
        <div style="text-align: center;">
            <h4>Add contact</h4>
            <div>
                <input id="t_friend_request_id" type="text" />
                <button id="b_send_request">Send request</button>
            </div>

             <h4>Friend requests</h4>
             <%
                    ArrayList<backend.User> friend_requests = (ArrayList<backend.User>) request.getAttribute("friend_request_list");
                    if (friend_requests == null || friend_requests.isEmpty()) {
                            out.println("<div>You don't have any friend request.</div>");
                    } else {
                        for (backend.User friend_request: friend_requests) {
                                out.println("<button name='b_friend_request' id='" + friend_request.get_user_id() + "'" + ">" 
                                            + "from " + friend_request.get_alias() 
                                            + "(" + friend_request.get_user_id() + ")" + "</button>");
                        }
                    }
            %>

            <h4>My friends</h4>
            <div>
            <%
                    ArrayList<backend.User> friends = (ArrayList<backend.User>) request.getAttribute("friend_list");
                    HashMap<Integer, Integer> unread_count = (HashMap<Integer, Integer>) request.getAttribute("unread_count_map");
                    if (friends == null || friends.isEmpty()) {
                            out.println("<div>You don't have any Webchat friend yet.</div>");
                    } else {
                        for (backend.User friend: friends) {
                                Integer n_unread = unread_count.get(friend.get_user_id());
                                out.println("<button name='b_friend' id='" + friend.get_user_id() + "'>" 
                                            + friend.get_alias() + "(" + friend.get_user_id() + ")" + "</button>");
                                out.println("(<div name='" + friend.get_user_id() + "'style='display: inline-block'>" 
                                            + (n_unread != null ? n_unread : 0) + "</div>)");
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
        </div>
    </body>
    
    <script lang="javascript">
        var user_id = <%=request.getSession().getAttribute("user_id")%>;
        var alias = "<%=request.getSession().getAttribute("alias")%>";
        
        var current_chat_target = null;
        var window_focusing = true;
        var chat_history_box = $("#t_chat_history");
        var msg_box = $("#t_msg_box");
        
        function update_chat_title() {
            if (current_chat_target === null) {
                $("#h_chat_title").html("Click on one of your friends and start chatting!");
            } else {
                $("#h_chat_title").html("My chat with " + current_chat_target);
            }
        }
        
        function process_chat_messages(chat) {
            if (current_chat_target !== null && chat.sender === current_chat_target) {
                append_to_chat_box(chat.sender, chat.content);
                // if (!window_focusing)
                pop_notification(chat.sender, chat.content);
            } else
                pop_notification(chat.sender, chat.content);
        }
        
        function process_friend_request(fr) {
            if (fr.type === "FriendRequest") {
                pop_notification(fr.sender_alias + "(" + fr.sender + ")", 
                                 " wanted to add you as his friend.");
            } else {
                pop_notification(fr.sender_alias + "(" + fr.sender + ")", 
                                 " has confirmed your friend request.");
            }
            location.reload();
        }
        
        function notification_poll() {
            // Long polling.
            $.ajax({url: "PollNotification",
                    success: function(result) {
                        try {
                            var notifications = JSON.parse(result);
                            for (var i = 0; i < notifications.length; i ++) {
                                switch (notifications[i].type) {
                                    case "Message":
                                        process_chat_messages(notifications[i]);
                                        break;
                                    case "FriendRequest":
                                    case "FriendRequestConfirm":
                                        process_friend_request(notifications[i]);
                                        break;
                                }
                            }
                        } catch (ex) {
                            if (result !== "") {
                                alert(result);
                                return ;
                            }
                        }
                        notification_poll();
            }});
        }
        
        function pop_notification(sender, content) {
            // Update notification tag.
            var n = parseInt($("div[name='" + sender + "']").html());
            $("div[name='" + sender + "']").html(n + 1);
            
            // Play sound.
            new Audio("snd/bing.mp3").play();
            
            // Pop out notification.
            if (!("Notification" in window)) {
                alert(sender + ": " + content);
            } else if (Notification.permission === "granted") {
                new Notification(sender + ": " + content);
            } else if (Notification.permission !== 'denied') {
              Notification.requestPermission(function (permission) {
                if (permission === "granted") {
                    new Notification(sender + ": " + content);
                }
              });
            }
        }
        
        function empty_chat_box() {
            chat_history_box.empty();
        }
        
        function append_to_chat_box(sender, content) {
            var sender_info;
            var fd = $("#" + sender);
            if (fd[0] !== undefined) {
                sender_info = fd.html();
            } else {
                sender_info = alias + "(" + user_id + ")";
            }
            chat_history_box.append(sender_info + ": " + content + "\n");
            chat_history_box.scrollTop(chat_history_box[0].scrollHeight);
        }
        
        function fire_chat(message) {
            if (current_chat_target === null || message === "") {
                alert("Cannot send your message to nobody :)");
                return false;
            } else {
                // Send chat content.
                $.ajax({url: "FireChat", data: {receiver: current_chat_target,
                                                    content: message},
                    success: function(result) {
                        if (result !== "GOOD") {
                            alert(result);
                            msg_box.val(message);
                        } else {
                            append_to_chat_box(<%=request.getSession().getAttribute("user_id") %>, message);
                        }
                }});
                return true;
            }
        }
        
        function update_chat_history(on_finished) {
            if (current_chat_target === null) {
                empty_chat_box();
            } else {
                // Request chat history.
                $.ajax({url: "ChatHistory", data: {sender: current_chat_target,
                                                    n: 20},
                    success: function(result) {
                        empty_chat_box();
                        
                        try {
                            var chats = JSON.parse(result);
                            
                            for (var i = 0; i < chats.length; i ++) {
                                append_to_chat_box(chats[i].sender, chats[i].content);
                            }
                            
                            on_finished();
                        } catch (ex) {
                            alert(result);
                            return ;
                        }
                }});
            }
        }
        
        $(window).focus(function() {
            window_focusing = true;
        });

        $(window).blur(function() {
            window_focusing = false;
        });

        $("button[name=b_friend]").on("click", function(e) {
            current_chat_target = parseInt(e.target.id);
            $.ajax({url: "ChatTo",
                    data: {receiver: e.target.id}, 
                    success: function(result) {
                        if (result === "GOOD") {
                            update_chat_title();
                            update_chat_history(function() {
                                $("div[name='" + current_chat_target + "']").html("0");
                            });
                        } else {
                            alert(result);
                        }
            }});
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
            if (fire_chat(msg_box.val()))
                msg_box.val("");
        });
        
        $("#t_msg_box").on("keypress", function(e) {
            if (e.keyCode === 13) { 
                if (fire_chat(msg_box.val()))
                    msg_box.val("");
                return false;
            }
            return true;
        });
        
        update_chat_title();
        update_chat_history();
        notification_poll();
    </script>
</html>
