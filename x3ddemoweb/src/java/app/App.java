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
package app;

/**
 * @author davis
 */
public class App {
        
        public static void init() {
                backend.SingletonEntities.init();
        }
        
        public static String get_version_string() {
                return "1.0-a1";
        }
        
        public static void destroy() {
		backend.SingletonEntities.destroy();
        }
	
	public static String get_db_console_url() {
		return backend.SingletonEntities.get_db_connector().get_db_console_location();
	}
}
