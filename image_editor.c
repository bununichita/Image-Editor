// # Copyright Bunu Nichita-Adrian 313CA 2022-2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define PREDEF 50
#define PREDEF2 1000

struct select {
	int x0;
	int x1;
	int y0;
	int y1;
};

struct color_matrix {
	int r;
	int g;
	int b;
};

union matrix {
	int **mat;
	struct color_matrix **c_mat;
};

struct file {
	char name[PREDEF];
	int type;
	int width;
	int height;
	int max_value;
	union matrix m;
	struct select sel;
};

int if_num(char *s)
{
	// Verificare daca un string este un numar
	int l = strlen(s);
	for (int i = 0; i < l; i++)
		if ((s[i] < '0' && s[i] != '-') || s[i] > '9')
			return 0;
	return 1;
}

int clamp(int n)
{
	if (n < 0)
		return 0;
	else if (n > 255)
		return 255;
	else
		return n;
}

void swap(int *a, int *b)
{
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

void swap_s(struct color_matrix *a, struct color_matrix *b)
{
	struct color_matrix aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

int load(struct file *p, char s[PREDEF], int loaded)
{
	// Daca exista matrice incarcata, elibereaza memoria acesteia
	if (loaded) {
		if (p->type == 2 || p->type == 5) {
			for (int i = 0; i < p->height; i++)
				free(p->m.mat[i]);
			free(p->m.mat);
		} else {
			for (int i = 0; i < p->height; i++)
				free(p->m.c_mat[i]);
			free(p->m.c_mat);
		}
	}
	FILE * f = fopen(s, "r");
	if (!f) {
		printf("Failed to load %s\n", s); return 0;
	}
	strcpy((p->name), s);
	char line[PREDEF2], aux[5], *pointer;
	fgets(line, PREDEF2, f);
	while (line[0] == '#')
		fgets(line, PREDEF2, f);
	strcpy(aux, line);
	p->type = (int)(aux[1] - '0');
	fgets(line, PREDEF2, f);
	while (line[0] == '#')
		fgets(line, PREDEF2, f);
	pointer = strtok(line, " ");
	(p->width) = atoi(pointer);
	pointer = strtok(NULL, " ");
	(p->height) = atoi(pointer);
	fgets(line, PREDEF2, f);
	while (line[0] == '#')
		fgets(line, PREDEF2, f);
	pointer = strtok(line, "\n");
	(p->max_value) = atoi(pointer);
	unsigned char pixel;
	if (p->type == 2 || p->type == 5) {
		(p->m.mat) = NULL; (p->m.mat) = malloc((p->height) * sizeof(int *));
		for (int i = 0; i < (p->height); i++)
			(p->m.mat)[i] = malloc((p->width) * sizeof(int));
		if (p->type == 2)
			for (int i = 0; i < (p->height); i++)
				for (int j = 0; j < (p->width); j++)
					fscanf(f, "%d", &p->m.mat[i][j]);
		else
			for (int i = 0; i < (p->height); i++)
				for (int j = 0; j < (p->width); j++) {
					fread(&pixel, sizeof(unsigned char), 1, f);
					p->m.mat[i][j] = (int)pixel;
				}
	} else {
		(p->m.c_mat) = NULL;
		(p->m.c_mat) = malloc((p->height) * sizeof(struct color_matrix *));
		for (int i = 0; i < (p->height); i++)
			(p->m.c_mat)[i] = malloc((p->width) * sizeof(struct color_matrix));
		if (p->type == 3)
			for (int i = 0; i < (p->height); i++)
				for (int j = 0; j < (p->width); j++) {
					fscanf(f, "%d", &p->m.c_mat[i][j].r);
					fscanf(f, "%d", &p->m.c_mat[i][j].g);
					fscanf(f, "%d", &p->m.c_mat[i][j].b);
				}
		else
			for (int i = 0; i < (p->height); i++)
				for (int j = 0; j < (p->width); j++) {
					fread(&pixel, sizeof(unsigned char), 1, f);
					(p->m.c_mat[i][j].r) = (int)pixel;
					fread(&pixel, sizeof(unsigned char), 1, f);
					(p->m.c_mat[i][j].g) = (int)pixel;
					fread(&pixel, sizeof(unsigned char), 1, f);
					(p->m.c_mat[i][j].b) = (int)pixel;
				}
	}
	(p->sel.x0) = 0; (p->sel.y0) = 0;
	(p->sel.x1) = (p->width);
	(p->sel.y1) = (p->height);
	fclose(f);
	printf("Loaded %s\n", s);
	return 1;
}

void sel_inv_comm(struct file *p, int x0, int x1, int y0, int y1)
{
	(p->sel.x0) = x0;
	(p->sel.x1) = x1;
	(p->sel.y0) = y0;
	(p->sel.y1) = y1;
	printf("Invalid command\n");
}

void sel_inv_coord(struct file *p, int x0, int x1, int y0, int y1)
{
	(p->sel.x0) = x0;
	(p->sel.x1) = x1;
	(p->sel.y0) = y0;
	(p->sel.y1) = y1;
	printf("Invalid set of coordinates\n");
}

int selection(char s[PREDEF], struct file *p)
{
	int nr_spaces = 0;
	for (int i = 0; i < (int)strlen(s); i++)
		if (s[i] == ' ')
			nr_spaces++;
	if (nr_spaces != 4) {
		printf("Invalid command\n");
		return 0;
	}
	int x0, x1, y0, y1;
	char *string;
	x0 = (p->sel.x0);
	x1 = (p->sel.x1);
	y0 = (p->sel.y0);
	y1 = (p->sel.y1);
	string = strtok(s, " \0");
	string = strtok(NULL, " \0");
	if (if_num(string)) {
		(p->sel.x0) = atoi(string);
	} else {
		sel_inv_comm(p, x0, x1, y0, y1);
		return 0;
	}
	(p->sel.x0) = atoi(string);
	string = strtok(NULL, " \0");
	if (if_num(string)) {
		(p->sel.y0) = atoi(string);
	} else {
		sel_inv_comm(p, x0, x1, y0, y1);
		return 0;
	}
	(p->sel.y0) = atoi(string);
	string = strtok(NULL, " \0");
	if (if_num(string)) {
		(p->sel.x1) = atoi(string);
	} else {
		sel_inv_comm(p, x0, x1, y0, y1);
		return 0;
	}
	(p->sel.x1) = atoi(string);
	string = strtok(NULL, " \0");
	if (if_num(string)) {
		(p->sel.y1) = atoi(string);
	} else {
		sel_inv_comm(p, x0, x1, y0, y1);
		return 0;
	}
	(p->sel.y1) = atoi(string);
	if (p->sel.x0 > p->sel.x1)
		swap(&p->sel.x0, &p->sel.x1);
	if (p->sel.y0 > p->sel.y1)
		swap(&p->sel.y0, &p->sel.y1);
	int dif_1, dif_2;
	dif_1 = p->sel.x1 - p->sel.x0;
	dif_2 = p->sel.y1 - p->sel.y0;
	if (!dif_1 || !dif_2) {
		sel_inv_coord(p, x0, x1, y0, y1);
		return 0;
	}
	if (p->sel.x0 < 0 || p->sel.y0 < 0) {
		sel_inv_coord(p, x0, x1, y0, y1);
		return 0;
	}
	if (p->sel.x1 > p->width || p->sel.y1 > p->height) {
		sel_inv_coord(p, x0, x1, y0, y1);
		return 0;
	}
	x0 = (p->sel.x0);
	x1 = (p->sel.x1);
	y0 = (p->sel.y0);
	y1 = (p->sel.y1);
	printf("Selected %d %d %d %d\n", x0, y0, x1, y1);
	return 0;
}

void select_all(struct file *p)
{
	(p->sel.x0) = 0;
	(p->sel.y0) = 0;
	(p->sel.x1) = (p->width);
	(p->sel.y1) = (p->height);
	printf("Selected ALL\n");
}

int histogram(char s[PREDEF], struct file *p)
{
	int nr_spaces = 0, aux, i, x, y, frq[256], frq_2[256], freq_max = 0;
	for (int i = 0; i < (int)strlen(s); i++)
		if (s[i] == ' ')
			nr_spaces++;
	if (nr_spaces != 2) {
		printf("Invalid command\n");
		return 0;
	}
	if (p->type == 3 || p->type == 6) {
		printf("Black and white image needed\n");
		return 0;
	}
	char *pointer, copie[PREDEF];
	strcpy(copie, s + 10);
	x = 0;
	y = 0;
	pointer = strtok(copie, " ");
	aux = strlen(pointer);
	for (i = 0; i < aux; i++)
		x = x * 10 + (int)((*(pointer + i)) - '0');
	pointer = strtok(NULL, " ");
	aux = strlen(pointer);
	for (i = 0; i < aux; i++)
		y = y * 10 + (int)((*(pointer + i)) - '0');
	for (int i = 0; i < 256; i++)
		frq[i] = 0;
	for (int i = p->sel.y0; i < p->sel.y1; i++)
		for (int j = p->sel.x0; j < p->sel.x1; j++)
			frq[p->m.mat[i][j]]++;
	int stack = ((p->max_value) + 1) / y;
	for (int i = 0; i < y; i++)
		frq_2[i] = 0;
	for (int i = 0; i < y; i++)
		for (int j = 0; j < stack; j++)
			frq_2[i] += frq[i * stack + j];
	for (int i = 0; i < y; i++)
		if (frq_2[i] > freq_max)
			freq_max = frq_2[i];
	for (int i = 0; i < y; i++) {
		frq_2[i] *= x;
		frq_2[i] /= freq_max;
	}
	for (int i = 0; i < y; i++) {
		printf("%d	|	", frq_2[i]);
		for (int j = 0; j < frq_2[i]; j++)
			printf("*");
		printf("\n");
	}
	return 0;
}

int equalize(struct file *p)
{
	if (p->type == 3 || p->type == 6) {
		printf("Black and white image needed\n");
		return 0;
	}
	int frq[256] = {0};
	double f_a;
	unsigned long long sum, area = (p->height) * (p->width);
	for (int i = 0; i < p->height; i++)
		for (int j = 0; j < p->width; j++)
			frq[p->m.mat[i][j]]++;
	for (int i = 0; i < p->height; i++)
		for (int j = 0; j < p->width; j++) {
			sum = 0;
			for (int c = 0; c <= p->m.mat[i][j]; c++)
				sum += frq[c];
			f_a = (double)255 * (double)sum / (double)area;
			f_a = round(f_a);
			if (f_a > 255)
				f_a = 255;
			p->m.mat[i][j] = f_a;
		}
	printf("Equalize done\n");
	return 0;
}

void rotate_sel(struct file *p, int angle, int h, int w)
{
	int y0 = p->sel.y0, x0 = p->sel.x0;
	if (p->type == 2 || p->type == 5) {
		int matrix[h][w];
		for (int count = 0; count < angle; count++) {
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
					matrix[i][j] = p->m.mat[y0 + h - 1 - j][x0 + i];
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
					p->m.mat[y0 + i][x0 + j] = matrix[i][j];
		}
	} else {
		struct color_matrix **matrix;
		matrix = malloc(h * sizeof(struct color_matrix *));
		for (int i = 0; i < h; i++)
			matrix[i] = malloc(w * sizeof(struct color_matrix));
		for (int count = 0; count < angle; count++) {
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++) {
					matrix[i][j].r = p->m.c_mat[y0 + h - 1 - j][x0 + i].r;
					matrix[i][j].g = p->m.c_mat[y0 + h - 1 - j][x0 + i].g;
					matrix[i][j].b = p->m.c_mat[y0 + h - 1 - j][x0 + i].b;
				}
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++) {
					p->m.c_mat[y0 + i][x0 + j].r = matrix[i][j].r;
					p->m.c_mat[y0 + i][x0 + j].g = matrix[i][j].g;
					p->m.c_mat[y0 + i][x0 + j].b = matrix[i][j].b;
				}
		}
		for (int i = 0; i < h; i++)
			free(matrix[i]);
		free(matrix);
	}
}

void rotate_full(struct file *p, int angle, int *h, int *w)
{
	if (p->type == 2 || p->type == 5) {
		int **matrix;
		for (int count = 0; count < angle; count++) {
			matrix = NULL;
			matrix = malloc((*w) * sizeof(int *));
			for (int i = 0; i < (*w); i++)
				matrix[i] = malloc((*h) * sizeof(int));
			for (int i = 0; i < (*w); i++)
				for (int j = 0; j < (*h); j++)
					matrix[i][j] = p->m.mat[(*h) - 1 - j][i];
			for (int i = 0; i < (*h); i++)
				free(p->m.mat[i]);
			free(p->m.mat);
			p->m.mat = NULL;
			p->m.mat = malloc((*w) * sizeof(int *));
			for (int i = 0; i < (*w); i++)
				p->m.mat[i] = malloc((*h) * sizeof(int));
			for (int i = 0; i < (*w); i++)
				for (int j = 0; j < (*h); j++)
					p->m.mat[i][j] = matrix[i][j];
			for (int i = 0; i < (*w); i++)
				free(matrix[i]);
			free(matrix);
			p->height = *w;
			p->width = (*h);
			p->sel.x1 = (*h);
			p->sel.y1 = *w;
			swap(w, h);
		}
	} else {
		struct color_matrix **matrix;
		matrix = malloc((*w) * sizeof(struct color_matrix *));
		for (int i = 0; i < (*w); i++)
			matrix[i] = malloc((*h) * sizeof(struct color_matrix));
		for (int count = 0; count < angle; count++) {
			matrix = NULL;
			matrix = malloc((*w) * sizeof(struct color_matrix *));
			for (int i = 0; i < (*w); i++)
				matrix[i] = malloc((*h) * sizeof(struct color_matrix));
			for (int i = 0; i < (*w); i++)
				for (int j = 0; j < (*h); j++) {
					matrix[i][j].r = p->m.c_mat[(*h) - 1 - j][i].r;
					matrix[i][j].g = p->m.c_mat[(*h) - 1 - j][i].g;
					matrix[i][j].b = p->m.c_mat[(*h) - 1 - j][i].b;
				}
			for (int i = 0; i < (*h); i++)
				free(p->m.c_mat[i]);
			free(p->m.c_mat);
			p->m.c_mat = NULL;
			p->m.c_mat = malloc((*w) * sizeof(struct color_matrix *));
			for (int i = 0; i < (*w); i++)
				p->m.c_mat[i] = malloc((*h) * sizeof(struct color_matrix));
			for (int i = 0; i < (*w); i++)
				for (int j = 0; j < (*h); j++) {
					p->m.c_mat[i][j].r = matrix[i][j].r;
					p->m.c_mat[i][j].g = matrix[i][j].g;
					p->m.c_mat[i][j].b = matrix[i][j].b;
				}
			for (int i = 0; i < (*w); i++)
				free(matrix[i]);
			free(matrix);
			p->height = (*w);
			p->width = (*h);
			p->sel.x1 = (*h);
			p->sel.y1 = (*w);
			swap(w, h);
		}
	}
}

int rotate(struct file *p, int angle, int loaded)
{
	if (!loaded) {
		printf("No image loaded\n");
		return 0;
	}
	if (angle % 90 || angle > 360 || angle < -360) {
		printf("Unsupported rotation angle\n");
		return 0;
	}
	int h, w;
	h = (p->sel.y1) - (p->sel.y0);
	w = (p->sel.x1) - (p->sel.x0);
	int y0 = p->sel.y0, y1 = p->sel.y1, x0 = p->sel.x0, x1 = p->sel.x1;
	int all = 1, angle_copy = angle;
	if (x0 || y0 || p->height - y1 || p->width - x1) {
		all = 0;
		if (h - w) {
			printf("The selection must be square\n");
			return 0;
		}
	}
	angle = angle % 360 / 90;
	if (angle < 0)
		angle += 4;
	if (!angle) {
		printf("Rotated %d\n", angle_copy);
		return 0;
	}
	if (!all) {
		if (!angle) {
			printf("Rotated %d\n", angle_copy);
			return 0;
		}
		rotate_sel(p, angle, h, w);
	} else {
		if (!angle) {
			printf("Rotated %d\n", angle_copy);
			return 0;
		}
		rotate_full(p, angle, &h, &w);
	}
	printf("Rotated %d\n", angle_copy);
	return 0;
}

int crop(struct file *p)
{
	int h, w;
	h = (p->sel.y1) - (p->sel.y0);
	w = (p->sel.x1) - (p->sel.x0);
	if (h == p->height && w == p->width) {
		printf("Image cropped\n");
		return 0;
	}
	int y0 = p->sel.y0;
	int x0 = p->sel.x0;
	if (p->type == 2 || p->type == 5) {
		int matrix[h][w];
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				matrix[i][j] = (p->m.mat[y0 + i][x0 + j]);
		for (int i = 0; i < p->height; i++)
			free(p->m.mat[i]);
		free(p->m.mat);
		p->m.mat = NULL;
		p->m.mat = malloc(h * sizeof(int *));
		for (int i = 0; i < h; i++)
			p->m.mat[i] = malloc(w * sizeof(int));
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				p->m.mat[i][j] = matrix[i][j];
		p->height = h;
		p->width = w;
		p->sel.y0 = 0;
		p->sel.y1 = h;
		p->sel.x0 = 0;
		p->sel.x1 = w;
	} else {
		struct color_matrix **matrix;
		matrix = NULL;
		matrix = malloc(h * sizeof(struct color_matrix *));
		for (int i = 0; i < h; i++)
			matrix[i] = malloc(w * sizeof(struct color_matrix));
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				matrix[i][j].r = (p->m.c_mat[y0 + i][x0 + j].r);
				matrix[i][j].g = (p->m.c_mat[y0 + i][x0 + j].g);
				matrix[i][j].b = (p->m.c_mat[y0 + i][x0 + j].b);
			}
		}
		for (int i = 0; i < p->height; i++)
			free(p->m.c_mat[i]);
		free(p->m.c_mat);
		p->m.c_mat = NULL;
		p->m.c_mat = malloc(h * sizeof(struct color_matrix *));
		for (int i = 0; i < h; i++)
			p->m.c_mat[i] = malloc(w * sizeof(struct color_matrix));
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				p->m.c_mat[i][j].r = matrix[i][j].r;
				p->m.c_mat[i][j].g = matrix[i][j].g;
				p->m.c_mat[i][j].b = matrix[i][j].b;
			}
		}
		for (int i = 0; i < h; i++)
			free(matrix[i]);
		free(matrix);
		p->height = h;
		p->width = w;
		p->sel.y0 = 0;
		p->sel.y1 = h;
		p->sel.x0 = 0;
		p->sel.x1 = w;
	}
	printf("Image cropped\n");
	return 0;
}

void err_apply(int loaded, int invalid, int p_inval)
{
	if (!loaded)
		printf("No image loaded\n");
	else if (invalid)
		printf("APPLY parameter invalid\n");
	else if (p_inval)
		printf("Invalid command\n");
}

int edge(struct file *p)
{
	if (p->type == 2 || p->type == 5) {
		printf("Easy, Charlie Chaplin\n");
		return 0;
	}
	int x0 = p->sel.x0, y0 = p->sel.y0, x1 = p->sel.x1, y1 = p->sel.y1;
	if (x0 == 0)
		x0 = p->sel.x0 + 1;
	if (y0 == 0)
		y0 = p->sel.y0 + 1;
	if (p->height == y1)
		y1 = p->sel.y1 - 1;
	if (p->width == x1)
		x1 = p->sel.x1 - 1;
	int w = x1 - x0, h = y1 - y0;
	struct color_matrix **matrix;
	matrix = malloc(h * sizeof(struct color_matrix *));
	for (int i = 0; i < h; i++)
		matrix[i] = malloc(w * sizeof(struct color_matrix));
	for (int i = y0; i < y1; i++) {
		for (int j = x0; j < x1; j++) {
			int sum = 0;
			sum += (-1) * (p->m.c_mat[i - 1][j - 1].r);
			sum += (-1) * (p->m.c_mat[i - 1][j].r);
			sum += (-1) * (p->m.c_mat[i - 1][j + 1].r);
			sum += (-1) * (p->m.c_mat[i][j - 1].r);
			sum += (-1) * (p->m.c_mat[i][j + 1].r);
			sum += (-1) * (p->m.c_mat[i + 1][j - 1].r);
			sum += (-1) * (p->m.c_mat[i + 1][j].r);
			sum += (-1) * (p->m.c_mat[i + 1][j + 1].r);
			sum += 8 * (p->m.c_mat[i][j].r);
			matrix[i - y0][j - x0].r = clamp(sum);
			sum = 0;
			sum += (-1) * (p->m.c_mat[i - 1][j - 1].g);
			sum += (-1) * (p->m.c_mat[i - 1][j].g);
			sum += (-1) * (p->m.c_mat[i - 1][j + 1].g);
			sum += (-1) * (p->m.c_mat[i][j - 1].g);
			sum += (-1) * (p->m.c_mat[i][j + 1].g);
			sum += (-1) * (p->m.c_mat[i + 1][j - 1].g);
			sum += (-1) * (p->m.c_mat[i + 1][j].g);
			sum += (-1) * (p->m.c_mat[i + 1][j + 1].g);
			sum += 8 * (p->m.c_mat[i][j].g);
			matrix[i - y0][j - x0].g = clamp(sum);
			sum = 0;
			sum += (-1) * (p->m.c_mat[i - 1][j - 1].b);
			sum += (-1) * (p->m.c_mat[i - 1][j].b);
			sum += (-1) * (p->m.c_mat[i - 1][j + 1].b);
			sum += (-1) * (p->m.c_mat[i][j - 1].b);
			sum += (-1) * (p->m.c_mat[i][j + 1].b);
			sum += (-1) * (p->m.c_mat[i + 1][j - 1].b);
			sum += (-1) * (p->m.c_mat[i + 1][j].b);
			sum += (-1) * (p->m.c_mat[i + 1][j + 1].b);
			sum += 8 * (p->m.c_mat[i][j].b);
			matrix[i - y0][j - x0].b = clamp(sum);
		}
	}
	for (int i = y0; i < y1; i++)
		for (int j = x0; j < x1; j++) {
			p->m.c_mat[i][j].r = matrix[i - y0][j - x0].r;
			p->m.c_mat[i][j].g = matrix[i - y0][j - x0].g;
			p->m.c_mat[i][j].b = matrix[i - y0][j - x0].b;
		}
	for (int i = 0; i < h; i++)
		free(matrix[i]);
	free(matrix);
	printf("APPLY EDGE done\n");
	return 0;
}

int sharpen(struct file *p)
{
	if (p->type == 2 || p->type == 5) {
		printf("Easy, Charlie Chaplin\n");
		return 0;
	}
	int x0 = p->sel.x0, y0 = p->sel.y0, x1 = p->sel.x1, y1 = p->sel.y1;
	if (x0 == 0)
		x0 = p->sel.x0 + 1;
	if (y0 == 0)
		y0 = p->sel.y0 + 1;
	if (p->height == y1)
		y1 = p->sel.y1 - 1;
	if (p->width == x1)
		x1 = p->sel.x1 - 1;
	int w = x1 - x0, h = y1 - y0;
	struct color_matrix **matrix;
	matrix = malloc(h * sizeof(struct color_matrix *));
	for (int i = 0; i < h; i++)
		matrix[i] = malloc(w * sizeof(struct color_matrix));
	for (int i = y0; i < y1; i++) {
		for (int j = x0; j < x1; j++) {
			int sum = 0;
			sum += (-1) * (p->m.c_mat[i - 1][j].r);
			sum += (-1) * (p->m.c_mat[i][j - 1].r);
			sum += (-1) * (p->m.c_mat[i][j + 1].r);
			sum += (-1) * (p->m.c_mat[i + 1][j].r);
			sum += 5 * (p->m.c_mat[i][j].r);
			matrix[i - y0][j - x0].r = clamp(sum);
			sum = 0;
			sum += (-1) * (p->m.c_mat[i - 1][j].g);
			sum += (-1) * (p->m.c_mat[i][j - 1].g);
			sum += (-1) * (p->m.c_mat[i][j + 1].g);
			sum += (-1) * (p->m.c_mat[i + 1][j].g);
			sum += 5 * (p->m.c_mat[i][j].g);
			matrix[i - y0][j - x0].g = clamp(sum);
			sum = 0;
			sum += (-1) * (p->m.c_mat[i - 1][j].b);
			sum += (-1) * (p->m.c_mat[i][j - 1].b);
			sum += (-1) * (p->m.c_mat[i][j + 1].b);
			sum += (-1) * (p->m.c_mat[i + 1][j].b);
			sum += 5 * (p->m.c_mat[i][j].b);
			matrix[i - y0][j - x0].b = clamp(sum);
		}
	}
	for (int i = y0; i < y1; i++)
		for (int j = x0; j < x1; j++) {
			p->m.c_mat[i][j].r = matrix[i - y0][j - x0].r;
			p->m.c_mat[i][j].g = matrix[i - y0][j - x0].g;
			p->m.c_mat[i][j].b = matrix[i - y0][j - x0].b;
		}
	for (int i = 0; i < h; i++)
		free(matrix[i]);
	free(matrix);
	printf("APPLY SHARPEN done\n");
	return 0;
}

int blur(struct file *p)
{
	if (p->type == 2 || p->type == 5) {
		printf("Easy, Charlie Chaplin\n");
		return 0;
	}
	int x0 = p->sel.x0, y0 = p->sel.y0, x1 = p->sel.x1, y1 = p->sel.y1;
	if (x0 == 0)
		x0 = p->sel.x0 + 1;
	if (y0 == 0)
		y0 = p->sel.y0 + 1;
	if (p->height == y1)
		y1 = p->sel.y1 - 1;
	if (p->width == x1)
		x1 = p->sel.x1 - 1;
	int w = x1 - x0, h = y1 - y0;
	struct color_matrix **matrix;
	matrix = malloc(h * sizeof(struct color_matrix *));
	for (int i = 0; i < h; i++)
		matrix[i] = malloc(w * sizeof(struct color_matrix));
	for (int i = y0; i < y1; i++) {
		for (int j = x0; j < x1; j++) {
			int sum = 0;
			sum += (p->m.c_mat[i - 1][j - 1].r);
			sum += (p->m.c_mat[i - 1][j].r);
			sum += (p->m.c_mat[i - 1][j + 1].r);
			sum += (p->m.c_mat[i][j - 1].r);
			sum += (p->m.c_mat[i][j + 1].r);
			sum += (p->m.c_mat[i + 1][j - 1].r);
			sum += (p->m.c_mat[i + 1][j].r);
			sum += (p->m.c_mat[i + 1][j + 1].r);
			sum += (p->m.c_mat[i][j].r);
			sum /= 9;
			matrix[i - y0][j - x0].r = clamp(sum);
			sum = 0;
			sum += (p->m.c_mat[i - 1][j - 1].g);
			sum += (p->m.c_mat[i - 1][j].g);
			sum += (p->m.c_mat[i - 1][j + 1].g);
			sum += (p->m.c_mat[i][j - 1].g);
			sum += (p->m.c_mat[i][j + 1].g);
			sum += (p->m.c_mat[i + 1][j - 1].g);
			sum += (p->m.c_mat[i + 1][j].g);
			sum += (p->m.c_mat[i + 1][j + 1].g);
			sum += (p->m.c_mat[i][j].g);
			sum /= 9;
			matrix[i - y0][j - x0].g = clamp(sum);
			sum = 0;
			sum += (p->m.c_mat[i - 1][j - 1].b);
			sum += (p->m.c_mat[i - 1][j].b);
			sum += (p->m.c_mat[i - 1][j + 1].b);
			sum += (p->m.c_mat[i][j - 1].b);
			sum += (p->m.c_mat[i][j + 1].b);
			sum += (p->m.c_mat[i + 1][j - 1].b);
			sum += (p->m.c_mat[i + 1][j].b);
			sum += (p->m.c_mat[i + 1][j + 1].b);
			sum += (p->m.c_mat[i][j].b);
			sum /= 9;
			matrix[i - y0][j - x0].b = clamp(sum);
		}
	}
	for (int i = y0; i < y1; i++)
		for (int j = x0; j < x1; j++) {
			p->m.c_mat[i][j].r = matrix[i - y0][j - x0].r;
			p->m.c_mat[i][j].g = matrix[i - y0][j - x0].g;
			p->m.c_mat[i][j].b = matrix[i - y0][j - x0].b;
		}
	for (int i = 0; i < h; i++)
		free(matrix[i]);
	free(matrix);
	printf("APPLY BLUR done\n");
	return 0;
}

int gaussian_blur(struct file *p)
{
	if (p->type == 2 || p->type == 5) {
		printf("Easy, Charlie Chaplin\n");
		return 0;
	}
	int x0 = p->sel.x0, y0 = p->sel.y0, x1 = p->sel.x1, y1 = p->sel.y1;
	if (x0 == 0)
		x0 = p->sel.x0 + 1;
	if (y0 == 0)
		y0 = p->sel.y0 + 1;
	if (p->height == y1)
		y1 = p->sel.y1 - 1;
	if (p->width == x1)
		x1 = p->sel.x1 - 1;
	int w = x1 - x0, h = y1 - y0;
	struct color_matrix **matrix;
	matrix = malloc(h * sizeof(struct color_matrix *));
	for (int i = 0; i < h; i++)
		matrix[i] = malloc(w * sizeof(struct color_matrix));
	for (int i = y0; i < y1; i++) {
		for (int j = x0; j < x1; j++) {
			int sum = 0;
			sum += (1) * (p->m.c_mat[i - 1][j - 1].r);
			sum += (2) * (p->m.c_mat[i - 1][j].r);
			sum += (1) * (p->m.c_mat[i - 1][j + 1].r);
			sum += (2) * (p->m.c_mat[i][j - 1].r);
			sum += (2) * (p->m.c_mat[i][j + 1].r);
			sum += (1) * (p->m.c_mat[i + 1][j - 1].r);
			sum += (2) * (p->m.c_mat[i + 1][j].r);
			sum += (1) * (p->m.c_mat[i + 1][j + 1].r);
			sum += 4 * (p->m.c_mat[i][j].r);
			sum /= 16;
			matrix[i - y0][j - x0].r = clamp(sum);
			sum = 0;
			sum += (1) * (p->m.c_mat[i - 1][j - 1].g);
			sum += (2) * (p->m.c_mat[i - 1][j].g);
			sum += (1) * (p->m.c_mat[i - 1][j + 1].g);
			sum += (2) * (p->m.c_mat[i][j - 1].g);
			sum += (2) * (p->m.c_mat[i][j + 1].g);
			sum += (1) * (p->m.c_mat[i + 1][j - 1].g);
			sum += (2) * (p->m.c_mat[i + 1][j].g);
			sum += (1) * (p->m.c_mat[i + 1][j + 1].g);
			sum += 4 * (p->m.c_mat[i][j].g);
			sum /= 16;
			matrix[i - y0][j - x0].g = clamp(sum);
			sum = 0;
			sum += (1) * (p->m.c_mat[i - 1][j - 1].b);
			sum += (2) * (p->m.c_mat[i - 1][j].b);
			sum += (1) * (p->m.c_mat[i - 1][j + 1].b);
			sum += (2) * (p->m.c_mat[i][j - 1].b);
			sum += (2) * (p->m.c_mat[i][j + 1].b);
			sum += (1) * (p->m.c_mat[i + 1][j - 1].b);
			sum += (2) * (p->m.c_mat[i + 1][j].b);
			sum += (1) * (p->m.c_mat[i + 1][j + 1].b);
			sum += 4 * (p->m.c_mat[i][j].b);
			sum /= 16;
			matrix[i - y0][j - x0].b = clamp(sum);
		}
	}
	for (int i = y0; i < y1; i++)
		for (int j = x0; j < x1; j++) {
			p->m.c_mat[i][j].r = matrix[i - y0][j - x0].r;
			p->m.c_mat[i][j].g = matrix[i - y0][j - x0].g;
			p->m.c_mat[i][j].b = matrix[i - y0][j - x0].b;
		}
	for (int i = 0; i < h; i++)
		free(matrix[i]);
	free(matrix);
	printf("APPLY GAUSSIAN_BLUR done\n");
	return 0;
}

void ap_choice(struct file *p, char s[50], int *invalid, int *p_inval)
{
	if (strcmp(s, "APPLY EDGE") == 0)
		edge(p);
	else if (strcmp(s, "APPLY SHARPEN") == 0)
		sharpen(p);
	else if (strcmp(s, "APPLY BLUR") == 0)
		blur(p);
	else if (strcmp(s, "APPLY GAUSSIAN_BLUR") == 0)
		gaussian_blur(p);
	else if (strlen(s) == 5)
		*p_inval = 1;
	else
		*invalid = 1;
}

void save(struct file *p, char s[PREDEF])
{
	char copie[PREDEF], filename[PREDEF], nr_cuv = 0, *p1, aux[PREDEF];
	strcpy(filename, s);
	strcpy(aux, filename);
	strcpy(filename, aux + 5);
	int area = p->height * p->width;
	strcpy(copie, s);
	p1 = strtok(copie, " ");
	while (p1) {
		nr_cuv++;
		p1 = strtok(NULL, " ");
	}
	if (filename[strlen(filename) - 1] == ' ')
		filename[strlen(filename) - 1] = '\0';
	if (nr_cuv == 2) {
		FILE *out = fopen(filename, "wb");
		if (p->type == 2 || p->type == 5)
			fprintf(out, "P5\n");
		else
			fprintf(out, "P6\n");
		fprintf(out, "%d %d\n", p->width, p->height);
		fprintf(out, "%d\n", p->max_value);
		if (p->type == 2 || p->type == 5) {
			unsigned char pixel[p->height][p->width];
			for (int i = 0; i < p->height; i++)
				for (int j = 0; j < p->width; j++)
					pixel[i][j] = (unsigned char)p->m.mat[i][j];
			fwrite(pixel, sizeof(unsigned char), area, out);
		} else {
			unsigned char pixel;
			for (int i = 0; i < p->height; i++)
				for (int j = 0; j < p->width; j++) {
					pixel = (unsigned char)(p->m.c_mat[i][j].r);
					fwrite(&pixel, sizeof(unsigned char), 1, out);
					pixel = (unsigned char)(p->m.c_mat[i][j].g);
					fwrite(&pixel, sizeof(unsigned char), 1, out);
					pixel = (unsigned char)(p->m.c_mat[i][j].b);
					fwrite(&pixel, sizeof(unsigned char), 1, out);
				}
		}
		fclose(out);
	} else if (nr_cuv == 3) {
		for (int i = 0; i < (int)strlen(filename); i++)
			if (filename[i] == ' ') {
				filename[i] = '\0';
				break;
			}
		FILE *out = fopen(filename, "w");
		if (p->type == 2 || p->type == 5)
			fprintf(out, "P2\n");
		else
			fprintf(out, "P3\n");
		fprintf(out, "%d %d\n", p->width, p->height);
		fprintf(out, "%d\n", p->max_value);
		if (p->type == 2 || p->type == 5) {
			for (int i = 0; i < p->height; i++) {
				for (int j = 0; j < p->width; j++)
					fprintf(out, "%d ", p->m.mat[i][j]);
				fprintf(out, "\n");
			}
		} else {
			for (int i = 0; i < p->height; i++) {
				for (int j = 0; j < p->width; j++) {
					fprintf(out, "%d ", p->m.c_mat[i][j].r);
					fprintf(out, "%d ", p->m.c_mat[i][j].g);
					fprintf(out, "%d ", p->m.c_mat[i][j].b);
				}
				fprintf(out, "\n");
			}
		}
		fclose(out);
	}
	printf("Saved %s\n", filename);
}

void exit_p(struct file *p)
{
	if (p->type == 2 || p->type == 5) {
		for (int i = 0; i < p->height; i++)
			free(p->m.mat[i]);
		free(p->m.mat);
	} else {
		for (int i = 0; i < p->height; i++)
			free(p->m.c_mat[i]);
		free(p->m.c_mat);
	}
}

int main(void)
{
	int angle, loaded = 0, fin = 0;
	struct file *p;
	p = malloc(sizeof(struct file));
	char s[PREDEF];
	while (1) {
		fgets(s, PREDEF, stdin);
		s[strlen(s) - 1] = '\0';
		if (!strncmp(s, "SELECT ALL", 10)) {
			if (loaded)
				select_all(p);
			else
				printf("No image loaded\n");
		} else if (!strncmp(s, "SELECT", 6)) {
			if (loaded)
				selection(s, p);
			else
				printf("No image loaded\n");
		} else if (strncmp(s, "LOAD", 4) == 0) {
			char aux_copy[PREDEF];
			strcpy(aux_copy, s + 5);
			loaded = load(p, aux_copy, loaded);
		} else if (strncmp(s, "HISTOGRAM", 9) == 0) {
			if (loaded)
				histogram(s, p);
			else
				printf("No image loaded\n");
		} else if (strcmp(s, "EQUALIZE") == 0) {
			if (loaded)
				equalize(p);
			else
				printf("No image loaded\n");
		} else if (strncmp(s, "ROTATE", 6) == 0) {
			angle = atoi(s + 7);
			rotate(p, angle, loaded);
		} else if (strcmp(s, "CROP") == 0) {
			if (loaded)
				crop(p);
			else
				printf("No image loaded\n");
		} else if (strncmp(s, "APPLY", 5) == 0) {
			int invalid = 0, p_inval = 0;
			if (loaded)
				ap_choice(p, s, &invalid, &p_inval);
			err_apply(loaded, invalid, p_inval);
		} else if (strncmp(s, "SAVE", 4) == 0) {
			if (loaded)
				save(p, s);
			else
				printf("No image loaded\n");
		} else if (strncmp(s, "EXI", 3) == 0) {
			if (!loaded)
				printf("No image loaded\n");
			else
				exit_p(p);
			free(p);
			fin = 1;
		} else {
			printf("Invalid command\n");
		}
		if (fin)
			break;
	}
	return 0;
}
