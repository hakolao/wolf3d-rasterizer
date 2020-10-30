/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: veilo <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/30 18:07:34 by veilo             #+#    #+#             */
/*   Updated: 2020/08/30 18:07:37 by veilo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
**	Renders a triangle with given render function.
**	Also creates temporary clipping triangles
**	if the triangle intersects with the camera near plane.
**	The vtc[9] array is shared between the temporary triangles
*/

static void		render_triangle(t_wolf3d *app,
								void (*render_f)(uint32_t *buf[2],
												uint32_t *dim,
												t_triangle *tri),
								t_triangle *triangle)
{
	uint32_t	*buffers[2];
	uint32_t	dimensions[2];
	t_triangle	clipped_triangles[2];
	t_vertex	vtc[9];
	int32_t		test_clip;
	
	buffers[0] = app->window->framebuffer;
	buffers[1] = app->window->zbuffer;
	dimensions[0] = app->window->width;
	dimensions[1] = app->window->height;
	l3d_set_clipped_triangles(vtc, triangle, clipped_triangles);
	test_clip = l3d_clip_triangle(triangle,
		&app->active_scene->main_camera->viewplanes[0], clipped_triangles);
	if (test_clip == 2)
	{
		screen_intersection(app, &clipped_triangles[0]);
		screen_intersection(app, &clipped_triangles[1]);
		render_f(buffers, dimensions, &clipped_triangles[0]);
		render_f(buffers, dimensions, &clipped_triangles[1]);
	}
	else if (test_clip ==1)
	{
		screen_intersection(app, &clipped_triangles[0]);
		render_f(buffers, dimensions, &clipped_triangles[0]);
	}
	else
	{
		screen_intersection(app, triangle);
		render_f(buffers, dimensions, triangle);
	}
}

static void		rasterize_work(void *params)
{
	t_triangle_work	*work;

	work = params;
	render_triangle(work->app, l3d_triangle_raster, work->triangle);
	free(work);
}

static void		zbuffer_work(void *params)
{
	t_triangle_work	*work;

	work = params;
	render_triangle(work->app, l3d_triangle_set_zbuffer, work->triangle);
	free(work);
}

static void		add_triangle_work(t_wolf3d *app, t_render_pass pass,
					t_triangle *triangle)
{
	t_triangle_work	*work;

	error_check(!(work = malloc(sizeof(*work))), "Failed to malloc...");
	work->app = app;
	work->triangle = triangle;
	if (pass == rpass_rasterize)
		thread_pool_add_work(app->thread_pool, rasterize_work, (void*)work);
	else if (pass == rpass_zbuffer)
		thread_pool_add_work(app->thread_pool, zbuffer_work, (void*)work);
	else
		free(work);
}

static void		render_scene(t_wolf3d *app, t_render_pass pass)
{
	int				i;
	int				j;
	t_triangle		*triangle;

	i = -1;
	while (++i < (int)app->active_scene->num_objects)
	{
		j = -1;
		while (++j < app->active_scene->objects[i]->num_triangles)
		{
			triangle = app->active_scene->objects[i]->triangles + j;
			if (is_rendered(app, triangle))
				add_triangle_work(app, pass, triangle);
		}
	}
	thread_pool_wait(app->thread_pool);
}

void			wolf3d_render(t_wolf3d *app)
{	
	if (app->active_scene->main_camera != NULL)
	{
		render_scene(app, rpass_zbuffer);
		render_scene(app, rpass_rasterize);
	}
	ui_render(app);
}
