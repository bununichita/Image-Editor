# C PGM / PPM format image editor

### Author: Nichita-Adrian Bunu, 323CA Facultatea de Automatica si Calculatoare UNSTPB 
**Contact:** [nichita_adrian.bunu@stud.acs.upb.ro](mailto:nichita_adrian.bunu@stud.acs.upb.ro)

---

## Overview

This project provides a collection of subprograms to handle image processing tasks in C. The utility allows loading, selecting, cropping, rotating, equalizing, and saving images in PGM / PPM format. It includes a histogram feature for grayscale images and efficient memory management using matrix structures.

---

## Features and Functionality

### LOAD

The `load` function loads an image file specified by the user, saving the information to a provided structure:
- Frees any previously loaded file data.
- Extracts metadata like type, width, height, and max value.
- Reads pixel data into a matrix depending on the image type.
- Displays an error message if the file cannot be opened.

### SELECT

The `selection` function selects a specified area within the image:
- Validates coordinates and displays errors for invalid selections.
- If valid, prints "Selected x0 y0 x1 y1".

### SELECT_ALL

`select_all` selects the entire image, setting selection coordinates to cover the whole file:
- Prints "Selected ALL".

### HISTOGRAM

`histogram` generates a histogram for a grayscale image within a specified selection:
- Validates coordinates and image type.
- Displays errors for invalid inputs.
- Calculates and displays the histogram if valid.

### EQUALIZE

`equalize` performs histogram equalization on grayscale images to balance intensity levels:
- Applies the equalization transformation and prints "Equalize done".

### ROTATE

The `rotate` function rotates an image or selected area:
- **rotate_sel** rotates a selected area.
- **rotate_full** rotates the entire image.
- Both versions use an intermediate matrix for efficient rotation without modifying original data.
  
### CROP

The `crop` function cuts out a selected portion of the image:
- Allocates a temporary matrix to store the cropped area.
- Replaces the original matrix with the cropped section.

### APPLY

`apply` is a transformation function for image processing:
- For grayscale images, it performs a specific operation based on the parameters.
- Results are clamped to fit within allowed pixel values.

### SAVE

`save` writes the image to a file in binary or text format based on user input:
- Writes image metadata and pixel data accordingly.

### MAIN

The `main` function continuously processes user commands:
- Commands like `LOAD`, `SAVE`, `EXIT` call relevant functions.
- Frees memory upon exit and displays appropriate messages for invalid commands.

---

## Efficiency Enhancements

- Use of `union` for efficient memory management.
- Efficient memory use by managing matrix selection coordinates instead of creating full copies.
- Rotation functions skip 360-degree rotations for time efficiency.
- Explicit function and variable names contribute to maintainability and readability.

---
