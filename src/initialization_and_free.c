/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization_and_free.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 22:54:20 by hipham            #+#    #+#             */
/*   Updated: 2025/01/31 21:19:32 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void map_initialising(t_user_map *map)
{
	map->map_width = 0;
	map->map_height = 0;
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

// void player_initilising(t_player *player)
// {
// 	player->angle = 0;
// 	player->dx = 0;
// 	player->dy = 0;
// 	player->x = 0;
// 	player->y = 0;
// }

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

void cub3d_initialising(t_cub3d *data)
{
	data->iheight = data->map.map_height * cell_size;
	data->iwidth = data->map.map_width * cell_size;
	data->pos.dx = data->map.pos.dx;
	data->pos.dy = data->map.pos.dy;
	data->map.pw = data->iwidth / data->map.map_width;
	data->map.ph = data->iheight / data->map.map_height;
	data->pos.x = data->map.pos.x * data->map.pw + data->map.pw / 2.0;
	data->pos.y = data->map.pos.y * data->map.ph + data->map.ph / 2.0;
	data->pos.angle = 0;
}
