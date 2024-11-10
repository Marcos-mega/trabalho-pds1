#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X 39
#define Y 23

struct ListaDeRestaurantes {
    char nome[Y];
    char preco[7];
    float tempo;
    int linha;
    int coluna;
    int velocidade;
    int distancia;
};

void distancia (int matriz_base[X][Y], int matriz_distancia[X][Y], int i, int j, int k) {
    if ((matriz_distancia [i][j] == 0) && ((i < X) && (i > -1)) && ((j < Y) && (j > -1))) { 
        if (matriz_base [i][j] == 1) {
            matriz_distancia [i][j] = k;
            k++;
            distancia (matriz_base, matriz_distancia, i+1, j, k);
            distancia (matriz_base, matriz_distancia, i, j+1, k);
            distancia (matriz_base, matriz_distancia, i-1, j, k);
            distancia (matriz_base, matriz_distancia, i, j-1, k);
        } else {
            matriz_distancia [i][j] = -1;
        }
    }
}

void distancia_restaurante (int matriz[X][Y], FILE* arquivo, struct ListaDeRestaurantes lista[Y]) {
    int i = 0;
    struct ListaDeRestaurantes iterador[1];
    printf("Distancia em zambs de sua casa ao restaurante:\n");
    while (fscanf (arquivo, "%d %d %22s %6s %d", &lista[i].linha, &lista[i].coluna, lista[i].nome, lista[i].preco, &lista[i].velocidade) == 5) {
        lista [i].distancia = matriz [lista[i].linha-1][lista[i].coluna-1];
        lista [i].tempo = (float)lista[i].distancia / lista[i].velocidade;
        printf("%d- %d zambs, %s\n", i+1, lista[i].distancia, lista[i].nome);
        i ++;
    }   
    for (int k = 0; k < Y; k++) {
        for (int j = 0; j < Y-1; j++) {
            if (lista[j].tempo > lista[j+1].tempo) {
                iterador[0] = lista[j];
                lista[j] = lista[j+1];
                lista[j+1] = iterador[0];
            }
        }
    }
}

void print_lista (float tempo_maximo, char preco[7], struct ListaDeRestaurantes lista[Y]) {
    int i = 0;
    int k = 1; 
    if (tempo_maximo < 0) {
        printf("Essa eh a lista dos restaurantes de sua preferencia selecionados por velocidade de entrega:\n");
        while (i < Y) {
            if (strcmp (preco, lista[i].preco) == 0) {

                printf("%d. %s: %.2f zambs/min\n", k, lista[i].nome, lista[i].tempo);
                k ++;
            }
            i ++;
        } 
    } else {
        printf("Essa eh a lista dos restaurantes de sua preferencia selecionados por tempo maximo de entrega preferida:\n");
        while (i < Y) {
            if (lista[i].tempo <= tempo_maximo && strcmp (preco, lista[i].preco) == 0) {
                printf("%d. %s: %.2f zambs/min\n", k, lista[i].nome, lista[i].tempo);
                k ++;
            }
            i++;
        }
    }
}

int main () {
    int linha = 0;
    int coluna = 0;
    int quantidade_linhas = 0;
    int quantidade_colunas = 0;
    int pavimentado = 0;
    int linhas_txt = 0;
    int numero;
    int cidade [X][Y];
    int matriz_distancia [X][Y];
    char preco[7];
    float tempo_maximo = -1.0;
    struct ListaDeRestaurantes lista[Y];
    FILE* f_ruas = fopen ("ruas.txt", "r+t");
    if (f_ruas == NULL) {
		printf("Erro ao abrir o arquivo ruas.txt.\n");
		return(1);
	}
    while (fscanf(f_ruas, "%d", &numero) == 1) {
        linhas_txt++;
    }
    fseek (f_ruas, 0, SEEK_SET);
    for (int i = 0; i < linhas_txt; i++) {
        fscanf(f_ruas, "%d %d %d", &quantidade_linhas, &quantidade_colunas, &pavimentado);
        cidade[quantidade_linhas-1][quantidade_colunas-1] = pavimentado;
    } 
    fclose (f_ruas);
    printf ("Digite as coordenadas da sua casa: \n");
    do {
        scanf ("%d %d", &linha, &coluna);
        if (cidade[linha-1][coluna-1] == 0 || linha > X || linha < 0 || coluna > Y || coluna < 0) {
            printf("Rua nao pavimentada ou inexistente, nao existe suporte de entrega para seu endereco. Entre com endereco valido:\n");
        }
    } while (cidade[linha-1][coluna-1] == 0 || linha > X || linha < 0 || coluna > Y || coluna < 0);
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            matriz_distancia[i][j] = 0;
        }
    }
    distancia (cidade, matriz_distancia, linha-1, coluna-1, 0);
    matriz_distancia [linha-1][coluna-1] = 0;
    FILE* f_restaurantes = fopen ("restaurantes.txt", "r+t");
    if (f_restaurantes == NULL) {
        printf ("Erro ao abrir o arquivo restaurantes.txt.\n");
        return (1);
    }
    printf("\n____________________________________________________________________\n\n1.)\n\n");
    distancia_restaurante (matriz_distancia, f_restaurantes, lista);
    fclose (f_restaurantes);
    printf("\n____________________________________________________________________\n\n2.)\n\nDigite sua preferencia por preco de restaurante (Caro ou Barato): \n");
    do {
        scanf("%6s", preco); 
        if (strcmp (preco, "Caro") == 0 || strcmp (preco, "Barato") == 0) {
            print_lista (tempo_maximo, preco, lista);
        } else {
            printf ("Erro, por favor digite uma entrada valida (Caro ou Barato): \n");
        }
    } while (strcmp (preco, "Caro") != 0 && strcmp (preco, "Barato") != 0);
    printf("\n____________________________________________________________________\n\n3.)\n\nDigite o tempo de entrega maximo de sua preferencia em minutos: \n");
    do {
        scanf("%f", &tempo_maximo); 
        if (tempo_maximo >= 0) {
            print_lista (tempo_maximo, preco, lista);
        } else {
            printf ("Erro, por favor digite uma entrada valida em minutos: ");
        }
    } while (tempo_maximo < 0);
    return (0);
}

 
