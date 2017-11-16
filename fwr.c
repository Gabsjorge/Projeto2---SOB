#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *arq;
    char fileName[20], Linha[100], *leitura;
    int result, i;

    if(argc > 1 && argc < 5)
    {
        if(*argv[1] == 'w')
        {
            strcpy(fileName, "/mnt/point1/");
            strcat(fileName, argv[2]);
            strcat(fileName,".txt");
            arq = fopen(fileName,"wt");
            if(arq == NULL)
            {
                printf("Problemas na criação do arquivo");
                return 0;
            }
            else {
                result = fputs(argv[3], arq);
                if (result == EOF)
                printf("Erro na Gravacao\n");
                fclose(arq);
                return 1;
            }
        }

        else if(*argv[1] == 'r')
        {
            strcpy(fileName,argv[2]);
            strcat(fileName,".txt");
            arq = fopen(fileName,"rt");
            if(!arq)
            {
                printf("Problemas na abertura do arquivo");
                return 0;
            }
            else {
                i = 1;
                while (!feof(arq))
                {
                    fflush(stdout);
                    leitura = fgets(Linha, 100, arq);
                    if (leitura){
                        
                        printf("%s\n",Linha);
                    }
                    i++;
                }
                fclose(arq);
                return 1;
            }
        }
    }
}