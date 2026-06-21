#ifndef PASSEIO_DO_CAVALO_H
#define PASSEIO_DO_CAVALO_H

#include <vector>
#include <string>
#include "Registrador.h"
using namespace std;


class PasseioDoCavalo {
private:
    int n;
    vector<vector<int>> tabuleiro;
    vector<pair<int, int>> movimentos;
    bool usarHeuristica;
    Registrador<pair<int, int>> registrador;

    bool movimentoValido(int x, int y);
    int contarMovimentos(int x, int y);
    bool resolver(int x, int y, int passo);

    void mostrarTabuleiro();  

public:
    bool visualizar = false;   
    int delayMs = 100;         

    PasseioDoCavalo(int tamanhoTabuleiro, bool heuristica);

    bool iniciar(
        int inicioX,
        int inicioY,
        const string& arquivoJson,
        bool salvar = true
    );
};

bool temSolucao(int n);

#endif
