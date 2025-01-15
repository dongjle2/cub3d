#include "../MLX/include/MLX42/MLX42.h"
#include "../MLX/include/MLX42/MLX42_Int.h"
#include "../includes/cub3D.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void		rotate(t_cub3d *data, int unit_degree);
void		put_pixel_box(t_cub3d *data, u_int32_t color);
void		cast_ray(void *param);
void		draw_wall_slice(t_cub3d *data, int x, double distance_to_wall,
				int ca, int color);

// Exit the program as failure.
static void	ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

float	deg2rad(int a)
{
	return (a * M_PI / 180.0);
}

float	rad2deg(float rad)
{
	return (rad * (180.0 / M_PI));
}

int	wall_collision(t_cub3d *data, int dir)
{
	int		x_offset;
	int		y_offset;
	int		cnt;
	int		ipx;
	int		ipy;
	int		ipx_add_xo;
	int		ipy_add_yo;
	char	c;
	char	a;

	cnt = 0;
	if (0 < data->pos.dx)
		x_offset = 10;
	else
		x_offset = -10;
	if (0 < data->pos.dy)
		y_offset = 10;
	else
		y_offset = -10;
	ipx = data->pos.x / data->minimap.tile_w;
	ipy = data->pos.y / data->minimap.tile_h;
	ipx_add_xo = (data->pos.x + x_offset) / data->minimap.tile_w;
	ipy_add_yo = (data->pos.y + y_offset) / data->minimap.tile_h;
	// int ipx_sub_xo = (data->pos.x - x_offset) / data->map.scale_factor_x;
	// int ipy_sub_yo = (data->pos.y - y_offset) / data->map.scale_factor_y;
	if (dir == 1) //W
	{
		printf("%u, %u, %f, %f\n", data->pos.x, data->pos.y, data->pos.dx,
				data->pos.dy);
		c = data->map.map_data[ipy][ipx_add_xo];
		if (c == '0' || ft_strchr("NSWE", c))
		{
			data->pos.x += data->pos.dx * 4;
			cnt++;
		}
		a = data->map.map_data[ipy_add_yo][ipx];
		if (a == '0' || ft_strchr("NSWE", a))
		{
			data->pos.y += data->pos.dy * 4;
			cnt++;
		}
		// printf("%d\n", cnt);
		if (cnt <= 1)
			printf("collision\n");
	}
	return (0);
}

void	move_forward(t_cub3d *data, int dir)
{
	data->pos.angle = rad2deg(atan2(-data->pos.dy, data->pos.dx));
	if (wall_collision(data, dir))
	{
		return ;
	}
	//speed is 4
	// if (dir == 1)
	// {
	// 	data->pos.y += data->pos.dy * 4;
	// 	data->pos.x += data->pos.dx * 4;
	// }
	else if (dir == -1)
	{
		data->pos.y -= data->pos.dy * 4;
		data->pos.x -= data->pos.dx * 4;
	}
}

void	my_keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub3d	*data;
	mlx_t	*mlx;
	int		key_pressed;

	data = param;
	mlx = data->mlx;
	(void)keydata;
	key_pressed = false;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_W))
	{
		memset(data->img2->pixels, 255, data->img2->width * data->img2->height
				* sizeof(int32_t));
		put_pixel_box(data, 0xFFFFFFFF);
		move_forward(data, 1);
		key_pressed = true;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_S))
	{
		memset(data->img2->pixels, 255, data->img2->width * data->img2->height
				* sizeof(int32_t));
		put_pixel_box(data, 0xFFFFFFFF);
		move_forward(data, -1);
		key_pressed = true;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_A))
	{
		memset(data->img2->pixels, 255, data->img2->width * data->img2->height
				* sizeof(int32_t));
		put_pixel_box(data, 0xFFFFFFFF);
		rotate(data, 3.5);
		key_pressed = true;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_D))
	{
		memset(data->img2->pixels, 255, data->img2->width * data->img2->height
				* sizeof(int32_t));
		put_pixel_box(data, 0xFFFFFFFF);
		rotate(data, -3.5);
		key_pressed = true;
	}
	// 	if (x, y in range)
	put_pixel_box(data, 0xFF0000FF);
	if (key_pressed == true)
		cast_ray(data);
}

int	adjust_angle(int angle)
{
	if (359 < angle)
		angle -= 360;
	if (angle < 0)
		angle += 360;
	return (angle);
}

void	rotate(t_cub3d *data, int unit_degree)
{
	data->pos.angle = rad2deg(atan2(-data->pos.dy, data->pos.dx));
	data->pos.angle += unit_degree;
	data->pos.angle = adjust_angle(data->pos.angle);
	data->pos.dx = cos(deg2rad(data->pos.angle));
	data->pos.dy = -sin(deg2rad(data->pos.angle));
}

void	put_pixel_box(t_cub3d *data, u_int32_t color)
{
	const int	x_margin[7] = {-3, -2, -1, 0, 1, 2, 3};
	const int	y_margin[7] = {-3, -2, -1, 0, 1, 2, 3};
	size_t		i;
	size_t		j;
	size_t		dir;
	
	i = 0;
	while (i < sizeof(x_margin) / sizeof(int))
	{
		j = 0;
		while (j < sizeof(y_margin) / sizeof(int))
		{
			mlx_put_pixel(data->img, data->pos.x + x_margin[i], data->pos.y
					+ y_margin[j], color);
			j++;
		}
		i++;
	}
	dir = 1;
	while (dir < 13)
	{
		mlx_put_pixel(data->img, data->pos.x + data->pos.dx * dir, data->pos.y
				+ data->pos.dy * dir, color);
		dir++;
	}
}

void	render_map(char **map, t_cub3d *data)
{
	int	scale_factor_x;
	int	scale_factor_y;
	int	color;
	int	x_start;
	int	y_start;
	int	x_end;
	int	y_end;

	scale_factor_x = data->img->width / data->map.num_tiles_x;
	scale_factor_y = data->img->height / data->map.num_tiles_y;
	for (int i = 0; map[i] != NULL; i++)
	{
		for (int j = 0; map[i][j] != '\0'; j++)
		{
			if (map[i][j] == '1')
				color = 0xFF0000FF; // Wall: Blue
			else if (map[i][j] == '0' || ft_strchr("NSEW", map[i][j]))
				color = 0xFFFFFFFF; // Space: White
			else
				color = 0x00000000; // Default: Black (or transparent)
			x_start = j * scale_factor_y + 1;
			y_start = i * scale_factor_x + 1;
			x_end = x_start + scale_factor_x - 1;
			y_end = y_start + scale_factor_y - 1;
			for (int y = y_start; y < y_end; y++)
			{
				for (int x = x_start; x < x_end; x++)
				{
					mlx_put_pixel(data->img, x, y, color);
				}
			}
		}
	}
}

void	draw_line(t_cub3d *data, float start_x, float start_y, float angle,
		float length, int color)
{
	float	end_x;
	float	end_y;
	int		x1;
	int		y1;
	int		x2;
	int		y2;
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;

	end_x = start_x + length * cos(deg2rad(angle));
	end_y = start_y + length * -sin(deg2rad(angle));
	x1 = (int)start_x;
	y1 = (int)start_y;
	x2 = (int)end_x;
	y2 = (int)end_y;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	sx = x1 < x2 ? 1 : -1;
	sy = y1 < y2 ? 1 : -1;
	err = (dx > dy ? dx : -dy) / 2;
	while (1)
	{
		// Boundary check to prevent invalid memory access
		if (x1 >= 0 && x1 < (int)data->img->width && y1 >= 0
			&& y1 < (int)data->img->height)
		{
			mlx_put_pixel(data->img, x1, y1, color);
		}
		if (x1 == x2 && y1 == y2)
			break ;
		e2 = err;
		if (e2 > -dx)
		{
			err -= dy;
			x1 += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			y1 += sy;
		}
	}
}

void	cast_ray(void *param)
{
	t_cub3d	*data;
	int		color;
	float	ra;
	int		i;
	float	tan_ra;


	data = param;
	float rx, ry, xo, yo, disV, disH, disT;
	int dof, mx, my;
	render_map(data->map.map_data, data);
	ra = adjust_angle(data->pos.angle + 30);
	i = -1;
	while (i++ < 63)
	{
		disV = 100000;
		disH = 100000;
		// Vertical
		dof = 0;
		tan_ra = tan(deg2rad(ra));
		if (cos(deg2rad(ra)) > 0.001)
		{
			rx = (((int)data->pos.x / data->minimap.tile_size) * data->minimap.tile_size) + data->minimap.tile_size;
			ry = (data->pos.x - rx) * tan_ra + data->pos.y;
			xo = data->minimap.tile_size;
			yo = -xo * tan_ra;
		}
		else if (cos(deg2rad(ra)) < -0.001)
		{
			rx = (((int)data->pos.x / data->minimap.tile_size) * data->minimap.tile_size) - 0.0001;
			ry = (data->pos.x - rx) * tan_ra + data->pos.y;
			xo = -data->minimap.tile_size;
			yo = -xo * tan_ra;
		}
		else
		{
			rx = data->pos.x;
			ry = data->pos.y;
			dof = data->map.num_tiles_x;
		}
		while (dof < data->map.num_tiles_x)
		{
			mx = (int)(rx / data->minimap.tile_size);
			my = (int)(ry / data->minimap.tile_size);
			// int mp = my * data->map.map_width + mx;
			if (mx >= 0 && my >= 0 && mx < data->map.num_tiles_x
				&& my < data->map.num_tiles_y &&
				data->map.map_data[my][mx] == '1')
			{
				dof = data->map.num_tiles_x;
				disV = pow(rx - data->pos.x, 2) + pow(ry - data->pos.y, 2); //
			}
			else
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		float vx = rx;
		float vy = ry;
		// Horizontal
		dof = 0;
		tan_ra = 1.0 / tan_ra;
		if (sin(deg2rad(ra)) > 0.001)
		{
			ry = (((int)data->pos.y / data->minimap.tile_size) * data->minimap.tile_size) - 0.0001;
			rx = (data->pos.y - ry) * tan_ra + data->pos.x;
			yo = -data->minimap.tile_size;
			xo = -yo * tan_ra;
		}
		else if (sin(deg2rad(ra)) < -0.001)
		{
			ry = (((int)data->pos.y / data->minimap.tile_size) * data->minimap.tile_size) + data->minimap.tile_size;
			rx = (data->pos.y - ry) * tan_ra + data->pos.x;
			yo = data->minimap.tile_size;
			xo = -yo * tan_ra;
		}
		else
		{
			rx = data->pos.x;
			ry = data->pos.y;
			dof = data->map.num_tiles_x;
		}
		while (dof < data->map.num_tiles_x)
		{
			mx = (int)(rx / data->minimap.tile_size);
			my = (int)(ry / data->minimap.tile_size);
			// int mp = my * data->map.map_width + mx;
			if (mx >= 0 && my >= 0 && mx < data->map.num_tiles_x
				&& my < data->map.num_tiles_y &&
				data->map.map_data[my][mx] == '1')
			{
				dof = data->map.num_tiles_x;
				disH = pow(rx - data->pos.x, 2) + pow(ry - data->pos.y, 2); //
			}
			else
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		// Use closest intersection
		if (disV < disH)
		{
			rx = vx;
			ry = vy;
			disT = sqrt(disV);
			color = 1;
		}
		else
		{
			disT = sqrt(disH);
			color = 0;
		}
		draw_line(data, data->pos.x, data->pos.y, ra, disT, 0x00FF00FF);
		draw_wall_slice(data, i, disT, adjust_angle(data->pos.angle - ra), color);
		ra = adjust_angle(ra - 1);
	}
}

uint32_t	get_rgb_color(t_user_map map, char c, int color)
{
	if (c == 'C')
		return (get_rgba(map.ceiling[0], map.ceiling[1], map.ceiling[2], 255));
	if (c == 'F')
		return (get_rgba(map.floor[0], map.floor[1], map.floor[2], 60));
	if (c == 'W')
		return ((color == 0) ? 0x00FF00FF * 0.8 : 0x00FF00FF);
	return (0);
}

void	put_x_pixel_loop(int x, int y, t_cub3d *data, int color, char c)
{
	uint32_t	step;
	uint32_t	start_x;
	size_t		j;
	uint32_t	px;

	step = (data->img2->width / 60.0);
	start_x = step * x;
	j = 0;
	while (j < step)
	{
		px = start_x + j;
		if (px < data->img2->width)
		{
			mlx_put_pixel(data->img2, px, y, get_rgb_color(data->map, c,
						color));
		}
		j++;
	}
}

void	draw_wall_slice(t_cub3d *data, int x, double distance_to_wall, int ca,
		int color)
{
	int	wall_height;
	int	line_top;
	int	line_bottom;

	if (!data->img2)
		return ;
	// Correct distance for fish-eye effect
	distance_to_wall *= cos(deg2rad(ca));
	// Calculate wall height
	wall_height = (int)(6000 / distance_to_wall);
	// Determine wall slice bounds
	line_top = (HEIGHT / 2) - (wall_height / 2);
	line_bottom = (HEIGHT / 2) + (wall_height / 2);
	// Clip bounds to screen
	if (line_top < 0)
		line_top = 0;
	if (line_bottom >= HEIGHT)
		line_bottom = HEIGHT - 1;
	// Draw wall
	for (int y = line_top; y <= line_bottom; y++)
		put_x_pixel_loop(x, y, data, color, 'W');
	// Draw ceiling
	for (int y = 0; y < line_top; y++)
		put_x_pixel_loop(x, y, data, color, 'C');
	// Draw floor
	for (int y = line_bottom; y < HEIGHT; y++)
		put_x_pixel_loop(x, y, data, color, 'F');
}

int32_t	main(int ac, char *av[])
{
	char	*path;
	t_cub3d	data;

	if (ac == 2)
	{
		path = av[1];
		map3d_initialising(&data.map);
		if (parsed_map(path, &data))
		{
			printf("nbr. of y tiles = %i, nbr. of y tiles = %i\n",
					data.map.num_tiles_x, data.map.num_tiles_y);
			// MLX allows you to define its core behaviour before startup.
			// mlx_set_setting(MLX_MAXIMIZED, true);
			data.mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
			if (!data.mlx)
				ft_error();
			/* Do stuff */
			// Create and display the image.
			minimap_initilising(&data);
			cub3d_initialising(&data);
			// printf("mini map width = %i\n", data.minimap_w);
			data.img = mlx_new_image(data.mlx, data.minimap.w, data.minimap.h);
			if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0,
						0) < 0))
				ft_error();
			data.img2 = mlx_new_image(data.mlx, WIDTH - data.minimap.w, HEIGHT);
			if (!data.img2 || (mlx_image_to_window(data.mlx, data.img2,
						data.map.num_tiles_x * data.minimap.tile_size, 0) < 0))
				ft_error();
			// Even after the image is being displayed, we can still modify the buffer.
			render_map(data.map.map_data, &data);
			put_pixel_box(&data, 0xF000000F);
			// Register a hook and pass mlx as an optional param.
			// NOTE: Do this before calling mlx_loop!
			// mlx_loop_hook(data.mlx, ft_hook, &data);
			mlx_key_hook(data.mlx, &my_keyhook, &data);
			mlx_loop(data.mlx);
			mlx_terminate(data.mlx);
			ft_free_map(data.map);
		}
		else
			return (EXIT_SUCCESS);
	}
	else
		printf("Error: Invalid input!\n./cub3D [MAP.cub]\n");
	return (EXIT_SUCCESS);
}
