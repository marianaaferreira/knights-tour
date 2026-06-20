#include <iostream>
#include <string>
#include "PasseioDoCavalo.h"
#include "PasseioParberry.h"
#include "teste.h"

using namespace std;

int main() {
    int n, x, y;

    cout << "Algoritmo:\n";
    cout << "  1 - Backtracking (com heuristica de Warnsdorff)\n";
    cout << "  2 - Parberry (divisao e conquista, passeio fechado)\n";
    cout << "  3 - Rodar testes de desempenho\n";
    cout << "Escolha: ";
    int algoritmo;
    cin >> algoritmo;

    if (algoritmo == 3) {
        Teste::executar();
        return 0;
    }

    cout << "Tamanho do tabuleiro: ";
    cin >> n;

    if (!temSolucao(n)) {
        cout << "Sem solucao.\n";
        return 0;
    }

    if (algoritmo == 2 && !PasseioParberry::tamanhoValido(n)) {
        cout << "\nParberry exige um tabuleiro de lado par e maior que 6. "
             << "Nao e possivel para esse tamanho.\n";
        return 0;
    }

    cout << "X inicial: ";
    cin >> x;

    cout << "Y inicial: ";
    cin >> y;

    if (algoritmo == 2) {
        PasseioParberry parberry(n);

        if (parberry.iniciar(x, y, "passeio_parberry.json"))
            cout << "\nPasseio fechado gerado!\n";
        else
            cout << "\nFalha ao gerar o passeio.\n";

        cout << "Registro salvo em passeio_parberry.json\n";
    } else {
        PasseioDoCavalo passeio(n, true);

        if (passeio.iniciar(x, y, "passeio_backtracking.json"))
            cout << "\nSolucao encontrada!\n";
        else
            cout << "\nNao foi possivel encontrar solucao.\n";

        cout << "Registro salvo em passeio_backtracking.json\n";
    }

    return 0;
}
