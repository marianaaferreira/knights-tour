#ifndef PASSEIO_PARBERRY_H
#define PASSEIO_PARBERRY_H

#include <vector>
#include <string>
#include <utility>
#include <map>
#include "Registrador.h"
using namespace std;

/* 
Passeio do Cavalo por divisao e conquista (Parberry, 1997).
o tabuleiro e quebrado em blocos pequenos (6, 8 ou 10 de lado), em
cada bloco constroi-se um passeio fechado (um ciclo), e os ciclos vizinhos
sao unidos dois a dois por uma troca de arestas (remove duas arestas, uma de
cada ciclo, e adiciona duas arestas cruzadas) ate sobrar um unico ciclo que
cobre todo o tabuleiro(fechante).
Funciona para tabuleiros de lado par. Como cada lado e decomposto em pedacos
de {6,8,10}, exige-se um lado par maior que 6 (a verificacao fica no menu).
 O resultado e gravado em JSON pelo Registrador, comecando na casa pedida.
*/
 class PasseioParberry {
private:
    int n;                              // lado do tabuleiro
    int N;                             // n * n
    vector<vector<int>> adj;           // adjacencia do passeio (2 vizinhos por casa)
    map<pair<int, int>, vector<pair<int, int>>> cacheBase; // passeios-base por (largura, altura)

    Registrador<pair<int, int>> registrador;

    int id(int x, int y) const { return x * n + y; }
    int coordX(int id) const { return id / n; }
    int coordY(int id) const { return id % n; }
    bool salto(int a, int b) const;
    bool existeAresta(int a, int b) const;
    void adicionarAresta(int a, int b);
    void removerAresta(int a, int b);

    vector<int> decompor(int tamanho) const;       // lados dos blocos que somam 'tamanho'
    vector<pair<int, int>> tourBase(int largura, int altura); // ciclo fechado local
    bool unirBlocos(int linhaA, int colA, int linhaB, int colB,
                    const vector<int>& deslocamento, const vector<int>& lados);

public:
    PasseioParberry(int tamanho_tabuleiro);

    // Constroi o passeio fechado, grava o registro em arquivoJson (comecando em
    // (inicioX, inicioY)) e retorna true se o resultado e um passeio fechado
    // valido cobrindo todas as casas. Com salvar = false, nao grava o JSON
    bool iniciar(int inicioX, int inicioY, const string& arquivoJson,
                 bool salvar = true);

    // tamanho minimo.
    static bool tamanhoValido(int n) { return n % 2 == 0 && n > 6; }
};

#endif
