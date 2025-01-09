#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "../includes/cub3D.h"
#include "../MLX/include/MLX42/MLX42.h"
#include "../MLX/include/MLX42/MLX42_Int.h"

#define cell_size 64
#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64

void	rotate(t_cub3d *data, int unit_degree);
void	put_pixel_box(t_cub3d *data, u_int32_t color);
void	cast_ray(void *param);
void	draw_wall_slice(t_cub3d *data, int x, double distance_to_wall, int ca, int color);

int	texture[TEXTURE_WIDTH * TEXTURE_HEIGHT];

// Exit the program as failure.
static void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

float	deg2rad(int a)
{
	return (a * M_PI / 180.0);
}

void generate_texture(uint32_t *texture) {
    // Define the color for the brown brick (e.g., RGBA: 139, 69, 19, 255)
    uint32_t brown_brick_color = 0x8B4513FF; // Brown color with full opacity

    for (int y = 0; y < TEXTURE_HEIGHT; y++) {
        for (int x = 0; x < TEXTURE_WIDTH; x++) {
            texture[y * TEXTURE_WIDTH + x] = brown_brick_color;
        }
    }
}

int	wall_collision(t_cub3d *data, int dir)
{
	int	x_offset;
	int	y_offset;
	int	cnt;

	cnt = 0;
	if (0 < data->pos.dx)
		x_offset = 10;
	else
		x_offset = -10;
	if (0 < data->pos.dy)
		y_offset = 10;
	else
		y_offset = -10;
	int ipx = data->pos.x / data->map.scale_factor_x;
	int ipy = data->pos.y / data->map.scale_factor_y;
	int ipx_add_xo = (data->pos.x + x_offset) / data->map.scale_factor_x;
	int ipy_add_yo = (data->pos.y + y_offset) / data->map.scale_factor_y;
	// int ipx_sub_xo = (data->pos.x - x_offset) / data->map.scale_factor_x;
	// int ipy_sub_yo = (data->pos.y - y_offset) / data->map.scale_factor_y;
	if (dir == 1)	//W
	{
		// printf("%u, %u, %f, %f\n", data->pos.x, data->pos.y, data->pos.dx, data->pos.dy);
		if (data->map.map_data[ipy][ipx_add_xo] == '0')
		{
			data->pos.x += data->pos.dx * 4;
			cnt++;
		}
		if (data->map.map_data[ipy_add_yo][ipx] == '0')
		{
			data->pos.y += data->pos.dy * 4;
			cnt++;
		}
		// printf("%d\n", cnt);
		// if (cnt <= 1)
		// 	printf("collision\n");
	}
	
	return (0);
}

void move_forward(t_cub3d *data, int dir)
{
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

void my_keyhook(mlx_key_data_t keydata, void* param)
{
	t_cub3d	*data = param;
	mlx_t* mlx = data->mlx;
	(void) keydata;
	int	key_pressed;

	key_pressed = false;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_W))
	{
		memset(data->img2->pixels, 255, data->img2->width * data->img2->height * sizeof(int32_t));
		put_pixel_box(data, 0xFFFFFFFF);
		move_forward(data, 1);
		key_pressed = true;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_S))
	{
		memset(data->img2->pixels, 255, data->img2->width * data->img2->height * sizeof(int32_t));
		put_pixel_box(data, 0xFFFFFFFF);
		move_forward(data, -1);
		key_pressed = true;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_A))
	{
		memset(data->img2->pixels, 255, data->img2->width * data->img2->height * sizeof(int32_t));
		put_pixel_box(data, 0xFFFFFFFF);
		rotate(data, 5);
		key_pressed = true;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_D))
	{
		memset(data->img2->pixels, 255, data->img2->width * data->img2->height * sizeof(int32_t));
		put_pixel_box(data, 0xFFFFFFFF);
		rotate(data, -5);
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

void rotate(t_cub3d *data, int unit_degree)
{
	data->pos.angle += unit_degree;
	data->pos.angle = adjust_angle(data->pos.angle);
	data->pos.dx = cos(deg2rad(data->pos.angle));
	data->pos.dy = -sin(deg2rad(data->pos.angle));
}

void	put_pixel_box(t_cub3d *data, u_int32_t color)
{
	const int x_margin[7] = {-3, -2, -1, 0 ,1, 2, 3};
	const int y_margin[7] = {-3, -2, -1, 0 ,1, 2, 3};
	size_t	i;
	size_t	j;
	size_t	dir;

	i = 0;
	while (i < sizeof(x_margin) / sizeof(int))
	{
		j = 0 ;
		while (j < sizeof(y_margin) / sizeof(int))
		{
			mlx_put_pixel(data->img, data->pos.x + x_margin[i], data->pos.y + y_margin[j], color);
			j++;
		}
		i++;
	}
	dir = 1;
	while (dir < 13)
	{
		mlx_put_pixel(data->img, data->pos.x + data->pos.dx * dir, data->pos.y + data->pos.dy * dir, color);
		dir++;
	}
}

void render_map(char **map, int map_width, int map_height, int img_width, int img_height, t_cub3d *data) {
	int scale_factor_x = img_width / map_width;
	int scale_factor_y = img_height / map_height;
	int color;

	for (int i = 0; i < map_height; i++) {
		for (int j = 0; j < map_width; j++) {
			if (map[i][j] == '1')
				color = 0xFF0000FF; // Wall: Blue
			else if (map[i][j] == '0')
				color = 0xFFFFFFFF; // Space: White
			else
				color = 0x00000000; // Default: Black (or transparent)

			int x_start = j * scale_factor_x + 1;
			int y_start = i * scale_factor_y + 1;
			int x_end = x_start + scale_factor_x - 1;
			int y_end = y_start + scale_factor_y - 1;

			for (int y = y_start; y < y_end; y++) {
				for (int x = x_start; x < x_end; x++) {
					mlx_put_pixel(data->img, x, y, color);
				}
			}
		}
	}
}


void draw_line(t_cub3d *data, float start_x, float start_y, float angle, float length, int color) {
	float end_x = start_x + length * cos(deg2rad(angle));
	float end_y = start_y + length * -sin(deg2rad(angle));

	int x1 = (int)start_x;
	int y1 = (int)start_y;
	int x2 = (int)end_x;
	int y2 = (int)end_y;

	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = x1 < x2 ? 1 : -1;
	int sy = y1 < y2 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int e2;

	while (1) {
		mlx_put_pixel(data->img, x1, y1, color);
		if (x1 == x2 && y1 == y2)
			break;
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x1 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y1 += sy;
		}
	}
}

void cast_ray(void *param) {
	t_cub3d *data = param;
	float angle = data->pos.angle;
	float px = data->pos.x;
	float py = data->pos.y;
	float rx, ry, xo, yo, disV, disH, disT;
	int dof, mx, my;
	int	color;

	render_map(data->map.map_data, data->map.map_width, data->map.map_height, 512, 512, data);
	float ra = adjust_angle(angle + 30);
	int i = 0;
	while (i++ < 60)
	{
		disV = 100000;
		disH = 100000;
		// Vertical
		dof = 0;
		float tan_ra = tan(deg2rad(ra));
		if (cos(deg2rad(ra)) > 0.001) {
			rx = (((int)px/cell_size) * cell_size) + cell_size;
			ry = (px-rx) * tan_ra + py;
			xo = cell_size;
			yo = -xo * tan_ra;
		}
		else if (cos(deg2rad(ra)) < -0.001) {
			rx = (((int)px/cell_size) * cell_size) - 0.0001;
			ry = (px-rx) * tan_ra + py;
			xo = -cell_size;
			yo = -xo * tan_ra;
		}
		else {
			rx = px;
			ry = py;
			dof = 8;
		}

		while (dof < 8) {
			mx = (int)(rx/cell_size);
			my = (int)(ry/cell_size);
			// int mp = my * data->map.map_width + mx;
			if (mx >= 0 && my >= 0 && mx < data->map.map_width && my < data->map.map_height && 
				data->map.map_data[my][mx] == '1') {
				dof = 8;
				disV = sqrt(pow(rx - px, 2) + pow(ry - py, 2));	//
			}
			else {
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		float vx = rx, vy = ry;

		// Horizontal
		dof = 0;
		tan_ra = 1.0/tan_ra;
		if (sin(deg2rad(ra)) > 0.001) {
			ry = (((int)py/cell_size) * cell_size) - 0.0001;
			rx = (py-ry) * tan_ra + px;
			yo = -cell_size;
			xo = -yo * tan_ra;
		}
		else if (sin(deg2rad(ra)) < -0.001) {
			ry = (((int)py/cell_size) * cell_size) + cell_size;
			rx = (py-ry) * tan_ra + px;
			yo = cell_size;
			xo = -yo * tan_ra;
		}
		else {
			rx = px;
			ry = py;
			dof = 8;
		}

		while (dof < 8) {
			mx = (int)(rx/cell_size);
			my = (int)(ry/cell_size);
			// int mp = my * data->map.map_width + mx;
			if (mx >= 0 && my >= 0 && mx < data->map.map_width && my < data->map.map_height && 
				data->map.map_data[my][mx] == '1') {
				dof = 8;
				disH = sqrt(pow(rx - px, 2) + pow(ry - py, 2));	//
			}
			else {
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}

		// Use closest intersection
		if (disV < disH) {
			rx = vx;
			ry = vy;
			disT = disV;
			color = 1;
		}
		else
		{
			disT = disH;
			color = 0;
		}
		// printf("Closest intersection at (%f, %f) with distance %f\n", rx, ry, disT);
		// Draw the ray line from player position to hit point
		// float ray_length = sqrt(pow(rx - px, 2) + pow(ry - py, 2));
		draw_line(data, px, py, ra, disT, 0x00FF00FF);
		draw_wall_slice(data, i, disT, adjust_angle(angle-ra), color);
		// draw_wall_slice(data, i, disT, adjust_angle(ra - angle));
		ra = adjust_angle(ra - 1);
	}
}

void draw_wall_slice(t_cub3d *data, int x, double distance_to_wall, int ca, int color) {
	distance_to_wall *= cos(deg2rad(ca));
	// Calculate wall height
	int wall_height = (int)(6000 / distance_to_wall);
	double step = 1.0 * TEXTURE_HEIGHT / wall_height;
	// Calculate start and end of the vertical line
	int line_top = (HEIGHT / 2) - (wall_height / 2);
	int line_bottom = (HEIGHT / 2) + (wall_height / 2);

	// Clip to screen bounds
	if (line_top < 0) line_top = 0;
	if (line_bottom >= HEIGHT) line_bottom = HEIGHT - 1;

	double texPos = (line_top - HEIGHT / 2 + wall_height / 2) * step;
	// printf("%d %d\n", line_top, line_bottom);
	int texX = (int)(x * TEXTURE_WIDTH);
	// int texY = (int)texPos && (TEXTURE_HEIGHT - 1);	//why %?
	// Calculate wall color based on distance (e.g., darker for farther walls)
	// int color = calculate_color(distance_to_wall);

	// Draw the vertical line one pixel at a time
	for (int y = line_top; y <= line_bottom; y++) {
		// int texY = (int)texPos && (TEXTURE_HEIGHT - 1);	//why %?
		int texY = (int)texPos;	//why %?
		texPos += step;
		printf("%d %d\n", texX, texY);
		printf("%d\n", texture[TEXTURE_HEIGHT * texY + texX]);
		for (int j = 0; j < 8; j++)
		{
			if (color == 0)
				mlx_put_pixel(data->img2, 8 * x + j, y, texture[TEXTURE_HEIGHT * texY + texX] * 0.8); // Draw a single pixel
			else
				mlx_put_pixel(data->img2, 8 * x + j, y, texture[TEXTURE_HEIGHT * texY + texX]); // Draw a single pixel
		}
	}
}

int32_t	main(int ac, char *av[])
{
	char *path;
	t_cub3d	data;
	// int map_input[] = {1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1};
	char *map_input[] = {
	" 1111111",
	" 1010001",
	"10010101",
	"10000111",
	"10000001",
	"1010001",
	"10111001",
	"11  1111"
	};//		   |
	//init_fuction V
	data.map.map_data = malloc(sizeof(map_input));
	ft_memcpy(data.map.map_data, map_input, sizeof(map_input));
	data.map.map_width = 8;
	data.map.map_height = 8;
	data.map.scale_factor_x = 512 / 8;
	data.map.scale_factor_y = 512 / 8;
	data.pos.x = 300;
	data.pos.y = 300;
	data.pos.dx = 1;
	data.pos.dy = 0;
	data.pos.angle = 0;

	if (ac == 2)
	{
		path = av[1];
		if (parsed_map(path, data.map))
		{
			// MLX allows you to define its core behaviour before startup.
			// mlx_set_setting(MLX_MAXIMIZED, true);
			data.mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
			if (!data.mlx)
				ft_error();

			/* Do stuff */
			// Create and display the image.
			data.img = mlx_new_image(data.mlx, 512, 512);
			if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
				ft_error();
			data.img2 = mlx_new_image(data.mlx, 512, 512);
			if (!data.img2 || (mlx_image_to_window(data.mlx, data.img2, 515, 0) < 0))
				ft_error();

			// Even after the image is being displayed, we can still modify the buffer.
			render_map(data.map.map_data, 8, 8, 512, 512, &data);
			put_pixel_box(&data, 0xF000000F);

			// Register a hook and pass mlx as an optional param.
			// NOTE: Do this before calling mlx_loop!
			// mlx_loop_hook(data.mlx, ft_hook, &data);
			mlx_key_hook(data.mlx, &my_keyhook, &data);
			mlx_loop(data.mlx);
			mlx_terminate(data.mlx);
		}
		else
			printf("Invalid MAP.\n");
	}
	else
		printf("Invalid input.\n./cub3D [MAP.cub]\n");
	return (EXIT_SUCCESS);
}
