#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define TAM_CELULA 40

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
            matriz[i][j] = GetRandomValue(0, 1);
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
            Color cor = matriz[i][j] == 1 ? (Color){55, 55, 55, 255} : (Color){25, 25, 25, 255};
            DrawRectangle(j * TAM_CELULA, i * TAM_CELULA, TAM_CELULA, TAM_CELULA, cor);
            DrawRectangleLines(j * TAM_CELULA, i * TAM_CELULA, TAM_CELULA, TAM_CELULA, DARKGRAY);
        }
    }
}

void inicializar_bola(Bola *b) {
    b->pos = (Vector2){GetRandomValue(50, LARGURA_JANELA - 50), GetRandomValue(50, ALTURA_JANELA - 50)};
    b->vel = (Vector2){GetRandomValue(-3, 3), GetRandomValue(-3, 3)};
    if (b->vel.x == 0) b->vel.x = 2;
    if (b->vel.y == 0) b->vel.y = 2;
    b->raio = GetRandomValue(8, 15);
    b->cor = (Color){GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255};
}

Bola *criar_bolas(int quantidade) {
    Bola *bolas = (Bola *)malloc(quantidade * sizeof(Bola));
    for (int i = 0; i < quantidade; i++) {
        Bola *b = (bolas + i);
        inicializar_bola(b);
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

int main(void) {
    InitWindow(LARGURA_JANELA, ALTURA_JANELA, "exercicio 1 - realloc");
    SetTargetFPS(60);

    int linhas = ALTURA_JANELA / TAM_CELULA;
    int colunas = LARGURA_JANELA / TAM_CELULA;
    int **matriz = criar_matriz(linhas, colunas);

    int quantidade_bolas = 12;
    Bola *bolas = criar_bolas(quantidade_bolas);

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_SPACE)) {
            quantidade_bolas++;
            bolas = (Bola *)realloc(bolas, quantidade_bolas * sizeof(Bola));
            inicializar_bola(bolas + quantidade_bolas - 1);
        }

        if (IsKeyPressed(KEY_BACKSPACE) && quantidade_bolas > 1) {
            quantidade_bolas--;
            bolas = (Bola *)realloc(bolas, quantidade_bolas * sizeof(Bola));
        }

        for (int i = 0; i < quantidade_bolas; i++) {
            atualizar_bola(bolas + i);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        desenhar_matriz(matriz, linhas, colunas);

        for (int i = 0; i < quantidade_bolas; i++) {
            DrawCircleV(bolas[i].pos, bolas[i].raio, bolas[i].cor);
        }

        DrawText(TextFormat("bolas: %d", quantidade_bolas), 10, 10, 20, WHITE);
        DrawText("espaco = adiciona bola   backspace = remove bola", 10, ALTURA_JANELA - 30, 18, LIGHTGRAY);

        EndDrawing();
    }

    free(bolas);
    liberar_matriz(matriz, linhas);
    CloseWindow();

    return 0;
}
