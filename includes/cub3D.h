#ifndef CUB3D_H
#define CUB3D_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include "../includes/typedef.h"
#include "../MLX/include/MLX42/MLX42.h"
#include "../libft/includes/libft.h"
#include "../libft/includes/get_next_line.h"

#define WIDTH 1280
#define HEIGHT 640
#define cell_size 16

//initialization.c
void map_initialising(t_user_map *map);
void player_initilising(t_player *player);
void cub3d_initialising(t_cub3d *data);
void ft_free_map(t_user_map map);

//validation.c
bool validate_map(t_user_map *map);
bool validate_maze(char **maze, t_user_map *map);
bool validate_player_pos(char c, unsigned int j, unsigned int i, t_user_map *map);
bool validate_color_values(int *color);

//parsing.c
bool parsed_map(char *map_path, t_cub3d *data);

//cub3D_utils.c
int error_sms(char *s, int ret);
void ft_malloc(void **data, size_t size);
int get_rgba(int r, int g, int b, int a);

//parsing_utils.c
bool check_for_wall(char *line);
bool get_player(char **maze, int i, int *longest_line, t_user_map *map);
void parse_player_dir(int dx, int dy, t_user_map *map);
void convert_to_data(t_user_map *map, char *join);
void save_texture(char **dir, char *path);

#endif