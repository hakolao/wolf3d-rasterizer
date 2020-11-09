/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:19:50 by ohakola           #+#    #+#             */
/*   Updated: 2020/11/03 15:11:57 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int		window_resize_callback(void *data, SDL_Event *event)
{
	t_window	*window;

	if (event->type == SDL_WINDOWEVENT &&
		(event->window.event == SDL_WINDOWEVENT_RESIZED ||
		event->window.event == SDL_WINDOWEVENT_MINIMIZED ||
		event->window.event == SDL_WINDOWEVENT_MAXIMIZED ||
		event->window.event == SDL_WINDOWEVENT_SHOWN ||
		event->window.event == SDL_WINDOWEVENT_HIDDEN))
	{
		window = (t_window*)data;
		if (event->window.windowID == window->window_id)
		{
			window->resized = true;
			SDL_GetWindowSize(window->window, &window->width,
				&window->height);
			if (event->window.event == SDL_WINDOWEVENT_HIDDEN)
				window->is_hidden = true;
			else if (event->window.event == SDL_WINDOWEVENT_SHOWN)
				window->is_hidden = false;
		}
	}
	return (0);
}

void			window_frame_clear(t_window *window)
{
	int			i;
	uint32_t	color;

	i = 0;
	color = 0x000000FF;
	while (i < window->width * window->height)
	{
		window->buffers->framebuffer[i] = color;
		window->buffers->framebuffer[i + 1] = color;
		window->buffers->framebuffer[i + 2] = color;
		window->buffers->framebuffer[i + 3] = color;
		window->buffers->zbuffer[i] = INT32_MAX;
		window->buffers->zbuffer[i + 1] = INT32_MAX;
		window->buffers->zbuffer[i + 2] = INT32_MAX;
		window->buffers->zbuffer[i + 3] = INT32_MAX;
		i += 4;
	}
}

void			window_frame_draw(t_window *window)
{
	SDL_UpdateTexture(window->frame, NULL, window->buffers->framebuffer,
		window->width * 4);
	SDL_RenderCopy(window->renderer, window->frame, NULL, NULL);
	SDL_RenderPresent(window->renderer);
}

void			window_frame_recreate(t_window *window)
{
	if (window->frame != NULL)
		SDL_DestroyTexture(window->frame);
	window->frame = SDL_CreateTexture(window->renderer,
		PIXEL_FORMAT, SDL_TEXTUREACCESS_STREAMING, window->width,
		window->height);
	error_check(window->frame == NULL, SDL_GetError());
	if (window->buffers != NULL)
	{
		free(window->buffers->framebuffer);
		free(window->buffers->zbuffer);
		free(window->buffers);
	}
	error_check(!(window->buffers = malloc(sizeof(t_l3d_buffers))),
		"Failed to malloc wolf3d buffers struct");
	error_check(!(window->buffers->framebuffer = (uint32_t*)malloc(sizeof(uint32_t) *
		window->width * window->height)),
		"Failed to malloc framebuffer in resize");
	error_check(!(window->buffers->zbuffer = (float*)malloc(sizeof(float) *
		window->width * window->height)),
		"Failed to malloc zbuffer in resize");
	if (window->main_font != NULL)
		TTF_CloseFont(window->main_font);
	window->main_font = TTF_OpenFont(GAME_FONT, FONT_SIZE);
	error_check(window->main_font == NULL, TTF_GetError());
	if (window->debug_font != NULL)
		TTF_CloseFont(window->debug_font);
	window->debug_font = TTF_OpenFont(DEBUG_FONT, FONT_SIZE);
	error_check(window->debug_font == NULL, TTF_GetError());
}

void			window_create(t_window **window_ref)
{
	t_window *window;

	error_check((window = (t_window*)malloc(sizeof(t_window))) == NULL,
		"Window malloc failed");
	window->window = SDL_CreateWindow(NAME, SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
	window->width = WIDTH;
	window->height = HEIGHT;
	error_check(window->window == NULL, SDL_GetError());
	window->renderer =
		SDL_CreateRenderer(window->window, -1, SDL_RENDERER_SOFTWARE);
	error_check(window->renderer == NULL, SDL_GetError());
	window->window_id = SDL_GetWindowID(window->window);
	window->is_hidden = false;
	window->frame = NULL;
	window->buffers = NULL;
	window->main_font = NULL;
	window->debug_font = NULL;
	window_frame_recreate(window);
	SDL_AddEventWatch(window_resize_callback, window);
	window->resized = false;
	*window_ref = window;
}
