<%-- 
    Document   : user_tag
    Created on : Feb 1, 2017, 1:58:18 PM
    Author     : davis
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<div class="about">
                Page owner: <%=request.getSession().getAttribute("alias") %> 
                           (<%=request.getSession().getAttribute("user_id").toString() %>)
            </div>
