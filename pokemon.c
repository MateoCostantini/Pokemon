#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "createPPM.h"
 

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



void genLifeMat( float ***mat, size_t rows, size_t columns, float value) {
    *mat = matNew( rows, columns );

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

void genTypeMat(float ***mat, size_t rows, size_t columns){
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


void genBlankMat(float ***mat, size_t rows, size_t columns){
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
}

void actTypeMat(float ** oldTypeMat, float ** newTypeMat, float ** lifeMat, float ** damageMat, int rows, int columns, float initialLife){
    int neighboursType[4];
    int randNeighbour;
    float killer;
    for(size_t r=0; r<rows; r++){
        for (size_t c=0; c<columns; c++){
            if (oldTypeMat[r][c] != 3.0){
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

void matFree(float ***mat, size_t rows, size_t cols) {
    float **_m;

    if (mat) {
        _m = *mat;
        while (rows--) {
            free(_m[rows]);
        }
        free(_m);
        *mat = NULL;
    }
}

int play(int *parseVals) {
    //FILE *filePointer ;
    int rows, cols;
    float initialLife;

    float **life;
    float **type;
    float **modifType;
    float **typeDamage;
    //int const typesPokes = 4;

    //filePointer = fopen( "text.txt", "r" ) ;
     
    if ( parseVals == NULL ) {
        printf( "fail in arguments given." ) ;
    } else {
        printf("arguments success.\n") ;
         
        //fscanf( filePointer, "%d %d %f", &rows, &cols, &initialLife);
        //printf( "<%d %d>\n", rows, cols) ;
        //rows = rows + 2;
        //cols = cols + 2;
        rows = parseVals[1]+2;
        cols = parseVals[2]+2;
        initialLife = parseVals[0];

        genLifeMat( &life, rows, cols, initialLife); 
        genTypeMat( &type, rows, cols);
        //genBlankMat( &modifLife, rows, cols, filePointer );
        genBlankMat( &modifType, rows, cols );
        genTypeDamageMat(&typeDamage);

        matShow( type, rows, cols );
        printf(" \n");
        //genPPM(type, rows, cols);


        for (size_t i = 0; i<parseVals[5]; i++){
        actTypeMat(type, modifType, life, typeDamage, rows, cols, initialLife);
        copyAndCleanMat(modifType, type, rows, cols);
        }

        matShow( life, rows, cols );
        puts("");
        matShow( type, rows, cols );
        //genPPM(type, rows, cols);

        /*matShow( modifType, rows, cols );
        matShow( typeDamage, typesPokes, typesPokes);*/

        //duplicateEx( provided, rows, cols );

        matFree( &life, rows, cols );
        matFree( &type, rows, cols );
        matFree( &modifType, rows, cols );
        matFree( &typeDamage, 4, 4);
    }
}


void *parser_arguments(int argc, char*argv[], char *helpMsg, int *argValues, bool *parseSuccessPtr){
    int maxLife, rows, columns, seed, ppmPrint;
    char *pFlags[] = {"-l", "--life", "-h", "--height", "-w", "--width", "-s", "--seed", "-n", "--nPPMprint", "-r", "--repetitions"};
    int pFlagsLength = 12;
    int compare;
    int argValueIndex;
    int value;
    char *leftover;
    int aux = 0;

    if(argc == pFlagsLength +1){
        for(size_t i = 1; i < argc-1; i+=2){
            for(size_t j = 0; j < pFlagsLength; j++){
                compare = strcmp(argv[i], pFlags[j]);
                if (compare == 0){
                    aux++;
                    argValueIndex = j/2;
                    value = strtod(argv[i+1], &leftover);
                    argValues[argValueIndex] = value;
                }
            }
        }
        if(aux == pFlagsLength/2){
            *parseSuccessPtr = true;
        }
    }if(argc!= pFlagsLength+1 || aux!=pFlagsLength/2){
        printf("%s", helpMsg);
            argValues = NULL;
            return NULL;
    }
    /*if (argc == 2){
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
            printf("%s", helpMsg);
            argValues = NULL;
            return NULL;
        }else{
            argValues = NULL;
            return NULL;
        }
    }else else{
        argValues = NULL;
        return NULL;
    }*/
}


 
int main(int argc, char *argv[]) {
    // save();
    bool *parseSuccessPtr;
    bool  parseSuccess = false;
    parseSuccessPtr = &parseSuccess;
    int *parseVals;
    char helpMsg[] = "helpppp\n";
    parser_arguments(argc, argv, helpMsg, parseVals, parseSuccessPtr);
    if (*parseSuccessPtr == true){
        srand(parseVals[3]);
        play(parseVals);
    }
    else{
        return 1;
    }
    
}