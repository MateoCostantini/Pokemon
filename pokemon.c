#include <stdio.h>
#include <stdlib.h>
 

int **matNew( size_t rows, size_t cols ) {
    int **m = malloc(sizeof(int *) * (rows));

    if (NULL == m) {
        return NULL;
    }
    for (size_t i = 0; i < rows; i++) {
        m[i] = malloc((cols) * sizeof(int));
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

float **matNewDamage( void) {
    float **m = malloc(sizeof(float *) * (4));

    if (NULL == m) {
        return NULL;
    }
    for (size_t i = 0; i < 4; i++) {
        m[i] = malloc((4) * sizeof(float));
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
    int value;
    fscanf( filePointer, "%d", &value );

    for ( size_t r = 0; r < rows; r++ ){ 
        for ( size_t c = 0; c < columns; c++ ){ 
           if(r == 0 || r == rows -1 || c == 0 || c == columns - 1){
                (*mat)[r][c] = 3;
           } else {
                (*mat)[r][c] = value;
           }
           
        }
    }
}

void genTypeMat(int ***mat, size_t rows, size_t columns, FILE *filepointer){
    *mat = matNew(rows, columns);
    for (size_t r = 0; r < rows; r++){
        for (size_t c = 0; c< columns; c++){
            if(r == 0 || r == rows -1 || c == 0 || c == columns - 1){
                (*mat)[r][c] = 3;
           } else {
                (*mat)[r][c] = rand() %3;
           }
        }
    }
}

void genBlankMat(int ***mat, size_t rows, size_t columns, FILE *filepointer){
    *mat = matNew(rows, columns);
    for (size_t r = 0; r < rows; r++){
        for (size_t c = 0; c< columns; c++){
            if(r == 0 || r == rows -1 || c == 0 || c == columns - 1){
                (*mat)[r][c] = 3;
           } else {
                (*mat)[r][c] = 0;
           }
        }
    }
}


void genTypeDamageMat(float *** mat){
    *mat = matNewDamage();
    for (size_t r = 0; r < 4; r++){
        for (size_t c = 0; c < 4; c++){
            if((r == 0 && c == 2) || (r == 1 && c == 0) || (r == 2 && c == 1)){
                (*mat)[r][c] = 2;
           } else if(c == 3 || r == 3){
                (*mat)[r][c] = 0;
           }else {
                (*mat)[r][c] = 0.5;
           }
        }
    }
}



void matShow( int ** mat, size_t rows, size_t cols ) {

    for ( size_t r = 0; r < rows; r++ ) {
        for ( size_t c = 0; c < cols; c++ ) {
            printf( "%d\t", mat[r][c] );
        }
        printf( "\n" );
    }

}

void matDamageShow( float ** mat){

    for ( size_t r = 0; r < 4; r++ ) {
        for ( size_t c = 0; c < 4; c++ ) {
            printf( "%f\t", mat[r][c] );
        }
        printf( "\n" );
    }

}

//Hacer que se ataquen usando la matriz de damage para calcular el da~no

float getDamage(float ** damageMat, int ** typeMat, int rowIndex, int colIndex){
    int actualPokeType = typeMat[rowIndex][colIndex];
    int leftPokeType = typeMat[rowIndex-1][colIndex];
    int rightPokeType = typeMat[rowIndex+1][colIndex];
    int upPokeType = typeMat[rowIndex][colIndex-1];
    int downPokeType = typeMat[rowIndex][colIndex+1];
    printf("%f\n", damageMat[leftPokeType][actualPokeType] + damageMat[rightPokeType][actualPokeType] + damageMat[upPokeType][actualPokeType] + damageMat[downPokeType][actualPokeType]);
    return damageMat[leftPokeType][actualPokeType] 
            + damageMat[rightPokeType][actualPokeType] 
            + damageMat[upPokeType][actualPokeType] 
            + damageMat[downPokeType][actualPokeType];
}

void actNewMats(int ** oldTypeMat, int ** newTypeMat, float ** oldLifeMat, float ** newLifeMat, float ** damageMat, int rows, int columns){
    for(size_t r=0; r<rows; r++){
        for (size_t c=0; c<rows; c++){
            if (oldTypeMat[r][c] != 3){
                if(oldLifeMat[r][c] > 0){
                    oldLifeMat[r][c] = getDamage(damageMat, oldTypeMat, rows, columns);
                }
            }
        }
    }
}

void copyAndCleanMat(int **new_mat, int **old_mat, size_t rows, size_t columns){
    for (size_t r = 0; r < rows; r++){
        for (size_t c = 0; c < columns; c++){
             old_mat[r][c] = new_mat[r][c];
             if(r == 0 || r == rows -1 || c == 0 || c == columns - 1){
                new_mat[r][c] = 3;
           } else {
                new_mat[r][c] = 0;
           }
        }
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
        rows = rows + 2;
        cols = cols + 2;

        int **life;
        int **type;
        int **modifLife;
        int **modifType;
        float **typeDamage;

        genLifeMat( &life, rows, cols, filePointer ); 
        genTypeMat( &type, rows, cols, filePointer );
        genBlankMat( &modifLife, rows, cols, filePointer );
        genBlankMat( &modifType, rows, cols, filePointer );
        genTypeDamageMat(&typeDamage);
        




        matShow( life, rows, cols );
        printf(" \n");
        matShow( type, rows, cols );
        printf(" \n");
        //copyAndCleanMat(type, life, rows, cols);
        //matShow( life, rows, cols );
        //printf(" \n");
        //matShow( type, rows, cols );
        matDamageShow( typeDamage);
        //matShow( modifType, rows, cols );



        //Matlife( &expected, rows, cols, filePointer );
        //matShow( expected, rows, cols );


        //duplicateEx( provided, rows, cols );

        //matShow( provided, rows, cols );
        //checkMat( modifLife, modifType, rows, cols );

        //matFree( &life, rows, cols );
        //matFree( &expected, rows, cols );

        fclose(filePointer) ;
        printf("The file is now closed.\n") ;
    }
}

 
int main() {
    // save();
    play();
} 