/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization_and_free.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 22:54:20 by hipham            #+#    #+#             */
/*   Updated: 2025/01/15 00:20:11 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void map3d_initialising(t_user_map *map)
{
	map->num_tiles_x = 0;
	map->num_tiles_y = 0;
	map->pw = 0;
	map->ph = 0;
	map->map_data = NULL;
	map->NO_texture = NULL;
	map->SO_texture = NULL;
	map->WE_texture = NULL;
	map->NO_texture = NULL;
	map->ceiling = NULL;
	map->floor = NULL;
	map->player_count = 0;
}

void ft_free_map(t_user_map map)
{
	if (map.map_data != NULL)
		ft_free(map.map_data);
	if (map.NO_texture != NULL)
		free(map.NO_texture);
	if (map.SO_texture != NULL)
		free(map.SO_texture);
	if (map.WE_texture != NULL)
		free(map.WE_texture);
	if (map.EA_texture != NULL)
		free(map.EA_texture);
	if (map.floor != NULL)
		free(map.floor);
	if (map.ceiling != NULL)
		free(map.ceiling);
}

void minimap_initilising(t_cub3d *data)
{
	t_minimap minimap;

	minimap.w = WIDTH * MINIMAP_SCALE / 100;
	minimap.tile_size = minimap.w / data->map.num_tiles_x; 
	minimap.h = data->map.num_tiles_y * minimap.tile_size;
	minimap.tile_w = minimap.w / data->map.num_tiles_x;
	minimap.tile_h = minimap.h / data->map.num_tiles_y;
	data->minimap = minimap;
}

void cub3d_initialising(t_cub3d *data)
{
	// data->iheight = data->map.num_tiles_y * cell_size;
	// data->iwidth = data->map.num_tiles_x * cell_size;
	data->pos.dx = data->map.pos.dx;
	data->pos.dy = data->map.pos.dy;
	// data->map.pw = data->iwidth / data->map.num_tiles_x;
	// data->map.ph = data->iheight / data->map.num_tiles_y;
	data->pos.x = data->map.pos.x * data->minimap.tile_w + data->minimap.tile_w / 2.0;
	data->pos.y = data->map.pos.y * data->minimap.tile_h + data->minimap.tile_h / 2.0;
	data->pos.angle = 0;
}
