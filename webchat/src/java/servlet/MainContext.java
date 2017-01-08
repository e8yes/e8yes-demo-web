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

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

/**
 *
 * @author davis
 */
public class MainContext implements ServletContextListener {

        ServletContext context;

        @Override
        public void contextInitialized(ServletContextEvent e) {
                System.out.println(e.getServletContext().getContextPath() + " Context Created");
                app.App.init();
                e.getServletContext().setAttribute("version_string", app.App.get_version_string());
        }

        @Override
        public void contextDestroyed(ServletContextEvent e) {
                app.App.destroy();
                System.out.println(e.getServletContext().getContextPath() + " Context Destroyed");
        }
}
