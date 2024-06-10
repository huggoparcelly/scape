#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapa.h"

void copiamapa(MAPA* destino, MAPA* origem) {
  destino->linhas = origem->linhas;
  destino->colunas = origem->colunas;

  alocamapa(destino);
  for(int i=0; i< origem->linhas; i++) {
    strcpy(destino->matriz[i], origem->matriz[i]);
  }
}

void liberamapa(MAPA* m) {
  for (int i = 0; i < m->linhas; i++) {
    free(m->matriz[i]);
  }
  free(m->matriz);
}

void lermapa(MAPA* m) {
  FILE *f;
  f = fopen("mapa.txt", "r");
  if (f == 0)
  {
    printf("Banco de dados não disoponível");
    exit(1);
  }

  fscanf(f, "%d %d", &m->linhas, &m->colunas);

  alocamapa(m);

  for (int i = 0; i < 5; i++)
  {
    fscanf(f, "%s", m->matriz[i]);
  }
  fclose(f);
}

void alocamapa(MAPA* m){
m->matriz = malloc(sizeof(char *) * m->linhas);
  for (int i = 0; i < m->linhas; i++)
  {
    m->matriz[i] = malloc(sizeof(char) * (m->colunas + 1));
  }
}

void imprimemapa(MAPA* m) {
  for (int i = 0; i < 5; i++) {
    printf("%s\n", m->matriz[i]);
  }
}

int encontramapa(MAPA* m, POSICAO* p, char c) {
  for (int i = 0; i < m->linhas; i++) {
    for(int j = 0; j < m->colunas; j++) {
      if(m->matriz[i][j] == c) {
        p->x = i;
        p->y = j;
        return 1;
      }
    }
  }
  return 0;
}

int podeandar(MAPA* m, char personagem, int x, int y) {
  return ehvalida(m, x, y) && ehvalida(m, x, y) &&
        !ehparede(m, x, y) &&
       !ehpersonagem(m, personagem, x, y);
}

int ehparede(MAPA* m, int x, int y) {
  return m->matriz[x][y] == PAREDE_VERTICAL ||
        m->matriz[x][y] == PAREDE_HORIZONTAL;
}

int ehpersonagem(MAPA* m, char personagem, int x, int y) {
  return m->matriz[x][y] == personagem;
}