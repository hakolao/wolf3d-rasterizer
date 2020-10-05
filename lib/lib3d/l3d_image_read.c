/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_image_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 13:33:28 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/05 17:24:00 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

/*
** BGR to RGB
*/

static void		l3d_copy_bmp_pixels_rgb(t_bmp_file_info_header *info_header,
				char *bmp_image)
{
	int32_t					i;
	unsigned char			temp_rgb;

	i = 0;
	while (i < (int32_t)info_header->image_size)
	{
		temp_rgb = bmp_image[i];
		bmp_image[i] = bmp_image[i + 2];
		bmp_image[i + 2] = temp_rgb;
		i += 3;
	}
}

/*
** ABGR to RGBA
*/

static void		l3d_copy_bmp_pixels_rgba(t_bmp_file_info_header *info_header,
				char *bmp_image)
{
	int32_t					i;
	int32_t					j;
	unsigned char			temp_rgb;

	i = 0;
	while (i < (int32_t)info_header->image_size)
	{
		j = -1;
		while (++j < 2)
		{
			temp_rgb = bmp_image[i + j];
			bmp_image[i + j] = bmp_image[i + 2 - j - 1];
			bmp_image[i + 2 - j - 1] = temp_rgb;
		}
		i += 4;
	}
}

static void		l3d_read_bmp_image(const char *filename, t_image *image_res)
{
	t_file_contents			*file;
	t_bmp_file_header		header;
	t_bmp_file_info_header	info_header;
	char					*bmp_image;
	void					*buf;


	file = read_file(filename);
	buf = file->buf;
	ft_memcpy(&header, buf, sizeof(header));
	ft_printf("%d\n", header.file_type);
	error_check(header.file_type != 0x4D42, "Invalid bmp image");
	ft_memcpy(&info_header, buf + sizeof(header), sizeof(info_header));
	error_check(!(bmp_image = malloc(sizeof(char) * info_header.image_size)),
		"Failed to malloc image pixels");
	ft_memcpy(bmp_image, buf, info_header.image_size);
	if (info_header.bits_per_pixel == 24)
		l3d_copy_bmp_pixels_rgb(&info_header, bmp_image);
	else if (info_header.bits_per_pixel == 32)
		l3d_copy_bmp_pixels_rgba(&info_header, bmp_image);
	image_res->bytes_per_pixel = info_header.bits_per_pixel / 8;
	image_res->width = info_header.width;
	image_res->height = info_header.height;
	image_res->pixels = bmp_image;
	destroy_file_contents(file);
}

/*
** Replaces alpha with 255 if image was only 24 bit.
*/

void			l3d_read_bmp_image_32bit_rgba(const char *filename,
				uint32_t **pixels_out, uint32_t *width, uint32_t *height)
{
	t_image		image;
	int32_t		i;
	uint32_t	*pixels;

	l3d_read_bmp_image(filename, &image);
	error_check(!(pixels =
		malloc(sizeof(uint32_t) * image.width * image.height)),
		"Failed to malloc 32 bit pixels");
	i = -1;
	while (++i < (int32_t)(image.height * image.width))
	{
		ft_memset(pixels + i, 255, sizeof(uint32_t));
		ft_memcpy(pixels + i, image.pixels + i, image.bytes_per_pixel * 8);
	}
	*pixels_out = pixels;
	*width = image.width;
	*height = image.height;
	free(image.pixels);
}