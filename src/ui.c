/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/02 16:14:01 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/03 16:15:08 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		render_menu(t_wolf3d *app)
{
	render_blinking_text(app, "Hello there!",
		(SDL_Color){255, 0, 0, 255},
		(int[2]){app->main_window->width / 2, app->main_window->height / 2});
}

void			render_ui(t_wolf3d *app)
{
	render_menu(app);
}
