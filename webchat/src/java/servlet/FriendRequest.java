/*
 * Copyright (C) 2017 davis
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
package servlet;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 *
 * @author davis
 */
public class FriendRequest extends HttpServlet {

        public static Map<String, String> process_query_string(String query) {
                String[] params = query.split("&");
                HashMap<String, String> map = new HashMap<>();
                for (String param : params) {
                        String[] s = param.split("=");
                        if (s.length == 2) {
                                String name = s[0];
                                String value = s[1];
                                map.put(name, value);
                        }
                }
                return map;
        }
        
        /**
         * Processes requests for both HTTP <code>GET</code> and
         * <code>POST</code> methods.
         *
         * @param request servlet request
         * @param response servlet response
         * @throws ServletException if a servlet-specific error occurs
         * @throws IOException if an I/O error occurs
         */
        protected void processRequest(HttpServletRequest request, HttpServletResponse response)
                throws ServletException, IOException {
                response.setContentType("text/html;charset=UTF-8");
                PrintWriter out = response.getWriter();
                
                String qs = request.getQueryString();
                Map<String, String> kvs = process_query_string(qs);
                
                String action = kvs.get("action");
                String target = kvs.get("target");
                backend.User user;
                Integer uid;
                Integer target_id;
                try {
                        if (action == null || target == null) {
                                throw new Exception();
                        }
                        user = (backend.User) request.getSession().getAttribute("user");
                        uid = (Integer) request.getSession().getAttribute("user_id");
                        target_id = Integer.parseInt(target);
                } catch (Exception ex) {
                        out.print("Malformed request: " + qs);
                        return ;
                }
                
                
                switch (action) {
                        case "send":
                                if (!app.UserOperator.send_friend_request(user, target_id, out)) {
                                        return ;
                                }
                                break;
                        case "confirm":
                                if (!app.UserOperator.confirm_friend_request(user, target_id, out)) {
                                        return ;
                                }
                                break;
                        case "deny":
                                if (!app.UserOperator.deny_friend_request(user, target_id)) {
                                        out.println("You haven't made such friend request.");
                                        return ;
                                }
                                break;
                        case "remove":
                                if (!app.UserOperator.end_friendship(uid, target_id)) {
                                        out.println("You were not friend to start with");
                                        return ;
                                }
                                break;
                        default:
                                out.println("No such action as " + action);
                                return;
                }
                
                out.println("GOOD");
        }

        // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
        /**
         * Handles the HTTP <code>GET</code> method.
         *
         * @param request servlet request
         * @param response servlet response
         * @throws ServletException if a servlet-specific error occurs
         * @throws IOException if an I/O error occurs
         */
        @Override
        protected void doGet(HttpServletRequest request, HttpServletResponse response)
                throws ServletException, IOException {
                processRequest(request, response);
        }

        /**
         * Handles the HTTP <code>POST</code> method.
         *
         * @param request servlet request
         * @param response servlet response
         * @throws ServletException if a servlet-specific error occurs
         * @throws IOException if an I/O error occurs
         */
        @Override
        protected void doPost(HttpServletRequest request, HttpServletResponse response)
                throws ServletException, IOException {
                processRequest(request, response);
        }

        /**
         * Returns a short description of the servlet.
         *
         * @return a String containing servlet description
         */
        @Override
        public String getServletInfo() {
                return "Short description";
        }// </editor-fold>

}
