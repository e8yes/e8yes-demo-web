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
import java.util.logging.Level;
import java.util.logging.Logger;
import servlet.PollNotification;

/**
 * @author davis
 */
public class NotificationPoller {
        
        public static void poll_notification(int uid, int delay, int length, PrintWriter out) {
                NotificationInterruptListener listener = new app.NotificationInterruptListener();
                MessageOperator.create_realtime_channel(uid, listener);
                
                // Wait roughly "length" seconds unless the listener get interrupted.
                for (int i = 0; i < length/delay; i ++) {
                        ArrayList<backend.Notification> noti = listener.fetch_notifications();
                        if (noti != null) {
                                NotificationJSONizer.jsonize(noti, out);
                                break;
                        }
                        
                        try {
                                Thread.sleep(delay);
                        } catch (InterruptedException ex) {
                                Logger.getLogger(PollNotification.class.getName()).log(Level.SEVERE, null, ex);
                        }
                }
                
                MessageOperator.close_realtime_channel(uid);
        }
}
