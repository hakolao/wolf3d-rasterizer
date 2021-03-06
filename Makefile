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
# Linux and MacOS specific includes & libs
# Linux requires sdl2 installed
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
	SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image -lSDL2_ttf
	LIB_MATH = -lm
	LIB_PTHRTEAD = -lpthread
else
	SDL_FLAGS = -rpath $(LIBSDL2) \
					-framework SDL2 -F$(LIBSDL2)/ \
					-framework SDL2_image -F$(LIBSDL2)/ \
					-framework SDL2_ttf -F$(LIBSDL2)/
	SDL_INCLUDES = -I$(LIBSDL2)/SDL2.framework/Headers \
			-I$(LIBSDL2)/SDL2_image.framework/Headers \
			-I$(LIBSDL2)/SDL2_ttf.framework/Headers
endif
LIBS = $(LIB3DFLAGS) $(LIBGMATRIXFLAGS) $(LIBFTFLAGS) $(SDL_FLAGS) $(LIB_MATH) $(LIB_PTHRTEAD)

INCLUDES = -I ./include \
		-I$(LIBFT)/include \
		-I$(LIB3D)/include \
		-I$(LIBGMATRIX)/include \
		$(SDL_INCLUDES)

FLAGS = -Wall -Wextra -Werror -O3 -flto
SOURCES = main.c \
			wolf3d.c \
			player/player.c \
			player/collision.c \
			player/movement.c \
			camera.c \
			debug.c \
			scene/scene.c \
			scene/scene_generate.c \
			scene/cleanup.c \
			scene/assets.c \
			scene/scene_asset_files.c \
			scene/scene_data.c \
			map/map.c \
			map/render.c \
			map/minimap.c \
			map/minimap_player.c \
			render/render.c \
			render/prepare.c \
			render/prepare_utils1.c \
			render/prepare_utils2.c \
			render/prepare_utils3.c \
			render/rasterize.c \
			render/prepare_clip.c \
			render/ui.c \
			render/ui_main_game.c \
			window/text.c \
			window/window.c \
			window/frame.c \
			window/utils.c \
			events/mouse_state.c \
			events/menu_events.c \
			events/events.c \
			events/keyboard_state.c \
			events/general_input_events.c

MAP_EDITOR_NAME = wolf3d_editor
MAP_EDITOR_SOURCES = map_editor/map_editor.c \
			map_editor/map.c \
			map_editor/menu.c \
			map_editor/save_menu.c \
			map_editor/cells.c \
			map_editor/patterns1.c \
			map_editor/patterns2.c \
			map_editor/patterns3.c \
			map/map.c \
			map/render.c \
			window/text.c \
			window/window.c \
			window/frame.c \
			window/utils.c	\
			window/buttons/button_events.c \
			window/buttons/button_group_events.c \
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
	$(CC) -o $@ $^ $(LIBS) $(FLAGS)
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
	$(CC) -o $@ $^ $(LIBS) $(FLAGS)
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
