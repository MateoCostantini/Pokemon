#pragma once


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
}