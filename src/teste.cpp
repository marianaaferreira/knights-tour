#include "teste.h"
#include "PasseioDoCavalo.h"
#include "PasseioParberry.h"
#include "monitor.h"

#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>

using namespace std;

struct Medida {
    double tempoUs;
    long long stack;
    long long heap;
    long long ram;
    long long iters;
    bool skip;
};

template <class F>
static Medida medir(F run) {
    const long long minTempo = 100000;       // 100 ms
    const long long maxTempo = 3000000;      // 3 segundos

    long long w0 = Monitor::relogioMicros();
    long long iters = 0, sp = 0, hp = 0, rp = 0;

    while (true) {
        char base;
        Monitor::iniciarStack(&base);
        Monitor::resetHeap();

        run();

        sp = max(sp, Monitor::bytesPilha());
        hp = max(hp, Monitor::heapPico());
        rp = max(rp, Monitor::ramTrabalho());
        iters++;

        long long decorrido = Monitor::relogioMicros() - w0;

        if (decorrido >= minTempo) break;

        // evita ficar repetindo teste lento
        if (decorrido >= maxTempo) {
            Medida m{};
            m.skip = true;
            return m;
        }
    }

    long long wall = Monitor::relogioMicros() - w0;

    Medida m;
    m.skip = false;
    m.iters = iters;
    m.tempoUs = (double)wall / iters;
    m.stack = sp;
    m.heap = hp;
    m.ram = rp;
    return m;
}

static void linha(int n, const char* nome, const Medida& m) {
    if (m.skip) {
        printf("%4d  %-13s %15s  %11s  %10s  %9s\n",
               n, nome, "SKIP", "-", "-", "-");
        return;
    }

    printf("%4d  %-13s %15.1f  %11.1f  %10.1f  %9.2f\n",
           n, nome, m.tempoUs,
           m.stack / 1024.0,
           m.heap / 1024.0,
           m.ram / 1048576.0);
}

void Teste::executar() {
    vector<int> tamanhos = {8, 10, 12, 16, 20, 24, 30, 40, 50, 70, 100};

    printf("%4s  %-13s %15s  %11s  %10s  %9s\n",
           "n", "algoritmo", "microssegundos", "stack(KB)", "heap(KB)", "ram(MB)");
    printf("-----------------------------------------------------------------------\n");

    for (int n : tamanhos) {

        if (n >= 100) {
            Medida bt{};
            bt.skip = true;
            linha(n, "Backtracking", bt);
        } else {
            Medida bt = medir([&] {
                PasseioDoCavalo p(n, true);
                p.iniciar(0, 0, "", false);
            });
            linha(n, "Backtracking", bt);
        }

        if (PasseioParberry::tamanhoValido(n)) {
            Medida pb = medir([&] {
                PasseioParberry p(n);
                p.iniciar(0, 0, "", false);
            });
            linha(n, "Parberry", pb);
        } else {
            Medida pb{};
            pb.skip = true;
            linha(n, "Parberry", pb);
        }

        printf("\n");
    }
}