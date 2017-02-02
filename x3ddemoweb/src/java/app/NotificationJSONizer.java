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

import java.io.PrintWriter;
import java.util.ArrayList;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

/**
 * @author davis
 */
public class NotificationJSONizer {

        public static void jsonize(ArrayList<backend.Notification> notis, PrintWriter out) {
                JSONArray a = new JSONArray();
                notis.stream().forEach((noti) -> {
                        JSONObject o = noti.jsonize();
                        o.put("type", noti.get_type().toString());
                        a.add(o);
                });

                out.print(a.toString());
        }
}
