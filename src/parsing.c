/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 21:46:15 by hipham            #+#    #+#             */
/*   Updated: 2025/01/10 22:37:51 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void parse_color(char *line, t_user_map *map)
{
	char **color;
	int color_arr[3];
	int i;
	char **arr;
	
	arr = ft_split(line, ' ');
	i = 0;
	while(arr[i] != NULL)
		i++;
	if (i == 2)
	{
		i = -1;
		color = ft_split(arr[1], ',');
		while (color[++i])
			color_arr[i] = ft_atoi(color[i]);
		if (!ft_strcmp(arr[0], "C"))
		{
			ft_malloc((void**)&map->ceiling, sizeof(color_arr));
			ft_memcpy(map->ceiling, color_arr, sizeof(color_arr));
		}
		if (!ft_strcmp(arr[0], "F"))
		{
			ft_malloc((void**)&map->floor, sizeof(color_arr));
			ft_memcpy(map->floor, color_arr, sizeof(color_arr));
		}
		ft_free(color);
		ft_free(arr);
	}
}

void parse_texture(char *line, t_user_map *map)
{
	char **arr;
	int i;

	i = 0;
	arr = ft_split(line, ' ');
	while(arr[i] != NULL)
		i++;
	if (i == 2)
	{
		if (ft_strcmp(arr[0], "NO") == 0)
			save_texture(&map->NO_texture, arr[1]);
		else if (ft_strcmp(arr[0], "SO") == 0)
			save_texture(&map->SO_texture, arr[1]);
		else if (ft_strcmp(arr[0], "WE") == 0)
			save_texture(&map->WE_texture, arr[1]);
		else if (ft_strcmp(arr[0], "EA") == 0)
			save_texture(&map->EA_texture, arr[1]);
	}
	ft_free(arr);
}

bool parsed_maze(char **line, int fd, t_user_map *map)
{
    char *tmp;
    char *join;

	join = ft_strdup("");
    while (*line)
    {
        tmp = *line;
        while (*tmp != '\0' && ft_isspace(*tmp))
            tmp++;
        if (*tmp == '1')
        {
            tmp = join;
            join = ft_strjoin(join, *line);
			free(tmp);
        }
        else
        {
            if (join != NULL)
                free(join);
			return(free(*line), 0);
        }        
        free(*line);
        *line = get_next_line(fd);
    }
	return (convert_to_data(map, join), 1);
}

bool map_parsing(int fd, t_user_map *map)
{
	char *line;
	char *tmp;

	line = get_next_line(fd);
	while (line)
	{
		tmp = line;
		while(*tmp != '\0' && ft_isspace(*tmp))
			tmp++;
		if (*tmp == '\0')
			;
		else if (*tmp != '\0' && (*tmp == 'F' || *tmp == 'C'))
			parse_color(tmp, map);
		else if (*tmp != '\0' && ft_strchr("NSWE", *tmp))
			parse_texture(tmp, map);
		else if (*tmp != '\0' && *tmp == '1')
		{
			if (parsed_maze(&line, fd, map) == 0)
				return (0);
			else
				return (1);
		}
		else
			return(printf("Error: Invalid maze\n"),free(line), 0);
		free(line);
		line = get_next_line(fd);
	}
	return(1);			
}

bool parsed_map(char *map_path, t_cub3d *data)
{
	int fd;

	fd = open(map_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Error: Invalid path!");
		return (0);
	}
	if (map_parsing(fd, &data->map))
	{
		if (!validate_map(&data->map))
		{
			ft_free_map(data->map);
			return(0);
		}	
	}
	else
		return(0);
	
	return(1);
}
