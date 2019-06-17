package org.e8yes.srvs.buzlogic.ctx;

import java.util.Set;
import org.e8yes.srvs.Permission;

/**
 * Defines the identity context of a user -- Who is that and what can it do?
 *
 * @author davis
 */
public class IdentityContext {

        public Long userId;
        public Set<Permission> perms;
}
