# bhavasudhan_mini_project
# 2D Graphics Editor (Windows Console)

A simple C program that implements a 2D graphics editor using a character-based canvas.

## Features

- Draw shapes using ASCII-style graphics on a `WIDTH x HEIGHT` canvas
- Supported objects:
  - Line
  - Rectangle
  - Circle
  - Triangle
- Add objects to the scene
- Delete objects by ID
- Display the current picture in the console
- List all added objects

## Files

- `graphics_editor.c` - main source code for the editor

## Build Instructions

This project is designed for Windows and uses the Windows Console API.

Compile with Microsoft Visual C++ (cl.exe):

```cmd
cl.exe graphics_editor.c
```

Then run:

```cmd
graphics_editor.exe
```

## Usage

1. Run the compiled program.
2. Select an option from the menu using number keys or arrow keys.
3. Follow the prompts to add, delete, or display objects.

## Notes

- The canvas is stored as a 2D array of characters.
- Empty space is represented by `_` and drawn pixels are represented by `*`.
- No external libraries are required.
