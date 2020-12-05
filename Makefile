CC = gcc
NAME=wolf3d
DIR_SRC = ./src
DIR_OBJ = temp
LIBSDL2 = ./lib/SDL2
LIBFT = ./lib/libft
LIB3D = ./lib/lib3d
LIBGMATRIX = ./lib/libgmatrix

LIBFTFLAGS = -L$(LIBFT) -lft
LIB3DFLAGS = -L$(LIB3D) -l3d
LIBGMATRIXFLAGS = -L$(LIBGMATRIX) -lgmatrix
SDL_FLAGS = -rpath $(LIBSDL2) \
				-framework SDL2 -F$(LIBSDL2)/ \
				-framework SDL2_image -F$(LIBSDL2)/ \
				-framework SDL2_ttf -F$(LIBSDL2)/
LIBS = $(LIBFTFLAGS) $(LIB3DFLAGS) $(LIBGMATRIXFLAGS) $(SDL_FLAGS)
INCLUDES = -I ./include \
		-I$(LIBFT)/include \
		-I$(LIB3D)/include \
		-I$(LIBGMATRIX)/include \
		-I$(LIBSDL2)/SDL2.framework/Headers \
		-I$(LIBSDL2)/SDL2_image.framework/Headers \
		-I$(LIBSDL2)/SDL2_ttf.framework/Headers

FLAGS = -Wall -Wextra -Werror -O3
SOURCES = main.c \
			wolf3d.c \
			player/player.c \
			player/collision.c \
			player/movement.c \
			camera.c \
			time.c \
			debug.c \
			scene/scene.c \
			scene/scene_generate.c \
			scene/cleanup.c \
			scene/assets.c \
			scene/scene_asset_files.c \
			scene/scene_data.c \
			map/map.c \
			map/render.c \
			render/render.c \
			render/prepare.c \
			render/rasterize.c \
			render/prepare_clip.c \
			render/render_utils.c \
			render/ui.c \
			window/text.c \
			window/window.c \
			window/utils.c \
			events/mouse.c \
			events/menu_events.c \
			events/events.c \
			events/keyboard.c

MAP_EDITOR_NAME = wolf3d_editor
MAP_EDITOR_SOURCES = map_editor/map_editor.c \
			map_editor/map.c \
			map_editor/menu.c \
			map_editor/cells.c \
			map_editor/patterns.c \
			map/map.c \
			map/render.c \
			window/text.c \
			window/window.c \
			window/utils.c	\
			window/buttons/button_events.c \
			window/buttons/button_group_utils.c \
			window/buttons/button_group.c \
			window/buttons/button_utils.c \
			window/buttons/button.c


OBJS = $(addprefix $(DIR_OBJ)/,$(SOURCES:.c=.o))
MAP_EDITOR_OBJS = $(addprefix $(DIR_OBJ)/,$(MAP_EDITOR_SOURCES:.c=.o))

all: $(DIR_OBJ) $(NAME)

$(NAME): $(OBJS)
	@make libs
	@printf "\033[32;1mCompiling app...\n\033[0m"
	$(CC) $(FLAGS) $(LIBS) -o $@ $^
	@printf "\033[32;1mDone. Run: ./$(NAME)\n\033[0m"

libs:
	@printf "\033[32;1mCompiling libs...\n\033[0m"
	make -C $(LIBFT)
	make -C $(LIB3D)
	make -C $(LIBGMATRIX)

map_editor: $(DIR_OBJ) $(MAP_EDITOR_NAME)

$(MAP_EDITOR_NAME): $(MAP_EDITOR_OBJS)
	@make libs
	@printf "\033[32;1mCompiling map_editor...\n\033[0m"
	$(CC) $(FLAGS) $(LIBS) -o $@ $^
	@printf "\033[32;1mDone. Run: ./$(MAP_EDITOR_NAME)\n\033[0m"

$(DIR_OBJ):
	@mkdir -p temp
	@mkdir -p temp/map
	@mkdir -p temp/scene
	@mkdir -p temp/window
	@mkdir -p temp/window/buttons
	@mkdir -p temp/render
	@mkdir -p temp/events
	@mkdir -p temp/map_editor
	@mkdir -p temp/player

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

clean:
	@make -C $(LIBFT) clean
	@make -C $(LIB3D) clean
	@make -C $(LIBGMATRIX) clean
	@/bin/rm -f $(OBJS)
	@/bin/rm -f $(MAP_EDITOR_OBJS)
	@/bin/rm -rf $(DIR_OBJ)

fclean: clean
	@make -C $(LIBFT) fclean
	@make -C $(LIB3D) fclean
	@make -C $(LIBGMATRIX) fclean
	@/bin/rm -f $(NAME)
	@/bin/rm -f $(MAP_EDITOR_NAME)

re: fclean all

testrun: #this is only for quicker lib3d debug
	#@make -C $(LIB3D) clean
	#make -C $(LIB3D)
	@/bin/rm -f $(OBJS)
	@/bin/rm -rf $(DIR_OBJ)
	make all
	./wolf3d

norm: norminette $(DIR_SRC) $(LIBFT) $(LIB3D) $(LIBGMATRIX) ./include

.PHONY: all, $(DIR_OBJ), clean, fclean
