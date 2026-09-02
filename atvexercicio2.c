#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define TAM_CELULA 40
#define QUANTIDADE_BOLAS 12

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float raio;
    Color cor;
} Bola;

int **criar_matriz(int linhas, int colunas) {
    int **matriz = (int **)malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int *)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = 0;
        }
    }
    return matriz;
}

void liberar_matriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void desenhar_matriz(int **matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            Color cor = matriz[i][j] == 1 ? (Color){200, 180, 40, 255} : (Color){25, 25, 25, 255};
            DrawRectangle(j * TAM_CELULA, i * TAM_CELULA, TAM_CELULA, TAM_CELULA, cor);
            DrawRectangleLines(j * TAM_CELULA, i * TAM_CELULA, TAM_CELULA, TAM_CELULA, DARKGRAY);
        }
    }
}

Bola *criar_bolas(int quantidade) {
    Bola *bolas = (Bola *)malloc(quantidade * sizeof(Bola));
    for (int i = 0; i < quantidade; i++) {
        Bola *b = (bolas + i);
        b->pos = (Vector2){GetRandomValue(50, LARGURA_JANELA - 50), GetRandomValue(50, ALTURA_JANELA - 50)};
        b->vel = (Vector2){GetRandomValue(-3, 3), GetRandomValue(-3, 3)};
        if (b->vel.x == 0) b->vel.x = 2;
        if (b->vel.y == 0) b->vel.y = 2;
        b->raio = GetRandomValue(8, 15);
        b->cor = (Color){GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255};
    }
    return bolas;
}

void atualizar_bola(Bola *b) {
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;

    if (b->pos.x - b->raio < 0 || b->pos.x + b->raio > LARGURA_JANELA)
        b->vel.x *= -1;
    if (b->pos.y - b->raio < 0 || b->pos.y + b->raio > ALTURA_JANELA)
        b->vel.y *= -1;
}

void marcar_celula_visitada(int **grade, int linhas, int colunas, Bola *b, int *contador) {
    int coluna = (int)(b->pos.x / TAM_CELULA);
    int linha = (int)(b->pos.y / TAM_CELULA);

    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
        if (grade[linha][coluna] == 0) {
            grade[linha][coluna] = 1;
            *contador = *contador + 1;
        }
    }
}

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "exercicio 2 - mapa de calor");
    SetTargetFPS(60);

    int linhas = ALTURA_JANELA / TAM_CELULA;
    int colunas = LARGURA_JANELA / TAM_CELULA;
    int **grade = criar_matriz(linhas, colunas);

    Bola *bolas = criar_bolas(QUANTIDADE_BOLAS);

    int celulas_visitadas = 0;

    while (!WindowShouldClose()) {

        for (int i = 0; i < QUANTIDADE_BOLAS; i++) {
            atualizar_bola(bolas + i);
            marcar_celula_visitada(grade, linhas, colunas, bolas + i, &celulas_visitadas);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        desenhar_matriz(grade, linhas, colunas);

        for (int i = 0; i < QUANTIDADE_BOLAS; i++) {
            DrawCircleV(bolas[i].pos, bolas[i].raio, bolas[i].cor);
        }

        DrawText(TextFormat("celulas visitadas: %d / %d", celulas_visitadas, linhas * colunas), 10, 10, 20, WHITE);

        EndDrawing();
    }

    free(bolas);
    liberar_matriz(grade, linhas);
    CloseWindow();

    return 0;
}
