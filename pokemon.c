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

void genLifeMat( float ***mat, size_t rows, size_t columns, FILE *filePointer, float value) {
    *mat = matNew( rows, columns );
    /*float value;
    fscanf( filePointer, "%f", &value );*/

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

float genDamage(float ** lifeMat, float ** damageMat, float ** typeMat, int rowIndex, int colIndex){

    float damage;
    float killerType;
    int actualPokeType = typeMat[rowIndex][colIndex];
    int leftPokeType = typeMat[rowIndex-1][colIndex];
    int rightPokeType = typeMat[rowIndex+1][colIndex];
    int upPokeType = typeMat[rowIndex][colIndex-1];
    int downPokeType = typeMat[rowIndex][colIndex+1];
    int neighbours[] = {leftPokeType, rightPokeType, upPokeType, downPokeType};
    size_t i = 0;

    while(lifeMat[rowIndex][colIndex] > 0 && i<4){
        damage = damageMat[neighbours[i]][actualPokeType];
        lifeMat[rowIndex][colIndex] -= damage;
        if (lifeMat[rowIndex][colIndex] <= 0){
            killerType = neighbours[i];
            i = 0;
            return killerType;
        }else{
            i++;
        }
    }  
    return -1.0;  
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

//podria hacer que se peguen de a uno y crear un while el que recibe dano sigue vivo y falta que alguien le pegue, que le sigan pegando y sino que se cambie
// al tipo del ultimo que le pego (si se murio)
}

void actTypeMat(float ** oldTypeMat, float ** newTypeMat, float ** lifeMat, float ** damageMat, int rows, int columns, float initialLife){
    int neighboursType[4];
    int randNeighbour;
    float killer;
    for(size_t r=0; r<rows; r++){
        for (size_t c=0; c<rows; c++){
            if (oldTypeMat[r][c] != 3){
                if(lifeMat[r][c] > 0){
                    killer = genDamage(lifeMat, damageMat, oldTypeMat, r, c);
                    newTypeMat[r][c] = oldTypeMat[r][c];
                }if(lifeMat[r][c] <= 0 && killer != -1.0){
                    newTypeMat[r][c] = killer;
                    lifeMat[r][c] = initialLife;
                    
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
                new_mat[r][c] = 3.0;
           } else {
                new_mat[r][c] = 0.0;
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
    float initialLife;
    int const typesPokes = 4;

    filePointer = fopen( "text.txt", "r" ) ;
     
    if ( filePointer == NULL ) {
        printf( "file failed to open." ) ;
    } else {
        printf("The file is now opened.\n") ;
         
        fscanf( filePointer, "%d %d %f", &rows, &cols, &initialLife);
        printf( "<%d %d>\n", rows, cols) ;
        rows = rows + 2;
        cols = cols + 2;

        float **life;
        float **type;
        //float **modifLife;
        float **modifType;
        float **typeDamage;

        genLifeMat( &life, rows, cols, filePointer, initialLife); 
        genTypeMat( &type, rows, cols, filePointer );
        //genBlankMat( &modifLife, rows, cols, filePointer );
        genBlankMat( &modifType, rows, cols, filePointer );
        genTypeDamageMat(&typeDamage);
        
        



        for (size_t i = 0; i<25; i++){
        matShow( type, rows, cols );
        printf(" \n");
        /*matShow( type, rows, cols );
        printf(" \n");*/
        actTypeMat(type, modifType, life, typeDamage, rows, cols, initialLife);
        /*matShow( life, rows, cols );
        printf(" \n");
        matShow( modifType, rows, cols );
        printf(" \n");*/
        copyAndCleanMat(modifType, type, rows, cols);}
        /*matShow( modifType, rows, cols );
        matShow( typeDamage, typesPokes, typesPokes);*/


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