#include "monitor.h"

#include <cstdlib>
#include <malloc.h>
#include <new>
#include <chrono>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <psapi.h>

namespace Monitor { char* deepest = reinterpret_cast<char*>(~static_cast<size_t>(0)); }
static char* pilhaBase = nullptr;

static long long heapAtual = 0, heapMaximo = 0, heapBase = 0, heapAcumulado = 0, heapContador = 0;

void* operator new(std::size_t s) {
    if (s == 0) s = 1;
    void* p = std::malloc(s);
    if (!p) throw std::bad_alloc();
    long long real = static_cast<long long>(_msize(p));
    heapAtual += real;
    heapAcumulado += real;
    heapContador++;
    if (heapAtual > heapMaximo) heapMaximo = heapAtual;
    return p;
}

void operator delete(void* p) noexcept {
    if (!p) return;
    heapAtual -= static_cast<long long>(_msize(p));
    std::free(p);
}

void operator delete(void* p, std::size_t) noexcept { ::operator delete(p); }
void* operator new[](std::size_t s) { return ::operator new(s); }
void operator delete[](void* p) noexcept { ::operator delete(p); }
void operator delete[](void* p, std::size_t) noexcept { ::operator delete(p); }

void Monitor::iniciarStack(char* base) {
    pilhaBase = base;
    deepest = base;
}

long long Monitor::bytesPilha() {
    if (pilhaBase && deepest && deepest <= pilhaBase)
        return static_cast<long long>(pilhaBase - deepest);
    return 0;
}

void Monitor::resetHeap() {
    heapBase = heapAtual;
    heapMaximo = heapAtual;
    heapAcumulado = 0;
    heapContador = 0;
}

long long Monitor::heapPico() { return heapMaximo - heapBase; }
long long Monitor::heapTotal() { return heapAcumulado; }
long long Monitor::numAlocacoes() { return heapContador; }

long long Monitor::ramTrabalho() {
    PROCESS_MEMORY_COUNTERS m;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &m, sizeof(m)))
        return static_cast<long long>(m.WorkingSetSize);
    return 0;
}

long long Monitor::ramPico() {
    PROCESS_MEMORY_COUNTERS m;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &m, sizeof(m)))
        return static_cast<long long>(m.PeakWorkingSetSize);
    return 0;
}

long long Monitor::relogioMicros() {
    using namespace std::chrono;
    return duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
}
