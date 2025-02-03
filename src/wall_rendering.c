/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_rendering.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 01:53:22 by hipham            #+#    #+#             */
/*   Updated: 2025/02/02 17:52:09 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

static uint32_t	get_pixel_color(mlx_texture_t *texture, uint32_t tex_x,
		uint32_t tex_y)
{
	uint8_t	*pixel;

	pixel = &texture->pixels[(texture->width * tex_y + tex_x) * 4];
	return ((pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3]);
}

static t_wall_data	calculate_wall_dimensions(t_cub3d *data, double distance)
{
	t_wall_data	wall_data;

	// wall_data.height = (int)(20 * HEIGHT / distance);
	wall_data.height = (int)(HEIGHT / 1.5 * data->cell_size / distance);
		// Adjust wall height calculation
	if (wall_data.height > HEIGHT)
		wall_data.height = HEIGHT;
	wall_data.step = 1.0 * (double)data->texture[3]->height
		/ (double)wall_data.height;
	wall_data.line_top = (HEIGHT - wall_data.height) / 2;
	wall_data.line_bottom = wall_data.line_top + wall_data.height;
	if (wall_data.line_bottom >= HEIGHT)
		wall_data.line_bottom = HEIGHT - 1;
	return (wall_data);
}

static uint32_t	get_texture_x(float pos, u_int32_t tex_width, float cell_size)
{
	float		relative_pos;
	uint32_t	tex_x;

	relative_pos = fmod(pos, cell_size);
	tex_x = (uint32_t)((relative_pos / cell_size) * tex_width);
	tex_x %= tex_width;
	// return ((tex_x < 0) ? tex_x + tex_width : tex_x);
	return (tex_x);
}

mlx_texture_t	*get_wall_texture(t_cub3d *data, t_ray_data *ray)
{
	if (ray->color == 0)
	{
		if (ray->dirX >= 0)
			return (data->texture[2]); // North
		else
			return (data->texture[3]); // South
	}
	else
	{
		if (ray->dirY >= 0)
			return (data->texture[1]); // East
		else
			return (data->texture[0]); // West
	}
}

int get_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void floor_drawing(t_cub3d *data, t_wall_data wall, int x)
{
    uint32_t floor_color;

    floor_color = get_rgba(data->map.floor[0], data->map.floor[1], data->map.floor[2], 200);
    while (wall.line_bottom < HEIGHT)
    {
        mlx_put_pixel(data->img2, x, wall.line_bottom, floor_color);
        wall.line_bottom++;
    }
}

void ceiling_drawing(t_cub3d *data, t_wall_data wall, int x)
{
    uint32_t ceiling_color;
    int i;
    
    i = 0;
    ceiling_color = get_rgba(data->map.ceiling[0], data->map.ceiling[1], data->map.ceiling[2], 155);
    while (i < wall.line_top)
    {
        mlx_put_pixel(data->img2, x, i, ceiling_color);
        i++;
    }
}

void	draw_wall_slice(t_cub3d *data, int x, double distance_to_wall,
		t_ray_data *ray)
{
	t_wall_data	wall;
	uint32_t	tex_x;
	double		tex_pos;
	uint32_t	tex_y;
	uint32_t	pixel_color;
	uint32_t	r;
	uint32_t	g;
	uint32_t	b;

	wall = calculate_wall_dimensions(data, distance_to_wall);
	wall.texture = get_wall_texture(data, ray);
	tex_x = get_texture_x(ray->color ? ray->hit_y : ray->hit_x,
			wall.texture->width, data->cell_size);
	tex_pos = 0;
	for (int y = wall.line_top; y <= wall.line_bottom; y++)
	{
		tex_y = (uint32_t)tex_pos % wall.texture->height;
		tex_y = (tex_y >= wall.texture->height) ? wall.texture->height
			- 1 : tex_y;
		pixel_color = get_pixel_color(wall.texture, tex_x, tex_y);
		if (!ray->color)
		{
			r = ((pixel_color >> 24) & 0xFF);
			g = ((pixel_color >> 16) & 0xFF);
			b = ((pixel_color >> 8) & 0xFF);
			pixel_color = (r << 24) | (g << 16) | (b << 8) | 0xFF;
		}
		mlx_put_pixel(data->img2, x, y, pixel_color);
		tex_pos += wall.step;
	}
    floor_drawing(data, wall, x);
    ceiling_drawing(data, wall, x);
}
