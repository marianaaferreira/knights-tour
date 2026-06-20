#ifndef REGISTRADOR_H
#define REGISTRADOR_H

#include <vector>
#include <string>
#include <utility>
#include <fstream>
using namespace std;

template <typename T>
class Registrador {
private:
    struct Passo {
        long long iteracao;
        T posicao;
        int contador;
    };

    string algoritmo;
    int tamanho;
    vector<Passo> passos;
    long long iteracaoAtual = 0;

    static string posicaoParaJson(const T& posicao);

public:
    Registrador(const string& nomeAlgoritmo, int tamanhoTabuleiro)
        : algoritmo(nomeAlgoritmo), tamanho(tamanhoTabuleiro) {}

    void registrar(const T& posicao, int contador) {
        passos.push_back({iteracaoAtual++, posicao, contador});
    }

    void salvar(const string& caminhoArquivo) const {
        ofstream out(caminhoArquivo);

        out << "{\n";
        out << "  \"algorithm\": \"" << algoritmo << "\",\n";
        out << "  \"boardSize\": " << tamanho << ",\n";
        out << "  \"iterations\": [\n";

        for (size_t i = 0; i < passos.size(); i++) {
            const Passo& p = passos[i];
            out << "    {\"iteration\": " << p.iteracao
                << ", \"position\": " << posicaoParaJson(p.posicao)
                << ", \"counter\": " << p.contador << "}";
            if (i + 1 < passos.size())
                out << ",";
            out << "\n";
        }

        out << "  ]\n";
        out << "}\n";
    }
};

template <>
inline string Registrador<pair<int, int>>::posicaoParaJson(const pair<int, int>& p) {
    return "{\"x\": " + to_string(p.first) + ", \"y\": " + to_string(p.second) + "}";
}

#endif
