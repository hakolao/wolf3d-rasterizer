# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ohakola+veilo <ohakola+veilo@student.hi    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/29 14:50:34 by ohakola           #+#    #+#              #
#    Updated: 2020/12/05 15:32:30 by ohakola+vei      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
NAME = lib3d.a
LIBFT = ../libft
LIBGMATRIX = ../libgmatrix
DIR_SRC = ./
DIR_OBJ = temp
SOURCES = l3d_triangle.c \
			l3d_triangle_update.c \
			l3d_triangle_vector.c \
			l3d_ray.c \
			l3d_ray_hit.c \
			l3d_rand.c \
			l3d_obj_read.c \
			l3d_obj_read_utils1.c \
			l3d_obj_read_utils2.c \
			l3d_3d_object.c \
			l3d_3d_object_transform.c \
			l3d_kd_tree.c \
			l3d_kd_tree_node.c \
			l3d_kd_tree_utils.c \
			l3d_fminmax.c \
			l3d_aabb.c \
			l3d_kd_tree_aabb.c \
			l3d_pixel_plot.c \
			l3d_line_draw.c \
			l3d_image_place.c \
			l3d_color.c \
			l3d_image_read.c \
			l3d_triangle_raster.c \
			l3d_triangle_clipping.c \
			l3d_procedural_gen.c \
			l3d_buffers.c

SRCS = $(addprefix $(DIR_SRC)/,$(SOURCES))
OBJS = $(addprefix $(DIR_OBJ)/,$(SOURCES:.c=.o))
INCLUDES = -I ./include -I $(LIBFT)/include -I $(LIBGMATRIX)/include
FLAGS = -Wall -Wextra -Werror -O3

all: $(DIR_OBJ) $(NAME)

$(NAME): $(OBJS)
	@ar rc $(NAME) $(OBJS)

$(DIR_OBJ):
	@mkdir -p temp

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

clean:
	@/bin/rm -f $(OBJS)
	@/bin/rm -rf $(DIR_OBJ)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: all, $(DIR_OBJ), clean, fclean, re
