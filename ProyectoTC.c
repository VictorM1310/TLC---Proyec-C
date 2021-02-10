#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "ArregloTokens.h"

int MAXT=0;
int MAXTL = 0;
int CPR = 0;
int COAR = 0;
int COR = 0;
int CASG = 0;
int CLN = 0;
int CIDX = 0;
int CIDXNR = 0;
int CTXT = 0;
int CER = 0;
int CIAS = 0;

int CargaTokens(char *nomArchivo, char Tokens[][256],arregloInt2D *aI)
{
    FILE *arch;
    arch = fopen (nomArchivo, "r");
	char linea[256];
    int i=0;
	if (arch != NULL)
	{
        int NumLinea = 1;
		while(fgets(linea, 255, arch)!=NULL)
        {
            char *s, *ptr;
            s = strtok_r(linea, " \t\n", &ptr);
            if (s[0])
            {
                do
                {
                    if (linea[0] == '#')
                    {
                        break;
                    }
                    if(s[0]=='"')
                    {
                        char arrayTemp[256];
                        memset(arrayTemp,0,256);
                        strcat(arrayTemp,s);
                        do
                        {
                            strcat(arrayTemp, " ");
                            s = strtok_r(NULL, " \t\n", &ptr);
                            if(!s)
                            {
                                break;
                            }
                            strcat(arrayTemp,s);
                        }while(*s!='"');
                        arrayTemp[strlen(arrayTemp)-1]='\0';
                        if(isspace(arrayTemp[strlen(arrayTemp)-1])!= 0)
                            arrayTemp[strlen(arrayTemp)-1]='\0';
                        strncpy (Tokens[i], arrayTemp, 255);
                        aI->B[i][1] = NumLinea;
                        aI->B[i][0] = i;
                        printf("Esta es la posicion %d: %s\n",i,Tokens[i]);
                        printf("La longitud de la cadena es: %ld\n",strlen(arrayTemp));
                        Tokens[i][strlen(arrayTemp)]='\0';
                        s=NULL;
                    }
                    else
                    {
                        if(isspace(s[strlen(s)-1])!=0)
                            s[strlen(s)-1] = '\0';
                        strncpy (Tokens[i], s, 255);
                        aI->B[i][1] = NumLinea;
                        aI->B[i][0] = i;
                        printf("La longitud de la cadena es: %ld\n",strlen(s));
                        printf("Esta es la posicion %d: %s\n",i,Tokens[i]);
                        s = strtok_r(NULL, " \t\n", &ptr);
                    }
                    i++;
                } while (s != NULL);
                printf("\n");
            }
            else
                printf("No hay token que leer\n");
            NumLinea++;
        }
        MAXT = i;
	}
	return 0;
}
int CompPalabraReserv(char Tokens[][256], unsigned int i)
{

    if(strcmp(Tokens[i],"PROGRAMA") == 0)
        return 0;
    if(strcmp(Tokens[i],"SI") == 0)
        return 0;
    if(strcmp(Tokens[i],"ENTONCES") == 0)
        return 0;
    if(strcmp(Tokens[i],"SINO") == 0)
        return 0;
    if(strcmp(Tokens[i],"FINSI") == 0)
        return 0;
    if(strcmp(Tokens[i],"REPITE") == 0)
        return 0;
    if(strcmp(Tokens[i],"VECES") == 0)
        return 0;
    if(strcmp(Tokens[i],"FINREP") == 0)
        return 0;
    if(strcmp(Tokens[i],"IMPRIME") == 0)
        return 0;
    if(strcmp(Tokens[i],"LEE") == 0)
        return 0;
    if(strcmp(Tokens[i],"FINPROG") == 0)
        return 0;

    return -1;
}
int CompOperRel(char Tokens[][256], unsigned int i)
{
    if(strcmp(Tokens[i],">") == 0)
        return 0;
    if(strcmp(Tokens[i],"<") == 0)
        return 0;
    if(strcmp(Tokens[i],"==") == 0)
        return 0;

    return -1;
}
int CompOperArit(char Tokens[][256],unsigned int i)
{
    if(strcmp(Tokens[i],"+") == 0)
        return 0;
    if(strcmp(Tokens[i],"-") == 0)
        return 0;
    if(strcmp(Tokens[i],"*") == 0)
        return 0;
    if(strcmp(Tokens[i],"/") == 0)
        return 0;

    return -1;
}

int CompAsig(char Tokens[][256],unsigned int i)
{
    if(strcmp(Tokens[i],"=") == 0)
        return 0;

    return -1;
}

int CompLiteralNum(char Tokens[][256],unsigned int i, arregloInt2D *aI)
{
    unsigned int tama = 0;
    unsigned int DigitoInvalido = 0;
    tama = strlen(Tokens[i]);
    for(int j=0; j<tama; j++)
    {
        if (isdigit(Tokens[i][j])==0)
            return -1;
        if(Tokens[i][j] == '8'||Tokens[i][j] == '9')
            DigitoInvalido = 1;
    }
    if(DigitoInvalido == 1)
    {
        int Error = 0;
        int Ubi = 0;
        for (int k = 0; k < MAXT; k++)
        {
            if (strncmp(Tokens[i],Tokens[k],strlen(Tokens[i])) == 0)
            {
                Ubi = k;
                break;
            }
        }
        for(int j = 0; j<MAXT; j++)
        {
            if (aI->B[j][0] == Ubi)
            {
                Error = aI->B[j][1];
                break;
            }
        }
        printf("Error: notacion decimal no permitida en token (%s) en la linea %d\n",Tokens[i],Error);

        return -1;
    }

    return 0;
}
int CompIdent(char Tokens[][256],unsigned int i, arregloChar2D *aC,arregloInt2D *MTokLin)
{
    if(((Tokens[i][0] != '"') && (Tokens[i][strlen(Tokens[i])-1] == '"'))||((Tokens[i][0] == '"') && (Tokens[i][strlen(Tokens[i])-1] != '"')))
    {
        int Error = 0;
        int Ubi = 0;
        for (int k = 0; k < MAXT; k++)
        {
            if (strncmp(Tokens[i],Tokens[k],strlen(Tokens[k])) == 0)
            {
                Ubi = k;
                break;
            }
        }
        for(int j = 0; j<MAXT; j++)
        {
            if (MTokLin->B[j][0] == Ubi)
            {
                Error = MTokLin->B[j][1];
                break;
            }
        }
        printf("Error: faltan comillas de terminacion en el token (%s) ubicado en la linea %d\n",Tokens[i],Error);
    }

    if((isdigit(Tokens[i][0]) == 0)&&(isalpha(Tokens[i][0])!=0))
        return 0;
    return -1;
}
int BuscaIndice(int i, char Tokens[][256], arregloChar2D *IDXNR)
{
    int TokenRep = 0;
    int IndiceNR = 0;
    if(IDXNR)
    {
        TokenRep = 0;
        for(int j = 0; j<CIDXNR; j++)
        {
            if (strncmp(IDXNR->A[j],Tokens[i],strlen(IDXNR->A[j])) == 0)
            {
                TokenRep=1;
                IndiceNR = j+1;
            }
        }
        if(TokenRep==0)
        {
            strncpy(IDXNR->A[CIDXNR],Tokens[i],16);
            IndiceNR = CIDXNR+1;
            CIDXNR++;
        }
    }
    else
    {
        strncpy(IDXNR->A[CIDXNR],Tokens[i],16);
        CIDXNR++;
        return 1;
    }
    return IndiceNR;
}

int octalTOdecimal(arregloChar2D *LN, unsigned int i)
{
    int decimal = 0;
    int num = 0;
    int k = strlen(LN->A[i]);
    for(int j = 0; j < strlen(LN->A[i]); j++)
    {
        num = LN->A[i][j] - '0';
        decimal = decimal + (num * pow(8,k-1));
        k--;
    }
    return decimal;
}

void ClasificaTokens(FILE *ArchivoLex, char Tokens[][256], unsigned int i, arregloChar2D *PR, arregloChar2D *OAR, arregloChar2D *OR,arregloChar2D *ASG, arregloChar2D *LN, arregloChar2D *IDX, arregloChar2D *TXT,arregloChar2D *Errores, arregloInt2D *MTokLin,arregloChar2D *IDXNR)
{
    if(CompPalabraReserv((Tokens),i) == 0)
    {
        strncpy(PR->A[CPR], Tokens[i],strlen(Tokens[i]));
        CPR++;
        fprintf(ArchivoLex,"%s\n",Tokens[i]);
    }
    else
    {
        if(CompOperArit((Tokens),i) == 0)
        {
            strncpy(OAR->A[COAR], Tokens[i],strlen(Tokens[i]));
            COAR++;
            fprintf(ArchivoLex,"[op_ar]\n");
        }
        else
        {
            if(CompOperRel((Tokens),i) == 0)
            {
                strncpy(OR->A[COR], Tokens[i],strlen(Tokens[i]));
                COR++;
                fprintf(ArchivoLex,"[op_r]\n");
            }
            else
            {
                if(CompAsig((Tokens),i) == 0)
                {
                    strncpy(ASG->A[CASG], Tokens[i],strlen(Tokens[i]));
                    CASG++;
                    fprintf(ArchivoLex,"=\n");
                }
                else
                {
                    if(CompLiteralNum((Tokens),i,MTokLin) == 0)
                    {
                        strncpy(LN->A[CLN], Tokens[i],strlen(Tokens[i]));
                        CLN++;
                        fprintf(ArchivoLex,"[val]\n");
                    }
                    else
                    {
                        if(CompIdent((Tokens),i,Errores,MTokLin) == 0)
                        {
                            const char *ET = "[id]";
                            const char *ID1 = "ID0";
                            const char *ID2 = "ID";
                            if(Tokens[i][strlen(Tokens[i])-1]!='"')
                            {
                                if(strlen(Tokens[i])>16)
                                {
                                    printf("Advertencia: su identificador %s es mayor a 16 caracteres, solo se tomaran en cuenta los primeros 16\n",Tokens[i]);
                                    strncpy(IDX->A[CIDX],Tokens[i],16);

                                }
                                else
                                {
                                    strncpy(IDX->A[CIDX],Tokens[i],strlen(Tokens[i]));
                                }
                                int Valor = 0;
                                Valor = BuscaIndice(i,Tokens,IDXNR);

                                if(Valor>0 && Valor<=9)
                                    fprintf(ArchivoLex,"%s%s%d\n",ET,ID1,Valor);
                                else
                                {
                                    if(Valor>9)
                                        fprintf(ArchivoLex,"%s%s%d\n",ET,ID2,Valor);
                                }
                                CIDX++;
                            }
                        }
                        else
                        {
                            if((Tokens[i][0]=='"') && (Tokens[i][strlen(Tokens[i])-1]=='"'))
                            {
                                const char *ETQ = "[txt]";
                                const char *TX1 = "TX0";
                                const char *TX2 = "TX";
                                strncpy(TXT->A[CTXT], Tokens[i],strlen(Tokens[i]));
                                if(CTXT<9)
                                    fprintf(ArchivoLex,"%s%s%d\n",ETQ,TX1,CTXT+1);
                                else
                                    fprintf(ArchivoLex,"%s%s%d\n",ETQ,TX2,CTXT+1);
                                CTXT++;
                            }
                            else
                            {
                                if(Tokens[i][0]!='"')
                                {
                                    int NoImp=0;
                                    for(int x=0; x<strlen(Tokens[i]);x++)
                                        if(Tokens[i][x]=='8'||Tokens[i][x]=='9')
                                        {
                                            NoImp=-1;
                                        }
                                    if(NoImp==0)
                                    {
                                        strncpy(Errores->A[CER],Tokens[i],strlen(Tokens[i]));
                                        CER++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
void ImprimeSim(FILE *ArchivoSim,int CIDXNR,arregloChar2D *IDXNR,arregloChar2D *LN, arregloChar2D *TXT)
{
    const char *ID1 = "ID0";
    const char *ID2 = "ID";
    const char *TX1 = "TX0";
    const char *TX2 = "TX";

    fprintf(ArchivoSim,"IDS\n");
    for(int i = 0; i<CIDXNR;i++)
    {
        if(i<9)
            fprintf(ArchivoSim,"%s,%s%d\n",IDXNR->A[i],ID1,i+1);
        else
        {
            if(i>=9)
                fprintf(ArchivoSim,"%s,%s%d\n",IDXNR->A[i],ID2,i+1);
        }
    }
    fprintf(ArchivoSim,"\n");

    fprintf(ArchivoSim,"TXT\n");
    for(int i = 0; i<CTXT;i++)
    {
        if(i<9)
            fprintf(ArchivoSim,"%s,%s%d\n",TXT->A[i],TX1,i+1);
        else
            fprintf(ArchivoSim,"%s,%s%d\n",TXT->A[i],TX2,i+1);

    }
    fprintf(ArchivoSim,"\n");

    fprintf(ArchivoSim,"VAL\n");
    for(int i = 0; i<CLN;i++)
    {
        fprintf(ArchivoSim,"%s,%d\n",LN->A[i],octalTOdecimal(LN,i));
    }
    fprintf(ArchivoSim,"\n");
}

int BuscaErroLinea(int i, arregloInt2D *aI, char Tokens[][256], arregloChar2D *aC)
{
    int ErrorLinea = 0;
    int Ubicacion = 0;
    for (int k = 0; k < MAXT; k++)
    {
        if (strncmp(aC->A[i],Tokens[k],strlen(aC->A[i])) == 0)
        {
            Ubicacion = k;
            break;
        }
    }
    for(int j = 0; j<MAXT; j++)
    {
        if (aI->B[j][0] == Ubicacion)
        {
            ErrorLinea = aI->B[j][1];
            break;
        }
    }
    return ErrorLinea;
}
int CargaTokensLex(char *nomArchivo, arregloChar2D *TokensLex)
{
    FILE *arch;
    arch = fopen (nomArchivo, "r");
	char linea[256];
    int i=0;
	if (arch != NULL)
	{
		while(fgets(linea, 255, arch)!=NULL)
        {
            char *s, *ptr;
            s = strtok_r(linea, " \t\n", &ptr);
            if (s[0])
            {
                do
                {
                    if(isspace(s[strlen(s)-1])!=0)
                        s[strlen(s)-1] = '\0';
                    strncpy (TokensLex->A[i], s, 255);
                    printf("La longitud de la cadena es: %ld\n",strlen(s));
                    printf("Esta es la posicion %d: %s\n",i,TokensLex->A[i]);
                    s = strtok_r(NULL, " \t\n", &ptr);
                    i++;
                } while (s != NULL);
                printf("\n");
            }
            else
                printf("No hay token que leer\n");

        }
	}
	MAXTL = i;
	return 0;
}

int IndiceLinea (int Ubicacion, arregloInt2D *MTokLin)
{
    int ErrorLinea = 0;
    for(int j = 0; j<MAXT; j++)
            {
                if (MTokLin->B[j][0] == Ubicacion)
                    {
                        ErrorLinea = MTokLin->B[j][1];
                        break;
                    }
            }
    return ErrorLinea;
}

void BuscaProgFP(arregloChar2D *TokensLex,arregloInt2D *MTokLin)
{
    int ErrorP = 0;
    int NProg = 0;
    for(int i=0;i<MAXTL;i++)
    {
        if(strcmp(TokensLex->A[i],"PROGRAMA")==0)
        {
            NProg++;
            if(i !=0)
            {
                int Ubicacion = i;
                printf("Error: Ubicacion de la palabra reservada (PROGRAMA) incorrecta en la linea %d\n",IndiceLinea(Ubicacion,MTokLin));

                if(NProg>1)
                    printf("Error: Multiples definiciones de la palabra reservada (PROGRAMA) en la linea %d\n",IndiceLinea(Ubicacion,MTokLin));
            }
            else
                ErrorP = 1;
        }
    }
    if (ErrorP == 0)
        printf("Error: No se encontro la palabra reservada (PROGRAMA) en la linea 1\n");

    ErrorP = 0;
    int NFProg = 0;
    for(int i=0;i<MAXTL;i++)
    {
        if(strcmp(TokensLex->A[i],"FINPROG")==0)
        {
            NFProg++;
            if(i != (MAXTL-1))
            {
                int Ubicacion = i;
                printf("Error: Ubicacion de la palabra reservada (FINPROG) incorrecta en la linea %d\n",IndiceLinea(Ubicacion,MTokLin));
                if(NFProg>1)
                    printf("Error: Multiples definiciones de la palabra reservada (FINPROG) en la linea %d\n",IndiceLinea(Ubicacion,MTokLin));
            }
            else
                ErrorP = 1;
        }
    }
    if (ErrorP == 0)
        printf("Error: No se encontro la palabra reservada (FINPROG) en la linea %d\n",MTokLin->B[MAXTL-1][1]);

}
void VerificaAsig(arregloChar2D *TokensLex,arregloInt2D *MTokLin, int *IAsign)
{
    int Ubi = 0;
    for(int i = 0; i <MAXTL; i++)
    {
        if(strcmp(TokensLex->A[i],"=")==0)
        {
            IAsign[CIAS] = i;
            CIAS++;
        }
    }
    for (int j = 0; j<CIAS; j++)
    {
        if(strncmp(TokensLex->A[IAsign[j]-1],"[id]",4)!=0)
        {
            Ubi = IAsign[j] -1;
            printf("Error: Solo se puede usar la asignacion con identificadores, linea %d\n",IndiceLinea(Ubi,MTokLin));
        }
        if(strncmp(TokensLex->A[IAsign[j]+1],"[val]",5)!=0 && strncmp(TokensLex->A[IAsign[j]+1],"[id]",4)!=0)
        {
            Ubi = IAsign[j] + 1;
            printf("Error: Solo se puede asignar a identificadores o valores, linea %d\n",IndiceLinea(Ubi,MTokLin));
        }
    }

}
void VerificaImprime(arregloChar2D *TokensLex,arregloInt2D *MTokLin, int *IAsign)
{
    int Ubi = 0;
    for(int i = 0; i <MAXTL; i++)
    {
        Ubi = i;
        if(strcmp(TokensLex->A[i],"IMPRIME")==0 && (IndiceLinea(Ubi,MTokLin)!=IndiceLinea(Ubi-1,MTokLin)))
        {
            IAsign[CIAS] = i;
            CIAS++;
        }
        else
        {
            if(strcmp(TokensLex->A[i],"IMPRIME")==0 && (IndiceLinea(Ubi,MTokLin)==IndiceLinea(Ubi-1,MTokLin)))
            {
                printf("Error: La palabra reservada (IMPRIME) no esta al inicio de la linea, en la linea %d\n",IndiceLinea(Ubi,MTokLin));
            }
        }
    }

    for (int j = 0; j<CIAS; j++)
    {
        if(((strncmp(TokensLex->A[IAsign[j]+1],"[val]",5)!=0) && (strncmp(TokensLex->A[IAsign[j]+1],"[id]",4)!=0) && (strncmp(TokensLex->A[IAsign[j]+1],"[txt]",5)!=0)&& (IndiceLinea(IAsign[j],MTokLin)==IndiceLinea(IAsign[j]+1,MTokLin))))
        {
            Ubi = IAsign[j] + 1;
            printf("Error: Solo se puede imprimir un identificador, valor o un texto linea %d\n",IndiceLinea(Ubi,MTokLin));
        }
    }
}
void VerificaOperAr(arregloChar2D *TokensLex,arregloInt2D *MTokLin, int *IAsign)
{
    int Ubi = 0;
    for(int i = 0; i <MAXTL; i++)
    {
        Ubi = i;
        if((strncmp(TokensLex->A[i],"[op_ar]",7)==0)&&(IndiceLinea(Ubi,MTokLin)==IndiceLinea(Ubi-1,MTokLin)))
        {
            IAsign[CIAS] = i;
            CIAS++;
        }
        else
        {
            if((strncmp(TokensLex->A[i],"[op_ar]",7)==0)&&(IndiceLinea(Ubi,MTokLin)!=IndiceLinea(Ubi-1,MTokLin)))
            {
                printf("Error: El operador aritmetico no puede estar al inicio de la linea, en la linea %d\n",IndiceLinea(Ubi,MTokLin));
            }
        }
    }
    for (int j = 0; j<CIAS; j++)
    {
        if(strncmp(TokensLex->A[IAsign[j]-1],"[val]",5)!=0 && strncmp(TokensLex->A[IAsign[j]-1],"[id]",4)!=0)
        {
            Ubi = IAsign[j] -1;
            printf("Error: Solo se puede usar operador aritmetico con identificadores o valores, linea %d\n",IndiceLinea(Ubi,MTokLin));
        }
        if(strncmp(TokensLex->A[IAsign[j]+1],"[val]",5)!=0 && strncmp(TokensLex->A[IAsign[j]+1],"[id]",4)!=0)
        {
            Ubi = IAsign[j] + 1;
            printf("Error: Solo se puede usar operador aritmetico con identificadores o valores, linea %d\n",IndiceLinea(Ubi,MTokLin));
        }
    }
}
void VerificaOperRel(arregloChar2D *TokensLex,arregloInt2D *MTokLin, int *IAsign)
{
    int Ubi = 0;
    for(int i = 0; i <MAXTL; i++)
    {
        Ubi = i;
        if((strncmp(TokensLex->A[i],"[op_r]",6)==0)&&(IndiceLinea(Ubi,MTokLin)==IndiceLinea(Ubi-1,MTokLin)))
        {
            IAsign[CIAS] = i;
            CIAS++;
        }
        else
        {
            if((strncmp(TokensLex->A[i],"[op_r]",6)==0)&&(IndiceLinea(Ubi,MTokLin)!=IndiceLinea(Ubi-1,MTokLin)))
            {
                printf("Error: El operador relacional no puede estar al inicio de la linea, en la linea %d\n",IndiceLinea(Ubi,MTokLin));
            }
        }
    }
    for (int j = 0; j<CIAS; j++)
    {
        if(strncmp(TokensLex->A[IAsign[j]-1],"[id]",4)!=0)
        {
            Ubi = IAsign[j] -1;
            printf("Error: Antes de un operador relacional solo se pueden utilizar identificadores, linea %d\n",IndiceLinea(Ubi,MTokLin));
        }
        if(strncmp(TokensLex->A[IAsign[j]+1],"[val]",5)!=0 && strncmp(TokensLex->A[IAsign[j]+1],"[id]",4)!=0)
        {
            Ubi = IAsign[j] + 1;
            printf("Error: Solo se puede usar operador relacional con identificadores o valores, linea %d\n",IndiceLinea(Ubi,MTokLin));
        }
    }
}
int main(int argc, char **argv)
{
    FILE *ArchivoLex;
    FILE *ArchivoSim;
    char Tokens[100][256];
    int r = 100, c = 256;
    int r1 = 100, c1 = 2;

    arregloChar2D PR;
    arregloChar2D OAR;
    arregloChar2D OR;
    arregloChar2D ASG;
    arregloChar2D LN;
    arregloChar2D IDX;
    arregloChar2D TXT;
    arregloChar2D IDXNR;
    arregloChar2D Errores;
    arregloInt2D MTokLin;

    initArregloChar2D(&PR,r,c);
    initArregloChar2D(&OAR,r,c);
    initArregloChar2D(&OR,r,c);
    initArregloChar2D(&ASG,r,c);
    initArregloChar2D(&LN,r,c);
    initArregloChar2D(&IDX,r,c);
    initArregloChar2D(&TXT,r,c);
    initArregloChar2D(&IDXNR,r,c);
    initArregloInt2D(&MTokLin,r1,c1);
    initArregloChar2D(&Errores,r,c);

    int *IAsign;
    IAsign = malloc(50*(sizeof (int)));

	if (argc>1)
    {
      if (CargaTokens(argv[1], Tokens,&MTokLin) < 0)
         exit(1);
    }
    else
    {
      fprintf(stderr, "Error: Se tiene que pasar como argumento el archivo que contiene la base de datos\n");
      exit(1);
    }

    printf("Inicia la comparacion \n\n");

    for (int i=0;i <MAXT;i++)
    {
        printf("El token en Tokens[%d] es %s\n",i,Tokens[i]);
        printf("La longitud de Tokens[%d] es %ld\n\n",i,strlen(Tokens[i]));
    }

    int i=0;

    ArchivoLex = fopen("Archivo.lex","w");
    while(i<MAXT)
    {
        ClasificaTokens(ArchivoLex,Tokens, i,&PR,&OAR,&OR,&ASG,&LN,&IDX,&TXT,&Errores,&MTokLin,&IDXNR);
        i++;
    }
    fclose(ArchivoLex);
    printf("\n\n");
    printf("Arreglo PR\n");
    for (int i; i < CPR;i++)
    {
        printf("En PR[%d] esta: %s\n",i,PR.A[i]);
    }
    printf("\n\n");

    printf("Arreglo OAR\n");
    for (int i; i < COAR;i++)
    {
        printf("En OAR[%d] esta: %s\n",i,OAR.A[i]);
    }
    printf("\n\n");

    printf("Arreglo OR\n");
    for (int i; i < COR;i++)
    {
        printf("En OR[%d] esta: %s\n",i,OR.A[i]);
    }
    printf("\n\n");

    printf("Arreglo ASG\n");
    for (int i; i < CASG;i++)
    {
        printf("En ASG[%d] esta: %s\n",i,ASG.A[i]);
    }
    printf("\n\n");

    printf("Arreglo LN\n");
    for (int i; i < CLN;i++)
    {
        printf("En LN[%d] esta: %s\n",i,LN.A[i]);
    }
    printf("\n\n");

    printf("Arreglo IDX\n");
    for (int i; i < CIDX;i++)
    {
        printf("En IDX[%d] esta: %s\n",i,IDX.A[i]);
    }
    printf("\n\n");

    printf("Arreglo TXT\n");
    for (int i; i < CTXT;i++)
    {
        printf("En TXT[%d] esta: %s\n",i,TXT.A[i]);
    }
    printf("\n\n");

    printf("Arreglo IDXNR\n");
    for (int i; i < CIDXNR;i++)
    {
        printf("En IDXNR[%d] esta: %s\n",i,IDXNR.A[i]);
    }

    printf("\n\n");
    ArchivoSim = fopen("Archivo.sim","w");
    ImprimeSim(ArchivoSim,CIDXNR,&IDXNR,&LN,&TXT);
    fclose(ArchivoSim);

    printf("Matriz de errores con la forma indice/linea\n");
    for (int i; i < MAXT;i++)
    {
        printf("%d %d\n",MTokLin.B[i][0],MTokLin.B[i][1]);
    }

    for (int i; i < CER;i++)
    {
        printf("Error: el token %s no es una variable alfanumerica, en la linea %d\n",Errores.A[i],BuscaErroLinea(i,&MTokLin,Tokens,&Errores));
    }


    arregloChar2D TokensLex;
    initArregloChar2D(&TokensLex,r,c);
    CargaTokensLex("Archivo.lex", &TokensLex);

    printf("\n\n");
    for (int i; i < MAXTL; i++)
    {
        printf("En TokensLex[%d] esta: %s\n",i,TokensLex.A[i]);
    }
    printf("\n\n");
    BuscaProgFP(&TokensLex,&MTokLin);
    printf("\n\n");

    VerificaAsig(&TokensLex, &MTokLin, IAsign);
    printf("\n\n");
    CIAS = 0;
    free(IAsign);

    IAsign = malloc(50*(sizeof (int)));
    VerificaImprime(&TokensLex, &MTokLin, IAsign);
    printf("\n\n");

    CIAS = 0;
    free(IAsign);

    IAsign = malloc(50*(sizeof (int)));
    VerificaOperAr(&TokensLex, &MTokLin, IAsign);
    printf("\n\n");

    CIAS = 0;
    free(IAsign);

    IAsign = malloc(50*(sizeof (int)));
    VerificaOperRel(&TokensLex, &MTokLin, IAsign);
    printf("\n\n");

    liberaArregloChar2D(&PR);
    liberaArregloChar2D(&OAR);
    liberaArregloChar2D(&OR);
    liberaArregloChar2D(&ASG);
    liberaArregloChar2D(&LN);
    liberaArregloChar2D(&IDX);
    liberaArregloChar2D(&TXT);
    liberaArregloChar2D(&IDXNR);
    liberaArregloInt2D(&MTokLin);
    liberaArregloChar2D(&Errores);
    free(IAsign);
    return 0;
}
