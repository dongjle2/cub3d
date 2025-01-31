#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "../includes/cub3D.h"
#include "../MLX/include/MLX42/MLX42.h"
#include "../MLX/include/MLX42/MLX42_Int.h"
#include <assert.h>

// #define cell_size 32

void	rotate(t_cub3d *data, int unit_degree);
void	put_pixel_box(t_cub3d *data, u_int32_t color);
void	cast_ray(void *param);
void	draw_wall_slice(t_cub3d *data, int x, double distance_to_wall, int side, double rx, double ry);

// void map_initialising(t_user_map *map)
// {
// 	map->map_width = 0;
// 	map->map_height = 0;
// 	map->pw = 0;
// 	map->ph = 0;
// 	map->map_data = NULL;
// 	map->NO_texture = NULL;
// 	map->SO_texture = NULL;
// 	map->WE_texture = NULL;
// 	map->NO_texture = NULL;
// 	map->ceiling = NULL;
// 	map->floor = NULL;
// 	map->player_count = 0;
// }

// Exit the program as failure.
static void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

double	deg2rad(int a)
{
	return (a * M_PI / 180.0);
}

double rad2deg(double rad) 
{
    return rad * (180.0 / M_PI);
}

int	wall_collision(t_cub3d *data, int dir)
{
	int	x_offset;
	int	y_offset;

	if (0 < data->pos.dx)
		x_offset = 10;
	else
		x_offset = -10;
	if (0 < data->pos.dy)
		y_offset = 10;
	else
		y_offset = -10;
		
	int ipx = data->pos.x / data->map.pw;
	int ipy = data->pos.y / data->map.ph;
	int ipx_add_xo = (data->pos.x + x_offset) / data->map.pw;
	int ipy_add_yo = (data->pos.y + y_offset) / data->map.ph;
	int ipx_sub_xo = (data->pos.x - x_offset) / data->map.pw;
	int ipy_sub_yo = (data->pos.y - y_offset) / data->map.ph;
	if (dir == 1)	//W
	{
		// printf("%u, %u, %f, %f\n", data->pos.x, data->pos.y, data->pos.dx, data->pos.dy);
		char c = data->map.map_data[ipy][ipx_add_xo];
		if (c == '0' || ft_strchr("NSWE", c))
			data->pos.x += data->pos.dx * 4;
		char a = data->map.map_data[ipy_add_yo][ipx];
		if (a == '0' || ft_strchr("NSWE", a))
			data->pos.y += data->pos.dy * 4;
	}
	else if (dir == -1)	//S
	{
		char c = data->map.map_data[ipy][ipx_sub_xo];
		if (c == '0' || ft_strchr("NSWE", c))
			data->pos.x -= data->pos.dx * 4;
		char a = data->map.map_data[ipy_sub_yo][ipx];
		if (a == '0' || ft_strchr("NSWE", a))
			data->pos.y -= data->pos.dy * 4;
	}
	
	return (0);
}

void move_forward(t_cub3d *data, int dir)
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
	// else if (dir == -1)
	// {
	// 	data->pos.y -= data->pos.dy * 4;
	// 	data->pos.x -= data->pos.dx * 4;
	// }
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

double	adjust_angle(double angle)
{
	if (359 < angle)
		angle -= 360;
	if (angle < 0)
		angle += 360;
	return (angle);
}

void rotate(t_cub3d *data, int unit_degree)
{
	data->pos.angle = rad2deg(atan2(-data->pos.dy, data->pos.dx));
	data->pos.angle += unit_degree;
	data->pos.angle = adjust_angle(data->pos.angle);
	data->pos.dx = cos(deg2rad(data->pos.angle));
	data->pos.dy = -sin(deg2rad(data->pos.angle));
}

void	put_pixel_box(t_cub3d *data, u_int32_t color)
{
	// int scale_factor_x = data->img->width / data->map.map_width;
    // int scale_factor_y = data->img->height / data->map.map_height;
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

void render_map(char **map, t_cub3d *data) {
    int scale_factor_x = data->img->width / data->map.map_width;
    int scale_factor_y = data->img->height / data->map.map_height;
    int color;

    for (int i = 0; map[i] != NULL; i++) {
        for (int j = 0; map[i][j] != '\0'; j++) {
            if (map[i][j] == '1')
                color = 0xFF0000FF; // Wall: Blue
            else if (map[i][j] == '0' || ft_strchr("NSEW", map[i][j]))
                color = 0xFFFFFFFF; // Space: White
            else
                color = 0x00000000; // Default: Black (or transparent)
            int x_start = j * scale_factor_y + 1;
            int y_start = i * scale_factor_x + 1;
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


void draw_line(t_cub3d *data, double start_x, double start_y, double angle, double length, int color) {
	double end_x = start_x + length * cos(deg2rad(angle));
	double end_y = start_y + length * -sin(deg2rad(angle));

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
    double angle = data->pos.angle;
    double FOV = 60.0;
    double px = data->pos.x;
    double py = data->pos.y;
    int num_rays = 912;
    double ray_angle = FOV / num_rays;  // Step size between rays
    double start_angle = angle + (FOV / 2);  // Start from leftmost ray
    double rx, ry, xo, yo, disV, disH, disT;
    double dof, mx, my;
    int color;

    data->pos.ra = start_angle;
    int i = 0;
	render_map(data->map.map_data, data);
    while (i < num_rays) {
        disV = 1000000;
        disH = 1000000;
        double ca = adjust_angle( data->pos.ra - angle);  // Changed angle calculation
		
		// Vertical
		dof = 0;
		double tan_ra = tan(deg2rad(data->pos.ra));
		data->pos.ray_dirX = cos(deg2rad(data->pos.ra));
		if (data->pos.ray_dirX > 0.001) {
			rx = (((int)px / cell_size) * cell_size) + cell_size;
			ry = (px - rx) * tan_ra + py;
			xo = cell_size;
			yo = -xo * tan_ra;
		} else if (data->pos.ray_dirX < -0.001) {
			rx = (((int)px / cell_size) * cell_size) - 0.001;
			ry = (px - rx) * tan_ra + py;
			xo = -cell_size;
			yo = -xo * tan_ra;
		} else {
			rx = px;
			ry = py;
			dof = 8;
		}
		//printf("dof: %d\n", dof);
		//assert(dof < 8);
		while (dof < 8) {

			mx = (rx / cell_size);
			my = (ry / cell_size);
			if (mx >= 0 && my >= 0 && mx < data->map.map_width && my < data->map.map_height && 
				data->map.map_data[(int)my][(int)mx] == '1') {
				dof = 8;
				disV = sqrt(pow(rx - px, 2) + pow(ry - py, 2));
			} else {
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		double vx = rx, vy = ry;

		// Horizontal
		dof = 0;
		tan_ra = 1.0 / tan_ra;
		data->pos.ray_dirY = sin(deg2rad(data->pos.ra));
		if (data->pos.ray_dirY > 0.001) {
			ry = (((int)py / cell_size) * cell_size) - 0.001;
			rx = (py - ry) * tan_ra + px;
			yo = -cell_size;
			xo = -yo * tan_ra;
		} else if (sin(deg2rad(data->pos.ra)) < -0.001) {
			ry = (((int)py / cell_size) * cell_size) + cell_size;
			rx = (py - ry) * tan_ra + px;
			yo = cell_size;
			xo = -yo * tan_ra;
		} else {
			rx = px;
			ry = py;
			dof = 8;
		}

		while (dof < 8) {
			mx = (int)(rx / cell_size);
			my = (int)(ry / cell_size);
			if (mx >= 0 && my >= 0 && mx < data->map.map_width && my < data->map.map_height && 
				data->map.map_data[(int)my][(int)mx] == '1') {
				dof = 8;
				disH = sqrt(pow(rx - px, 2) + pow(ry - py, 2));
			} else {
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
        } else {
            disT = disH;
            color = 0;
        }

        double corrected_dist = disT * cos(deg2rad(ca));  // Keep fisheye correction

        // Draw wall slice first, then ray line
        draw_wall_slice(data, i, corrected_dist, color, rx, ry);
        draw_line(data, px, py, data->pos.ra, disT, 0x00FF00FF);
        
        data->pos.ra = adjust_angle(data->pos.ra - ray_angle);  // Changed to subtract ray_angle
        i++;
    }
}

// side = 1 -> SN, side = 0 -> EW

// void draw_wall_slice(t_cub3d *data, int x, double distance_to_wall, int side, double rx, double ry) {
//     printf("hit_x = %f, hit_y = %f\n", rx, ry);
//     int TEXTURE_HEIGHT = data->texture[2]->height;
//     int TEXTURE_WIDTH = data->texture[2]->width;

//     // Calculate wall height based on distance
//     int wall_height = (int)(((HEIGHT / 2) * cell_size) / distance_to_wall);
//     if (wall_height > HEIGHT) wall_height = HEIGHT;

//     int line_top = (HEIGHT - wall_height) / 2;
//     int line_bottom = line_top + wall_height;
//     double step = 1.0 * TEXTURE_HEIGHT / wall_height;
//     double texPos = 0;  // Start from the top

//     if (line_bottom >= HEIGHT) line_bottom = HEIGHT - 1;

//     int texX = (side == 0) ? 
//         (int)(rx / cell_size * TEXTURE_WIDTH) % TEXTURE_WIDTH :
//         (int)(ry / cell_size * TEXTURE_WIDTH) % TEXTURE_WIDTH;
//     if (texX < 0) texX += TEXTURE_WIDTH;

//     for (int y = line_top; y <= line_bottom; y++) {
//         int texY = (int)texPos % TEXTURE_HEIGHT;
//         uint8_t *pixel = &data->texture[2]->pixels[(TEXTURE_WIDTH * texY + texX) * 4];
//         uint32_t pixel_color = (pixel[0] << 24) | (pixel[1] << 16) | 
//                              (pixel[2] << 8) | pixel[3];

//         if (side == 0) {
//             uint32_t r = ((pixel_color >> 24) & 0xFF);
//             uint32_t g = ((pixel_color >> 16) & 0xFF);
//             uint32_t b = ((pixel_color >> 8) & 0xFF);
//             pixel_color = (r << 24) | (g << 16) | (b << 8) | 0xFF;
//         }
//         mlx_put_pixel(data->img2, x, y, pixel_color);
//         texPos += step;
//     }
// }

mlx_texture_t	*get_texture(t_cub3d *data, int side)
{
	printf("Side = %i, Ray angle: %f\n", side, data->pos.ra);

	data->pos.ra = adjust_angle(data->pos.ra);
	if (side == 0) 
	{
        if (data->pos.ra > (M_PI / 2) && data->pos.ra < (3 * (M_PI / 2))) 
			return(data->texture[1]); // east
		else
			return(data->texture[0]); //west
    } 
	else
	{
        if (data->pos.ra > 0 && data->pos.ra < M_PI) //south
			return(data->texture[3]);	 
		else // North
			return(data->texture[2]);	 
    }
}

void draw_wall_slice(t_cub3d *data, int x, double distance_to_wall, int side, double rx, double ry) {
    // printf("hit_x = %f, hit_y = %f\n", rx, ry);www

    // Select the texture based on the wall side (North, South, East, West)
    mlx_texture_t *texture = get_texture(data, side);

    // Calculate wall height based on distance
    int wall_height = (int)((HEIGHT) * cell_size / distance_to_wall);
    if (wall_height > HEIGHT) wall_height = HEIGHT;

    // Calculate the top and bottom of the wall slice
    int line_top = (HEIGHT - wall_height) / 2;
    int line_bottom = line_top + wall_height;
    double step = 1.0 * texture->height / wall_height;
    double texPos = 0;  // Start from the top of the texture

    if (line_bottom >= HEIGHT) line_bottom = HEIGHT - 1;

    // Texture X coordinate calculation (based on hit location)
    int texX = (side == 0) ? (int)(rx / cell_size * texture->width) % texture->width : 
                             (int)(ry / cell_size * texture->width) % texture->width;

    // Handle negative texX (wrap around)
    if (texX < 0) texX += texture->width;

    // Draw the wall slice using the selected texture
    for (int y = line_top; y <= line_bottom; y++) {
        int texY = (int)texPos % texture->height;

        // Get the pixel color from the selected texture
        uint8_t *pixel = &texture->pixels[(texY * texture->width + texX) * 4];  // RGBA format
        uint32_t pixel_color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];

        // Apply shading for side walls (darker effect for vertical walls)
        if (side == 0) {
            uint32_t r = ((pixel_color >> 24) & 0xFF);
            uint32_t g = ((pixel_color >> 16) & 0xFF);
            uint32_t b = ((pixel_color >> 8) & 0xFF);
            pixel_color = (r / 2 << 24) | (g / 2 << 16) | (b / 2 << 8) | 0xFF;  // Darken color for side walls
        }

        // Draw the pixel on the screen
        mlx_put_pixel(data->img2, x, y, pixel_color);
        texPos += step;  // Move down the texture
    }
}




int32_t	main(int ac, char *av[])
{
	char *path;
	t_cub3d	data;

	if (ac == 2)
	{
		path = av[1];
		map_initialising(&data.map);
		if (parsed_map(path, &data))
		{
			// printf("Width = %i, Height = %i\n", data.map.map_width, data.map.map_height);
			// MLX allows you to define its core behaviour before startup.
			// mlx_set_setting(MLX_MAXIMIZED, true);
			data.mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
			if (!data.mlx)
			{
				printf("Error: mlx_init failed\n");
				ft_error();
			}
			/* Do stuff */
			// Create and display the image.
			cub3d_initialising(&data);
			data.texture[0] = mlx_load_png(data.map.WE_texture);
			data.texture[1] = mlx_load_png(data.map.EA_texture);
			data.texture[2] = mlx_load_png(data.map.NO_texture);
			data.texture[3] = mlx_load_png(data.map.SO_texture);
			data.img = mlx_new_image(data.mlx, data.iwidth, data.iheight);
			if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
			{
				printf("Error: mlx_new_image failed\n");
				ft_error();
			}
			data.img2 = mlx_new_image(data.mlx, 912, 640);
			if (!data.img2 || (mlx_image_to_window(data.mlx, data.img2, data.map.map_width*32, 0) < 0))
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
