/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 23:00:50 by hipham            #+#    #+#             */
/*   Updated: 2025/01/13 21:38:33 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

bool validate_texture_path(char *path)
{
	int fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("Error");
		return (0);
	}
	close(fd);
	return (1);
}

bool validate_maze(char **maze, t_user_map *map)
{
	char *last_1;
	int i;
	int longest_line;
	
	longest_line = 0;
	i = 0;
	while(maze[i] != NULL)
		i++;
	map->num_tiles_y = i;
	if (!check_for_wall(maze[0]) || !check_for_wall(maze[i - 1]))
		return (printf("Maze is not closed\n"), 0);
	i = -1;
	while (++i < map->num_tiles_y)
	{
		last_1 = ft_strrchr(maze[i], '1');
		last_1++;
		if (*last_1 != '\0')
			return (printf("Maze is not closed\n"), 0);
		if (!get_player(maze, i, &longest_line, map))
			return (0);
	}
	if (map->player_count != 1)
		return (0);
	map->num_tiles_x = longest_line;	
	return (1);	
}

// NO(0, -1); SO(0, 1); EA(1, 0); WE(-1,0) 
bool validate_player_pos(char c, unsigned int j, unsigned int i, t_user_map *map)
{
	if (ft_strchr("NSEW", c))
	{
		map->player_count += 1;
		map->pos.x = j;
		map->pos.y = i;
		if(c == 'N')
			parse_player_dir(0, -1, map);	
		else if(c == 'S')
			parse_player_dir(0, 1, map);	
		else if(c == 'E')
			parse_player_dir(1, 0, map);	
		else if(c == 'W')
			parse_player_dir(-1, 0, map);			
	}
	else if (ft_strchr("01 ", c))
		;
	else
		return (0);
	return (1);
}

bool validate_color_values(int *color)
{
	int i;

	i = 0;
	while (i < 3)
	{
		printf("%i, ", color[i]);
		if (color[i ] < 0 || color[i] > 255)
			return(0);
		i++;
	}
	if (i != 3)
		return (0);
	return (1);
}

bool validate_map(t_user_map *map)
{
	if (map->NO_texture == NULL || map->SO_texture == NULL 
	|| map->WE_texture == NULL || map->EA_texture == NULL
	|| map->ceiling == NULL || map->floor == NULL
	|| map->map_data == NULL)
		return (printf("Error: Invalid map\n"), 0);
	if (!validate_texture_path(map->NO_texture))
		return (printf("Error: Invalid texture\n"), 0);		
	if (!validate_texture_path(map->SO_texture))
		return (printf("Error: Invalid texture\n"), 0);		
	if (!validate_texture_path(map->WE_texture))
		return (printf("Error: Invalid texture\n"), 0);		
	if (!validate_texture_path(map->EA_texture))
		return (printf("Error: Invalid texture\n"), 0);		
	if (!validate_color_values(map->ceiling))
		return (printf("Error: Invalid color\n"),0);
	if (!validate_color_values(map->floor))
		return (printf("Error: Invalid color\n"),0);
	if (!validate_maze(map->map_data, map))
		return (printf("Error: Invalid maze\n"),0);
	return (1);
}
