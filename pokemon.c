#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "createPPM.c"
#include "argParser.c"
 

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



void actTypeMat(float ** oldTypeMat, float ** newTypeMat, float ** lifeMat, float ** damageMat, int rows, int columns, float initialLife){
    int neighboursType[4];
    //int randNeighbour;
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
    int rows, cols;
    float initialLife;
    int checkPPMCycle = 0;

    float **life;
    float **type;
    float **modifType;
    float **typeDamage;
     
    if ( parseVals == NULL ) {
        printf( "fail in arguments given." ) ;
    } else {
        printf("arguments success.\n") ;

        rows = parseVals[1]+2;
        cols = parseVals[2]+2;
        initialLife = parseVals[0];

        genLifeMat( &life, rows, cols, initialLife); 
        genTypeMat( &type, rows, cols);
        genBlankMat( &modifType, rows, cols );
        genTypeDamageMat(&typeDamage);

        for (size_t i = 0; i<parseVals[5]; i++){
        checkPPMCycle++;
        actTypeMat(type, modifType, life, typeDamage, rows, cols, initialLife);
        copyAndCleanMat(modifType, type, rows, cols);
        if (checkPPMCycle == parseVals[4]){
            genPPM(type, rows, cols);
            }
        }

        matFree( &life, rows, cols );
        matFree( &type, rows, cols );
        matFree( &modifType, rows, cols );
        matFree( &typeDamage, 4, 4);
    }
}


 
int main(int argc, char *argv[]) {
    // save();
    bool *parseSuccessPtr;
    bool  parseSuccess = false;
    parseSuccessPtr = &parseSuccess;
    int parseVals[6];
    char helpMsg[300] = "Each of the following flags must be in use to run the program\n-l\tThe life with which the pokemons start the game.\n-r\tamount of rows in the map.\n-c\tAmount of columns in the map.\n-s\tThe seed to change the ramdom. It is used to modify the position of the pokemons.\n-p\tCycle of the PPM\n";
    parser_arguments(argc, argv, helpMsg, parseVals, parseSuccessPtr);
    if (*parseSuccessPtr == true){
        srand(parseVals[3]);
        play(parseVals);
    }
    else{
        return 1;
    }
    
}