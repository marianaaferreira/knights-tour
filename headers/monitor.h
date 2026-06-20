#ifndef MONITOR_H
#define MONITOR_H

#include <cstddef>

namespace Monitor {
    extern char* deepest;

    inline void marcarPilha(const void* p) {
        const char* c = static_cast<const char*>(p);
        if (c < deepest) deepest = const_cast<char*>(c);
    }

    void iniciarStack(char* base);
    long long bytesPilha();

    void resetHeap();
    long long heapPico();
    long long heapTotal();
    long long numAlocacoes();

    long long ramTrabalho();
    long long ramPico();

    long long relogioMicros();
}

#endif
