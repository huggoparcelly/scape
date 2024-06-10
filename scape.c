#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "scape.h"
#include "mapa.h"

MAPA m;
POSICAO heroi;
int tempilula = 0;

int praondefantasmavai(int xatual, int yatual, int* xdestino, int* ydestino) {
  int opcoes[4][2] = {
    {xatual, yatual + 1},
    {xatual + 1, yatual},
    {xatual, yatual - 1},
    {xatual - 1, yatual}
  };

  srand(time(0));
  for(int i = 0; i< 10; i++) {
    int posicao = rand() % 4;

    if(podeandar(&m, FANTASMA, opcoes[posicao][0], opcoes[posicao][1])) {
      *xdestino = opcoes[posicao][0];
      *ydestino = opcoes[posicao][1];

      return 1;
    }
  }

  return 0;
}

void fantasmas() {
  MAPA copia;

  copiamapa(&copia, &m);

  for (int i = 0; i < m.linhas; i++) {
    for(int j = 0; j < m.colunas; j++) {

      if(copia.matriz[i][j] == FANTASMA) {
        
        int xdestino;
        int ydestino;

        int encontrou = praondefantasmavai(i, j, &xdestino, &ydestino);

        if(encontrou) {
          andanomapa(&m, i, j, xdestino, ydestino);
        }
      }

    }
  }

  liberamapa(&copia);
}

int acabou() {
  POSICAO pos;
  int encontrou = encontramapa(&m, &pos, HEROI);
  return !encontrou;
}

int ehvalida(MAPA* m, int x, int y) {
  if(x >= m->linhas) return 0;
  if(y >= m->colunas) return 0;
  return 1;
}

int ehvazia(MAPA* m, int x, int y) {
  return m->matriz[x][y] == VAZIO;
}

void move(char direcao) {

  if(!ehdirecao(direcao)) return;

  int proximox = heroi.x;
  int proximoy = heroi.y;

  switch (direcao) {
  case ESQUERDA:
    proximoy--;
    break;
  case CIMA:
    proximox--;
    break;
  case BAIXO:
    proximox++;
    break;
  case DIREITA:
    proximoy++;
    break;
  }

  if(!podeandar(&m, HEROI, proximox, proximoy)) return;

  if(ehpersonagem(&m, PILULA, proximox, proximoy)) {
    tempilula = 1;
  }

  andanomapa(&m, heroi.x, heroi.y, proximox, proximoy);
  heroi.x = proximox;
  heroi.y = proximoy;
}

void andanomapa(MAPA* m, int xorigem, int yorigem, int xdestino, int ydestino) {
  char personagem = m->matriz[xorigem][yorigem];
  m->matriz[xdestino][ydestino] = personagem;
  m->matriz[xorigem][yorigem] = VAZIO;
}

int ehdirecao(char direcao) {
  return direcao == ESQUERDA || direcao == CIMA ||
         direcao == BAIXO || direcao == DIREITA;
}

void explodepipula() {
  for(int i = 1; i <= 3; i++) {
    if(ehvalida(&m, heroi.x, heroi.y+i) &&
      !ehparede(&m, heroi.x, heroi.y+i)) {
        m.matriz[heroi.x][heroi.y+i] = VAZIO;
      }
  }
}

int main() {

  lermapa(&m);
  encontramapa(&m, &heroi, HEROI);

  do {
    printf("Tem pilula: %s\n", (tempilula ? "SIM" : "NAO"));
    imprimemapa(&m);  
    
    char comando;
    scanf(" %c", &comando);
    move(comando);
    if(comando == BOMBA) explodepipula();
    fantasmas();

  } while (!acabou());

  liberamapa(&m);
  printf("Você foi capturado\n");
  printf("-----GAME OVER-----\n");
  return 0;
}

