#ifndef PASSEIO_PARBERRY_H
#define PASSEIO_PARBERRY_H

#include <vector>
#include <string>
#include <utility>
#include <map>
#include <set>
#include <random>
#include "Registrador.h"
using namespace std;

class PasseioParberry {
private:
    int n;
    int N;
    vector<vector<int>> adj;
    static map<pair<int, int>, vector<pair<int, int>>> cacheBase;

    Registrador<pair<int, int>> registrador;
    mt19937 rng;

    int id(int x, int y) const { return x * n + y; }
    int coordX(int id) const { return id / n; }
    int coordY(int id) const { return id % n; }
    bool salto(int a, int b) const;
    bool existeAresta(int a, int b) const;
    void adicionarAresta(int a, int b);
    void removerAresta(int a, int b);

    int metade(int lado) const;
    vector<pair<int, int>> tourBaseEstruturada(int largura, int altura);
    void colocarBase(int x0, int y0, int largura, int altura);
    bool unirRetangulos(int ax0, int ay0, int aw, int ah,
                        int bx0, int by0, int bw, int bh);
    bool construir(int x0, int y0, int largura, int altura);

public:
    PasseioParberry(int tamanhoTabuleiro);

    bool iniciar(int inicioX, int inicioY, const string& arquivoJson,
                 bool salvar = true);

    static bool tamanhoValido(int n) { return n % 2 == 0 && n > 6; }
};

#endif
