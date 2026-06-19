#include "teste.h"
#include "PasseioDoCavalo.h"
#include "PasseioParberry.h"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// devolve o tempo medio (em micro segundos).

static double mediaBacktracking(int n, int reps) {
    long long soma = 0;
    for (int r = 0; r < reps; r++) {
        PasseioDoCavalo p(n, true);
        auto t0 = high_resolution_clock::now();
        p.iniciar(0, 0, "", false);
        auto t1 = high_resolution_clock::now();

        soma += duration_cast<microseconds>(t1 - t0).count();
    }
    return (double)soma / reps;
}

static double mediaParberry(int n, int reps) {
    long long soma = 0;
    for (int r = 0; r < reps; r++) {
        PasseioParberry p(n);
        auto t0 = high_resolution_clock::now();
        p.iniciar(0, 0, "", false);
        auto t1 = high_resolution_clock::now();

        soma += duration_cast<microseconds>(t1 - t0).count();
    }
    return (double)soma / reps;
}

void Teste::executar() {
    vector<int> tamanhos = {8, 10, 12, 16, 20, 24, 30, 40};
    const int reps = 10;

    cout << "\n=== Teste de desempenho ===\n";
    cout << "(tempo medio em microssegundos, " << reps
         << " execucoes; nao inclui gravacao do JSON)\n\n";

    cout << " Tabuleiro |  Backtracking+Warnsdorff |   Parberry\n";
    cout << "-----------+--------------------------+--------------\n";

    cout << fixed << setprecision(1);
    for (int n : tamanhos) {
        string lado = to_string(n) + "x" + to_string(n);

        double bt = mediaBacktracking(n, reps);

        cout << "  " << left << setw(8) << lado << right
             << " | " << setw(18) << bt << " us | ";

        if (PasseioParberry::tamanhoValido(n)) {
            double pb = mediaParberry(n, reps);
            cout << setw(9) << pb << " us\n";
        } else {
            cout << setw(9) << "N/A" << "\n";
        }
    }

    cout << "\nObs.: Backtracking faz passeio aberto (qualquer tamanho); "
         << "Parberry faz passeio fechado (so lado par > 6).\n\n";
}
