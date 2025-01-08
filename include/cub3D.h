#ifndef CUB3D_H
#define CUB3D_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include "../include/typedef.h"
#include "../MLX/include/MLX42/MLX42.h"
#include "../libft/includes/libft.h"
#include "../libft/includes/get_next_line.h"

#define WIDTH 1280
#define HEIGHT 640
#define cell_size 8

bool parsed_map(char *map_path, t_user_map data);

#endif