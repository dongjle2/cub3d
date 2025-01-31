#ifndef TYPEDEF_H
# define TYPEDEF_H
# include "../MLX/include/MLX42/MLX42.h"

typedef struct s_player
{
	unsigned int	x;
	unsigned int	y;
	double			dx;
	double			dy;
	double			angle;

	//implement camera_plane
	double			view_dirX;
	double			view_dirY;
	double			cam_planeX;
	double			cam_planeY;
	double			ray_dirX;
	double			ray_dirY;
	double			ray_offset_x;
	double			ray_offset_y;
	double			ra;
}					t_player;

typedef struct s_user_map
{
	int				map_width;
	int				map_height;
	double			pw; //(pixel_width = scale_factor_x)
	double			ph; //(pixel_height = scale_factor_y)
	char			*NO_texture;
	char			*SO_texture;
	char			*WE_texture;
	char			*EA_texture;
	int				*floor;
	int				*ceiling;
	char			**map_data;
	int				player_count;
	t_player		pos;
}					t_user_map;

typedef struct s_cub3d
{
	uint32_t		iwidth;
	uint32_t		iheight;
	mlx_t			*mlx;
	mlx_image_t		*img;
	mlx_image_t		*img2;
	mlx_texture_t	*texture[4];
	t_player		pos;
	t_user_map		map;
	int				key_pressed;
}					t_cub3d;

#endif