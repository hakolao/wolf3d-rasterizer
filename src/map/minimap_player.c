/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/07 00:17:35 by ohakola           #+#    #+#             */
/*   Updated: 2020/12/07 01:03:21 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		direction_render(t_framebuffer *framebuffer, t_vec2 dir,
					t_vec2 render_pos, float size)
{
	int32_t		x1;
	int32_t		y1;
	int32_t		x2;
	int32_t		y2;
	uint32_t	dims[2];

	x1 = render_pos[0] + size / 2.0;
	y1 = render_pos[1] + size / 2.0;
	x2 = render_pos[0] + size / 2.0 + 30 * dir[0];
	y2 = render_pos[1] + size / 2.0 + 30 * dir[1];
	dims[0] = framebuffer->width;
	dims[1] = framebuffer->height;
	l3d_line_draw(framebuffer->buffer, dims,
		(int32_t[2][2]){{x1, y1}, {x2, y2}}, 0x00ff00ff);
}

void			player_dir_render(t_framebuffer *framebuffer,
					t_player *player, t_vec2 render_pos, float size)
{
	t_vec2		dir;

	ml_vector2_copy((t_vec2){-player->forward[2], player->forward[0]}, dir);
	ml_vector2_normalize(dir, dir);
	direction_render(framebuffer, dir, render_pos, size);
}
