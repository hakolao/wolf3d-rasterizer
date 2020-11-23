/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_buffers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 12:47:57 by ohakola+vei       #+#    #+#             */
/*   Updated: 2020/11/23 14:11:30 by ohakola+vei      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d.h"

static t_sub_zbuffer		*l3d_zbuffer_sub_create(t_zbuffer *parent,
														int32_t x, int32_t y)
{
	t_sub_zbuffer	*sub_buffer;

	error_check(!(sub_buffer = malloc(sizeof(t_sub_zbuffer))),
				"Failed to malloc frame sub buffer");
	sub_buffer->height = parent->height / parent->num_y;
	sub_buffer->width = parent->width / parent->num_x;
	sub_buffer->parent_height = parent->height;
	sub_buffer->parent_width = parent->width;
	sub_buffer->x_start = x * sub_buffer->width;
	sub_buffer->y_start = y * sub_buffer->height;
	error_check(!(
		sub_buffer->buffer = malloc(sizeof(float) *
			sub_buffer->height * sub_buffer->width)),
			"Failed to malloc frame sub buffer");
	return (sub_buffer);
}

static void					l3d_zbuffer_sub_destroy(t_sub_zbuffer *sub_buffer)
{
	if (sub_buffer == NULL)
		return ;
	free(sub_buffer->buffer);
	free(sub_buffer);
}

static t_sub_framebuffer	*l3d_framebuffer_sub_create(t_framebuffer *parent,
														int32_t x, int32_t y)
{
	t_sub_framebuffer	*sub_buffer;

	error_check(!(sub_buffer = malloc(sizeof(t_sub_framebuffer))),
				"Failed to malloc frame sub buffer");
	sub_buffer->height = parent->height / parent->num_y;
	sub_buffer->width = parent->width / parent->num_x;
	sub_buffer->parent_height = parent->height;
	sub_buffer->parent_width = parent->width;
	sub_buffer->x_start = x * sub_buffer->width;
	sub_buffer->y_start = y * sub_buffer->height;
	error_check(!(
		sub_buffer->buffer = malloc(sizeof(uint32_t) *
			sub_buffer->height * sub_buffer->width)),
			"Failed to malloc frame sub buffer");
	return (sub_buffer);
}

static void					l3d_framebuffer_sub_destroy(
								t_sub_framebuffer *sub_buffer)
{
	if (sub_buffer == NULL)
		return ;
	free(sub_buffer->buffer);
	free(sub_buffer);
}

t_framebuffer				*l3d_framebuffer_create(int32_t width, int32_t height)
{
	t_framebuffer	*framebuffer;
	int32_t			x;
	int32_t			y;
	int32_t			index;

	error_check(!(framebuffer = malloc(sizeof(t_framebuffer))),
		"Failed to malloc frame buffer");
	error_check(!(framebuffer->buffer =
		malloc(sizeof(uint32_t) * width * height)),
		"Failed to malloc frame buffer");
	framebuffer->num_x = 4;
	framebuffer->num_y = 4;
	framebuffer->width = width;
	framebuffer->height = height;
	error_check(!(framebuffer->sub_buffers = malloc(sizeof(t_sub_framebuffer*) *
				framebuffer->num_y * framebuffer->num_x)),
			"Failed to malloc frame sub buffers");
	y = -1;
	while (++y < framebuffer->num_y)
	{
		x = -1;
		while (++x < framebuffer->num_x)
		{
			index = y * framebuffer->num_x + x;
			framebuffer->sub_buffers[index] =
				l3d_framebuffer_sub_create(framebuffer, x, y);
		}
	}
	return (framebuffer);
}

void						l3d_framebuffer_destroy(t_framebuffer *framebuffer)
{
	int32_t		i;

	if (framebuffer != NULL)
	{
		i = -1;
		while (++i < framebuffer->num_x * framebuffer->num_y)
			l3d_framebuffer_sub_destroy(framebuffer->sub_buffers[i]);
		free(framebuffer->sub_buffers);
		free(framebuffer->buffer);
		free(framebuffer);
		framebuffer = NULL;
	}
}

t_zbuffer					*l3d_zbuffer_create(int32_t width, int32_t height)
{
	t_zbuffer		*zbuffer;
	int32_t			x;
	int32_t			y;
	int32_t			index;

	error_check(!(zbuffer = malloc(sizeof(t_zbuffer))),
		"Failed to malloc frame buffer");
	error_check(!(zbuffer->buffer =
		malloc(sizeof(uint32_t) * width * height)),
		"Failed to malloc frame buffer");
	zbuffer->num_x = 4;
	zbuffer->num_y = 4;
	zbuffer->width = width;
	zbuffer->height = height;
	error_check(!(zbuffer->sub_buffers = malloc(sizeof(t_sub_zbuffer*) *
				zbuffer->num_y * zbuffer->num_x)),
			"Failed to malloc frame sub buffers");
	y = -1;
	while (++y < zbuffer->num_y)
	{
		x = -1;
		while (++x < zbuffer->num_x)
		{
			index = y * zbuffer->num_x + x;
			zbuffer->sub_buffers[index] = l3d_zbuffer_sub_create(zbuffer, x, y);
		}
	}
	return (zbuffer);
}

void						l3d_zbuffer_destroy(t_zbuffer *zbuffer)
{
	int32_t		i;

	if (zbuffer != NULL)
	{
		i = -1;
		while (++i < zbuffer->num_x * zbuffer->num_y)
			l3d_zbuffer_sub_destroy(zbuffer->sub_buffers[i]);
		free(zbuffer->sub_buffers);
		free(zbuffer->buffer);
		free(zbuffer);
		zbuffer = NULL;
	}
}

t_l3d_buffers				*l3d_render_buffers_create(int32_t width, int32_t height)
{
	t_l3d_buffers	*buffers;

	error_check(!(buffers = malloc(sizeof(t_l3d_buffers))),
		"Failed to malloc l3d buffers struct");
	buffers->framebuffer = l3d_framebuffer_create(width, height);
	buffers->zbuffer = l3d_zbuffer_create(width, height);
	return (buffers);
}

void						l3d_render_buffers_destroy(t_l3d_buffers *buffers)
{
	if (buffers != NULL)
	{
		l3d_zbuffer_destroy(buffers->zbuffer);
		l3d_framebuffer_destroy(buffers->framebuffer);
		free(buffers);
	}
}

void						l3d_render_buffers_recreate(t_l3d_buffers **buffers,
												int32_t width, int32_t height)
{
	
	if (buffers != NULL && *buffers != NULL)
		l3d_render_buffers_destroy(*buffers);
	*buffers = l3d_render_buffers_create(width, height);
}