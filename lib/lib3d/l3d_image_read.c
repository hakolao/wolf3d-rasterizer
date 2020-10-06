/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   l3d_image_read.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 13:33:28 by ohakola           #+#    #+#             */
/*   Updated: 2020/10/06 19:35:07 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib3d_internals.h"

static void		l3d_flip_image_rows(t_image *image, uint32_t *pixels)
{
	int32_t		i;
	uint32_t	temp_row[L3D_MAX_BMP_WIDTH];

	i = -1;
	while (++i < (int32_t)image->height / 2)
	{
		ft_memcpy(temp_row, pixels + (i * image->width),
			image->width * sizeof(uint32_t));
		ft_memcpy(pixels + (i * image->width),
			pixels + (image->height - i - 1) * image->width,
				image->width * sizeof(uint32_t));
		ft_memcpy(pixels + (image->height - i - 1) * image->width,
			temp_row, image->width * sizeof(uint32_t));
	}
}

static void		l3d_bmp_colors_to_rgba(t_bmp_file_header *header,
					uint32_t bytes_per_pixel, char *bmp_image,
					t_bool with_alpha)
{
	int32_t					i;
	unsigned char			rgba[4];

	i = 0;
	while (i < (int32_t)(header->size - header->data_offset))
	{
		rgba[0] = bmp_image[i + bytes_per_pixel - 1];
		rgba[1] = bmp_image[i + bytes_per_pixel - 2];
		rgba[2] = bmp_image[i + bytes_per_pixel - 3];
		if (bytes_per_pixel == 4)
		{
			if (with_alpha)
				rgba[3] = bmp_image[bytes_per_pixel - 4];
			else
				rgba[3] = 255;
			bmp_image[i + 3] = rgba[3];
		}
		bmp_image[i] = rgba[0];
		bmp_image[i + 1] = rgba[1];
		bmp_image[i + 2] = rgba[2];
		i += bytes_per_pixel;
	}
}

static void		l3d_read_bmp_image(const char *filename, t_image *image_res,
					t_bool with_alpha)
{
	t_file_contents			*file;
	t_bmp_file_header		header;
	uint32_t				image_size;
	char					*bmp_image;
	void					*buf;


	file = read_file(filename);
	buf = file->buf;
	ft_memcpy(&header, buf, sizeof(header));
	error_check(header.file_type != 0x4D42, "Invalid bmp image");
	image_size = header.size - header.data_offset;
	error_check(!(bmp_image = malloc(sizeof(char) * image_size)),
		"Failed to malloc image pixels");
	ft_memcpy(bmp_image, buf + header.data_offset, image_size);
	image_res->bytes_per_pixel = header.bits_per_pixel / 8;
	l3d_bmp_colors_to_rgba(&header,
		image_res->bytes_per_pixel, bmp_image, with_alpha);
	image_res->width = header.width;
	image_res->height = header.height;
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
	int32_t		j;
	uint32_t	*pixels;

	l3d_read_bmp_image(filename, &image, false);
	error_check(!(pixels =
		malloc(sizeof(uint32_t) * image.width * image.height)),
		"Failed to malloc 32 bit pixels");
	i = 0;
	j = 0;
	while (i < (int32_t)(image.height * image.width))
	{
		ft_memset(pixels + i, 255, sizeof(uint32_t));
		ft_memcpy(pixels + i, image.pixels + j, image.bytes_per_pixel);
		// uint32_t lol[4];
		// l3d_u32_to_rgba(pixels[i], lol);
		// ft_printf("r %d g %d b %d a %d\n", lol[0], lol[1], lol[2], lol[3]);
		j += image.bytes_per_pixel;
		i++;
	}
	l3d_flip_image_rows(&image, pixels);
	*pixels_out = pixels;
	*width = image.width;
	*height = image.height;
	free(image.pixels);
}