/* the Music Player Daemon (MPD)
 * Copyright (C) 2003-2007 by Warren Dukes (warren.dukes@gmail.com)
 * This project's homepage is: http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "../output_api.h"
#include "../timer.h"

#include <glib.h>

struct null_data {
	Timer *timer;
};

static void *
null_init(G_GNUC_UNUSED struct audio_output *audio_output,
	  G_GNUC_UNUSED const struct audio_format *audio_format,
	  G_GNUC_UNUSED struct config_param *param)
{
	struct null_data *nd = g_new(struct null_data, 1);

	nd->timer = NULL;

	return nd;
}

static bool
null_open(void *data, struct audio_format *audio_format)
{
	struct null_data *nd = data;

	nd->timer = timer_new(audio_format);

	return true;
}

static void
null_close(void *data)
{
	struct null_data *nd = data;

	if (nd->timer != NULL) {
		timer_free(nd->timer);
		nd->timer = NULL;
	}
}

static bool
null_play(void *data, G_GNUC_UNUSED const char *chunk, size_t size)
{
	struct null_data *nd = data;
	Timer *timer = nd->timer;

	if (!timer->started)
		timer_start(timer);
	else
		timer_sync(timer);

	timer_add(timer, size);

	return true;
}

static void
null_cancel(void *data)
{
	struct null_data *nd = data;

	timer_reset(nd->timer);
}

const struct audio_output_plugin null_output_plugin = {
	.name = "null",
	.init = null_init,
	.open = null_open,
	.close = null_close,
	.play = null_play,
	.cancel = null_cancel,
};
