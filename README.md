# cub3D

A small 3D ray‑casting engine inspired by Wolfenstein 3D, written in C for the 42 curriculum. It parses a `.cub` map, loads textures, and renders a first‑person view using MLX42.

This repository includes:
- MLX42 (MiniLibX) under `MLX/` (built via CMake)
- A custom C standard library, `libft/`
- Source under `src/mandatory` and `src/bonus`
- Headers in `include/`
- Example maps in `maps/`
- Image assets in `images/`

## Features

- Ray casting wall rendering
- Texture mapping (N/S/E/W)
- Player movement and rotation
- Map parsing and validation
- Minimap overlay
- Weapon overlay/animation

Exact feature set depends on whether you build the mandatory or the bonus target.

## Requirements

- Unix-like OS (Linux or macOS)
- C toolchain (cc/gcc/clang) and make
- CMake (for building MLX42)
- GLFW (required by MLX42)
- OpenGL drivers
- Standard system libs used by MLX42: dl, pthread, m

Install GLFW:
- macOS: `brew install glfw`
- Ubuntu/Debian: `sudo apt update && sudo apt install -y libglfw3-dev`

CMake:
- macOS (via Homebrew): `brew install cmake`
- Ubuntu/Debian: `sudo apt install -y cmake`

## Build

The Makefile will build MLX42 and libft automatically as needed.

- Build mandatory:
```bash
make
```

- Build bonus:
```bash
make bonus
```

- Clean objects and MLX build:
```bash
make clean
```

- Remove binary and libs:
```bash
make fclean
```

- Rebuild from scratch:
```bash
make re
```

Output binary: `./cub3D`

## Run

Provide a `.cub` map file (see Map format below). Example:
```bash
./cub3D maps/sample.cub
```

If you get a window with a black screen or errors about textures, verify the paths inside your `.cub` file and that the image files exist under `images/`.

## Controls

Common defaults:
- W / S: move forward / backward
- A / D: strafe left / right
- Left / Right arrows (or Q / E): rotate view
- ESC: quit
- Space / Click: weapon action

Note: Actual key bindings are defined in the code (`keys.c`) and may vary slightly.

## Map format (.cub)

The project follows the standard 42 cub3d map format:

- Texture paths:
  - `NO path/to/north_texture.png`
  - `SO path/to/south_texture.png`
  - `WE path/to/west_texture.png`
  - `EA path/to/east_texture.png`
- Floor and ceiling colors (RGB):
  - `F R,G,B`
  - `C R,G,B`
- A grid map closed by walls (`1`), with:
  - `0` for empty space
  - `1` for wall
  - Player start position and direction using one of `N`, `S`, `E`, `W`

MLX42 works well with PNG textures.

Example:
```
NO images/brick_north.png
SO images/brick_south.png
WE images/brick_west.png
EA images/brick_east.png
F 50,50,50
C 150,150,200

1111111111
1000000001
10N0000001
1000110001
1000000001
1111111111
```

Place your textures under `images/` or update the paths accordingly.

## Project structure

```
.
├─ Makefile
├─ MLX/              # MLX42 (built via CMake to MLX/build)
├─ libft/            # custom standard library
├─ include/          # headers
├─ src/
│  ├─ mandatory/     # main implementation
│  └─ bonus/         # bonus features (minimap, weapon, etc.)
├─ maps/             # example .cub maps
└─ images/           # textures / sprites
```

Notable source files (mandatory):
- `main.c` – entry point, argument handling
- `parsing.c`, `parsing_utils.c`, `validation.c` – `.cub` parsing and checks
- `movements_and_rotation.c`, `keys.c` – input handling
- `ray_direction.c`, `get_ray_distance.c`, `rays.c` – ray casting
- `wall_rendering.c`, `rendering_utils.c` – drawing pipeline
- `initialization_and_free.c`, `cub3D_utils.c` – setup and cleanup

Bonus examples:
- `minimap.c` – minimap overlay
- `weapon.c` – weapon sprite/animation

## Troubleshooting

- GLFW not found:
  - Install it (see Requirements), ensure headers and libs are in default search paths.
- Linker errors for `-ldl`, `-lpthread`, `-lm`:
  - Ensure you’re on Linux or have equivalents; these are standard libs.
- MLX build errors:
  - Delete `MLX/build` and run `make` again.
- Window opens but no textures:
  - Check texture file paths in your `.cub`. Paths are relative to the working directory.
- Map validation failed:
  - Ensure the map is fully enclosed by walls, has exactly one player start, and valid characters only.

## Notes

- Build flags: `-Wextra -Wall -Werror` are enabled. Fix all warnings to build.
- Performance flags: `-Ofast` are used by default.

## Acknowledgements

- 42 Network’s cub3d subject
- MLX42
- Lode’s ray casting tutorials and Wolfenstein 3D for inspiration
