#pragma once


void genPPM(float **typeMat, int rows, int cols){
    const int maxValue = 255;
    printf("P3 %i %i %i ", rows-2, cols-2, maxValue);
    for (size_t i=0; i<rows; i++){
        for (size_t j =0; j<cols; j++){
            if(typeMat[i][j]==0.0){
                printf("255 0 0     ");
            }else if(typeMat[i][j]==1.0){
                printf("0 0 255     ");
            }else if(typeMat[i][j]==2.0){
                printf("0 255 0     ");
            }
        }
        
    }
}
