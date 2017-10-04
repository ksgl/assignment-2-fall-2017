//
//  main.c
//  assignment-2
//
//  Created by Ksenia on 04/10/2017.
//  Copyright © 2017 Ksenia. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <assert.h>

struct element {
    size_t row, col;
    double value;
};

struct element* create_matrix(size_t row_num, size_t col_num) {
    struct element* matrix = malloc(sizeof(struct element) * (row_num * col_num));
    if (matrix) {
        for (size_t i = 0; i < (row_num * col_num); ++i) {
            matrix[i].row = i / col_num;
            matrix[i].col = i % col_num;
            matrix[i].value = 0.0;
        }
    }
    return matrix;
}

void destroy_matrix(struct element** matrix) {
    if (matrix != NULL) {
        free(*matrix);
        *matrix = NULL;
    }
}

double get_elem(struct element* matrix, int row_num, int col_num) {
    assert(matrix);
    for (size_t i = 0; 1; ++i) {
        if (matrix[i].row == row_num && matrix[i].col == col_num) {
            return matrix[i].value;
        }
    }
    return 0.0;
}

void set_elem(struct element* matrix, size_t row_num, size_t col_num, double new_value) {
    assert(matrix);
    for (size_t i = 0; i < (row_num * col_num); ++i) {
        if (matrix[i].row == row_num && matrix[i].col == col_num) {
            matrix[i].value = new_value;
            return;
        }
    }
}

void print_matrix(FILE *file, struct element* matrix, size_t row_num, size_t col_num) {
    assert(matrix);
    for (int i = 0; i < (row_num * col_num); ++i) {
        if (i > 0 && i % col_num == 0) {
            fprintf(file, "\n");
        }
        fprintf(file, "%lf  ", get_elem(matrix, i / col_num, i % col_num));
    }
}

void transpose(struct element* matrix, size_t row_num, size_t col_num) {
    assert(matrix);
    assert(row_num > 0 || col_num > 0);
    size_t temp;
    for (size_t i = 0; i < (row_num * col_num); ++i) {
        temp = matrix[i].row;
        matrix[i].row = matrix[i].col;
        matrix[i].col = temp;
    }
}

int main(int argc, const char * argv[]) {
    if (argc != 3 ) {
        printf("Usage: %s <matrix-filename>\n", (argc > 0 ? argv[0] : "this-executable"));
        return EXIT_FAILURE;
    }
    
    const char* filename = argv[1];
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Unable to open the matrix file %s for reading: %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }
    
    size_t row_num = 0;
    size_t col_num = 0;
    
    int res = 0;
    
    res = fscanf(file, "%zu\n", &row_num);
    if (res != 1 || row_num <= 0) {
        if (feof(file) != 0 || row_num == 0) {
            printf("Error while reading the matrix file: expected a line with the number of matrix rows (positive integer).\n");
        }
        else {
            printf("Error while reading the matrix file: %s\n", (ferror(file) != 0 ? "unknown error" : strerror(errno)));
        }
        return EXIT_FAILURE;
    }
    
    res = fscanf(file, "%zu\n", &col_num);
    if (res != 1 || col_num <= 0) {
        if (feof(file) != 0 || col_num == 0) {
            printf("Error while reading the matrix file: expected a line with the number of matrix columns (positive integer).\n");
        }
        else {
            printf("Error while reading the matrix file: %s\n", (ferror(file) != 0 ? "unknown error" : strerror(errno)));
        }
        return EXIT_FAILURE;
    }
    
    struct element* matrix = create_matrix(row_num, col_num);
    if (matrix == NULL) {
        printf("Error while creating matrix");
        return EXIT_FAILURE;
    }
    
    //fseek(file, 0, SEEK_SET);
    
    for (size_t i = 0; i < (row_num * col_num); ++i) {
        
        res = fscanf(file, "%lf", &matrix[i].value);
        
        if (res != 1) {
            if (feof(file) != 0) {
                printf("Error while reading the matrix file: expected a floating point value (double).\n");
            }
            else {
                printf("Error while reading the matrix file: %s\n", (ferror(file) != 0 ? "unknown error" : strerror(errno)));
            }
            return EXIT_FAILURE;
        }
    }
    
    if (feof(file) == 0) {
        printf("Error while reading the matrix file: unexpected extra data.\n");
        return EXIT_FAILURE;
    }
    
    if (fclose(file) != 0) {
        printf("Unable to close the matrix file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    //print_matrix(matrix, row_num, col_num);
    transpose(matrix, row_num, col_num);
    const char* wfile = argv[2];
    file = fopen(wfile, "w");
    
    if (file == NULL) {
        printf("Unable to open the matrix file %s for writing: %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }
    
    print_matrix(file, matrix, col_num, row_num);
    
    if (fclose(file) != 0) {
        printf("Unable to close the matrix file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    destroy_matrix(&matrix);
    
    return EXIT_SUCCESS;
}

