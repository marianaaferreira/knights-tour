#include "PasseioDoCavalo.h"
#include "monitor.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

PasseioDoCavalo::PasseioDoCavalo(int tamanhoTabuleiro, bool heuristica)
    : n(tamanhoTabuleiro),
      usarHeuristica(heuristica),
      registrador(heuristica ? "Backtracking (Warnsdorff)" : "Backtracking",
                  tamanhoTabuleiro) {

    tabuleiro = vector<vector<int>>(n, vector<int>(n, -1));

    movimentos = {
        {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
        {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
    };
}

bool PasseioDoCavalo::movimentoValido(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < n && tabuleiro[x][y] == -1;
}

int PasseioDoCavalo::contarMovimentos(int x, int y) {
    int contador = 0;

    for (int i = 0; i < 8; i++) {
        int novoX = x + movimentos[i].first;
        int novoY = y + movimentos[i].second;

        if (movimentoValido(novoX, novoY))
            contador++;
    }
    return contador;
}

bool PasseioDoCavalo::resolver(int x, int y, int passo) {
    Monitor::marcarPilha(&x);
    if (passo == n * n)
        return true;

    vector<int> ordem;

    for (int i = 0; i < 8; i++) {
        int novoX = x + movimentos[i].first;
        int novoY = y + movimentos[i].second;

        if (movimentoValido(novoX, novoY))
            ordem.push_back(i);
    }

    if (usarHeuristica) {
        for (int i = 0; i < ordem.size(); i++) {
            for (int j = i + 1; j < ordem.size(); j++) {

                int ax = x + movimentos[ordem[i]].first;
                int ay = y + movimentos[ordem[i]].second;

                int bx = x + movimentos[ordem[j]].first;
                int by = y + movimentos[ordem[j]].second;

                if (contarMovimentos(ax, ay) > contarMovimentos(bx, by)) {
                    int temp = ordem[i];
                    ordem[i] = ordem[j];
                    ordem[j] = temp;
                }
            }
        }
    }

    for (int i = 0; i < ordem.size(); i++) {
        int indiceMovimento = ordem[i];

        int novoX = x + movimentos[indiceMovimento].first;
        int novoY = y + movimentos[indiceMovimento].second;

        tabuleiro[novoX][novoY] = passo;
        registrador.registrar({novoX, novoY}, passo);

        if (visualizar)
            mostrarTabuleiro();

        if (resolver(novoX, novoY, passo + 1))
            return true;

        tabuleiro[novoX][novoY] = -1;
        registrador.registrar({novoX, novoY}, -1);

        if (visualizar)
            mostrarTabuleiro();
    }

    return false;
}

bool PasseioDoCavalo::iniciar(int x, int y, const string& arquivoJson, bool salvar) {
    tabuleiro[x][y] = 0;
    registrador.registrar({x, y}, 0);

    bool encontrou = resolver(x, y, 1);

    if (salvar)
        registrador.salvar(arquivoJson);
    return encontrou;
}

bool temSolucao(int n) {
    if (n == 1) return true;
    if (n == 2 || n == 3) return false;
    return true;
}

void PasseioDoCavalo::mostrarTabuleiro() {
    system("cls");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            if (tabuleiro[i][j] == -1)
                cout << ".\t";
            else
                cout << tabuleiro[i][j] << "\t";
        }
        cout << endl;
    }

    this_thread::sleep_for(
        chrono::milliseconds(delayMs)
    );
}