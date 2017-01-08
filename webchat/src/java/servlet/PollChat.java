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

import backend.Message;
import com.sun.corba.se.impl.orbutil.concurrent.Mutex;
import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

class MessageInterruptListener implements backend.MessageListener {
        
        private final HttpServletResponse       m_response;
        private final ArrayList<Message>        m_messages;
        private boolean                         m_has_started = false;
        private final Mutex                     m_mutex;
        
        public MessageInterruptListener(HttpServletResponse response) {
                m_mutex = new Mutex();
                m_response = response;
                m_messages = new ArrayList<>();
        }
        
        public boolean shall_return() {
                try {
                        boolean ret_val;
                        m_mutex.acquire();
                        if (m_has_started && !m_messages.isEmpty()) {
                                MessageJSONizer.jsonize(m_messages, m_response.getWriter());
                                ret_val = true;
                        } else {
                                ret_val = false;
                        }
                        m_mutex.release();
                        return ret_val;
                } catch (InterruptedException | IOException ex) {
                        Logger.getLogger(MessageInterruptListener.class.getName()).log(Level.SEVERE, null, ex);
                        return true;
                }
        }
        
        @Override
        public void start() {
                try {
                        m_mutex.acquire();
                        m_has_started = true;
                        m_mutex.release();
                } catch (InterruptedException ex) {
                        Logger.getLogger(MessageInterruptListener.class.getName()).log(Level.SEVERE, null, ex);
                }
        }

        @Override
        public boolean on_receive(Message msg) {
                try {
                        boolean ret_val;
                        m_mutex.acquire();
                        ret_val = !m_has_started || m_messages.isEmpty();
                        if (ret_val) {
                                m_messages.add(msg);
                        }
                        m_mutex.release();
                        return ret_val;
                } catch (InterruptedException ex) {
                        Logger.getLogger(MessageInterruptListener.class.getName()).log(Level.SEVERE, null, ex);
                        return false;
                }
        }
        
}

/**
 *
 * @author davis
 */
public class PollChat extends HttpServlet {

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
                
                Integer uid = (Integer) request.getSession().getAttribute("user_id");
                if (uid == null) {
                        response.getWriter().print("You haven't logged in yet.");
                        return;
                }
                
                MessageInterruptListener listener = new MessageInterruptListener(response);
                app.MessageOperator.create_realtime_channel(uid, listener);
                
                // Wait roughly 30 seconds unless the listener get interrupted.
                for (int i = 0; i < 30; i ++) {
                        if (listener.shall_return())
                                break;
                        try {
                                Thread.sleep(1000);
                        } catch (InterruptedException ex) {
                                Logger.getLogger(PollChat.class.getName()).log(Level.SEVERE, null, ex);
                        }
                }
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
