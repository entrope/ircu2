/*
 * IRC - Internet Relay Chat, ircd/m_die.c
 * Copyright (C) 1990 Jarkko Oikarinen and
 *                    University of Oulu, Computing Center
 *
 * See file AUTHORS in IRC package for additional names of
 * the programmers.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 1, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id$
 */

#include "config.h"

#include "client.h"
#include "ircd.h"
#include "ircd_log.h"
#include "ircd_reply.h"
#include "ircd_string.h"
#include "msg.h"
#include "numeric.h"
#include "numnicks.h"
#include "s_bsd.h"
#include "send.h"

/* #include <assert.h> -- Now using assert in ircd_log.h */


/** Handle a DIE message from an operator.
 *
 * \a parv is ignored.
 *
 * See @ref m_functions for discussion of the arguments.
 * @param[in] cptr Client that sent us the message.
 * @param[in] sptr Original source of message.
 * @param[in] parc Number of arguments.
 * @param[in] parv Argument vector.
 */
int mo_die(struct Client* cptr, struct Client* sptr, int parc, char* parv[])
{
  struct Client *acptr;
  int i;

  if (!HasPriv(sptr, PRIV_DIE))
    return send_reply(sptr, ERR_NOPRIVILEGES);

  for (i = 0; i <= HighestFd; i++)
  {
    if (!(acptr = LocalClientArray[i]))
      continue;
    if (IsUser(acptr))
      sendcmdto_one(&me, CMD_NOTICE, acptr, "%C :Server Terminating. %s",
		    acptr, get_client_name(sptr, HIDE_IP));
    else if (IsServer(acptr))
      sendcmdto_one(&me, CMD_ERROR, acptr, ":Terminated by %s",
		    get_client_name(sptr, HIDE_IP));
  }
  server_die("received DIE");

  return 0;
}
