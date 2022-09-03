#include <stdio.h>
#include <stdlib.h>
 

int **matNew( size_t rows, size_t cols ) {
    int **m = malloc(sizeof(int *) * rows);

    if (NULL == m) {
        return NULL;
    }
    for (size_t i = 0; i < rows; i++) {
        m[i] = malloc(cols * sizeof(int));
        if (NULL == m[i]) {
            while (i--) {
                free(m[i]);
            }
            free(m);
            return NULL;
        }
    }

    return m;
}

void genLifeMat( int ***mat, size_t rows, size_t columns, FILE *filePointer ) {
    *mat = matNew( rows, columns );

    for ( size_t r = 0; r < rows; r++ ){ 
        for ( size_t c = 0; c < columns; c++ ){ 
           int value;
           fscanf( filePointer, "%d", &value );
           (*mat)[r][c] = value;
        }
    }
}

void genTypeMat(int ***mat, size_t rows, size_t columns, FILE *filepointer){
    *mat = matNew(rows, columns);
    for (size_t r = 0; r < rows; r++){
        for (size_t c = 0; c< columns; c++){
            (*mat)[r][c] = rand() %4;
        }
    }
}

void matShow( int ** mat, size_t rows, size_t cols ) {

    for ( size_t r = 0; r < rows; r++ ) {
        for ( size_t c = 0; c < cols; c++ ) {
            printf( "%d ", mat[r][c] );
        }
        printf( "\n" );
    }

}

void duplicateEx( int ** mat, size_t rows, size_t cols ) {
    for ( size_t r = 0; r < rows; r++ ) {
        for ( size_t c = 0; c < cols; c++ ) {
             mat[r][c] = mat[r][c] * 2;
        }
    }
}

void check( int expected, int provided, char* msg ) {
    if (expected == provided) {
        printf( "." );
    } else {
        puts( "" );
        puts( msg );
    }
}

void checkMat( int **expected, int **provided, size_t rows, size_t cols ) {
    for ( size_t r = 0; r < rows; r++ ) {
        for ( size_t c = 0; c < cols; c++ ) {
            check( expected[r][c], provided[r][c], "expected value fail" );
        }
    }
}

void matFree(int ***mat, size_t rows, size_t cols) {
    int **_m;

    if (mat) {
        _m = *mat;
        while (rows--) {
            free(_m[rows]);
        }
        free(_m);
        *mat = NULL;
    }
}

int play() {
    FILE *filePointer ;
    int rows, cols;

    filePointer = fopen( "text.txt", "r" ) ;
     
    if ( filePointer == NULL ) {
        printf( "file failed to open." ) ;
    } else {
        printf("The file is now opened.\n") ;
         
        fscanf( filePointer, "%d %d", &rows, &cols );
        printf( "<%d %d>\n", rows, cols) ;

        int **life;
        int **type;

        genLifeMat( &life, rows, cols, filePointer ); 
        genTypeMat( &type, rows, cols, filePointer );

        matShow( life, rows, cols );
        matShow( type, rows, cols );
        //Matlife( &expected, rows, cols, filePointer );
        //matShow( expected, rows, cols );


        //duplicateEx( provided, rows, cols );

        //matShow( provided, rows, cols );
        //checkMat( provided, expected, rows, cols );

        matFree( &life, rows, cols );
        //matFree( &expected, rows, cols );

        fclose(filePointer) ;
        printf("The file is now closed.\n") ;
    }
}

 
int main() {
    // save();
    play();
} 