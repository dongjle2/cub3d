#ifndef TYPEDEF_H
# define TYPEDEF_H
# include "../MLX/include/MLX42/MLX42.h"

typedef struct s_player
{
	unsigned int	x;
	unsigned int	y;
	float			dx;
	float			dy;
	float			angle;
}					t_player;

typedef struct s_user_map
{
	int				map_width;
	int				map_height;
	float			pw; //(pixel_width = scale_factor_x)
	float			ph; //(pixel_height = scale_factor_y)
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
	uint32_t		minimap_w;
	int				cell_size;
	uint32_t		iwidth;
	uint32_t		iheight;
	mlx_t			*mlx;
	mlx_image_t		*img;
	mlx_image_t		*img2;
	t_player		pos;
	t_user_map		map;
	int				key_pressed;
}					t_cub3d;

#endif