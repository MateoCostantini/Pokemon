#include <stdio.h>
#include <stdlib.h>
 

float **matNew( size_t rows, size_t cols ) {
    float **m = malloc(sizeof(float *) * (rows));

    if (NULL == m) {
        return NULL;
    }
    for (size_t i = 0; i < rows; i++) {
        m[i] = malloc((cols) * sizeof(float));
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

/*float **matNewDamage( void) {
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
}*/

void genLifeMat( float ***mat, size_t rows, size_t columns, FILE *filePointer ) {
    *mat = matNew( rows, columns );
    float value;
    fscanf( filePointer, "%f", &value );

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

void genTypeMat(float ***mat, size_t rows, size_t columns, FILE *filepointer){
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

void genBlankMat(float ***mat, size_t rows, size_t columns, FILE *filepointer){
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
    *mat = matNew(4, 4);
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



void matShow( float ** mat, size_t rows, size_t cols ) {

    for ( size_t r = 0; r < rows; r++ ) {
        for ( size_t c = 0; c < cols; c++ ) {
            printf( "%.1f\t", mat[r][c] );
        }
        printf( "\n" );
    }

}

/*void matDamageShow( float ** mat){

    for ( size_t r = 0; r < 4; r++ ) {
        for ( size_t c = 0; c < 4; c++ ) {
            printf( "%f\t", mat[r][c] );
        }
        printf( "\n" );
    }

}*/

//Hacer que se ataquen usando la matriz de damage para calcular el da~no

float getDamage(float ** damageMat, float ** typeMat, int rowIndex, int colIndex){
    int actualPokeType = typeMat[rowIndex][colIndex];
    int leftPokeType = typeMat[rowIndex-1][colIndex];
    int rightPokeType = typeMat[rowIndex+1][colIndex];
    int upPokeType = typeMat[rowIndex][colIndex-1];
    int downPokeType = typeMat[rowIndex][colIndex+1];
    
    return damageMat[leftPokeType][actualPokeType] 
            + damageMat[rightPokeType][actualPokeType] 
            + damageMat[upPokeType][actualPokeType] 
            + damageMat[downPokeType][actualPokeType];
}

float randNeighbour(float ** typeMat, int rowIndex, int colIndex, int rows, int columns){
    size_t i;
    float neighbour;
    float neighbours[] = {typeMat[rowIndex-1][colIndex], 
                            typeMat[rowIndex+1][colIndex],
                            typeMat[rowIndex][colIndex-1],
                            typeMat[rowIndex][colIndex +1]};

    while (neighbour != (0.0 || 1.0 || 2.0)){
        i = rand() % 4; 
        neighbour = neighbours[i];
    }
    return neighbour;
    // si se mueren un cuadrado de 3x3 al mismo tiempo, saltaria un error con el espacio del medio, porque si el while corre hasta que no sea pared
    //y hasta que encuentre un vecino vivo para reemplazar, habria que crear un tipo de poke vacio

}

void actNewMats(float ** oldTypeMat, float ** newTypeMat, float ** lifeMat, float ** damageMat, int rows, int columns){
    int neighboursType[4];
    int randNeighbour;
    for(size_t r=0; r<rows; r++){
        for (size_t c=0; c<rows; c++){
            if (oldTypeMat[r][c] != 3){
                if(lifeMat[r][c] > 0){
                    lifeMat[r][c] = lifeMat[r][c] - getDamage(damageMat, oldTypeMat, r, c);
                }if(lifeMat[r][c] <= 0){

                    /*neighboursType[0] = oldTypeMat[r-1][c];
                    neighboursType[1] = oldTypeMat[r+1][c];
                    neighboursType[2] = oldTypeMat[r][c-1];
                    neighboursType[3] = oldTypeMat[r][c+1];
                    randNeighbour = rand() %3;
                    while*/
                }
            }
        }
    }
}



void copyAndCleanMat(float **new_mat, float **old_mat, size_t rows, size_t columns){
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
    int const typesPokes = 4;

    filePointer = fopen( "text.txt", "r" ) ;
     
    if ( filePointer == NULL ) {
        printf( "file failed to open." ) ;
    } else {
        printf("The file is now opened.\n") ;
         
        fscanf( filePointer, "%d %d", &rows, &cols );
        printf( "<%d %d>\n", rows, cols) ;
        rows = rows + 2;
        cols = cols + 2;

        float **life;
        float **type;
        float **modifLife;
        float **modifType;
        float **typeDamage;

        genLifeMat( &life, rows, cols, filePointer ); 
        genTypeMat( &type, rows, cols, filePointer );
        //genBlankMat( &modifLife, rows, cols, filePointer );
        genBlankMat( &modifType, rows, cols, filePointer );
        genTypeDamageMat(&typeDamage);
        
        




        matShow( life, rows, cols );
        printf(" \n");
        matShow( type, rows, cols );
        printf(" \n");
        actNewMats(type, modifType, life, typeDamage, rows, cols);
        matShow( life, rows, cols );

        //copyAndCleanMat(type, life, rows, cols);
        //matShow( life, rows, cols );
        //printf(" \n");
        //matShow( type, rows, cols );
        //matShow( typeDamage, typesPokes, typesPokes);
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