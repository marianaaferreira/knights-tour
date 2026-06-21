#include "PasseioParberry.h"
#include "monitor.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>

using namespace std;

static const int DX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
static const int DY[8] = {1, 2, 2, 1, -1, -2, -2, -1};

map<pair<int, int>, vector<pair<int, int>>> PasseioParberry::cacheBase;

PasseioParberry::PasseioParberry(int tamanhoTabuleiro)
    : registrador("Parberry ", tamanhoTabuleiro) {
    n = tamanhoTabuleiro;
    N = n * n;
    rng = mt19937(random_device{}());
}

bool PasseioParberry::salto(int a, int b) const {
    int dx = abs(coordX(a) - coordX(b));
    int dy = abs(coordY(a) - coordY(b));
    return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
}

bool PasseioParberry::existeAresta(int a, int b) const {
    for (int v : adj[a])
        if (v == b) return true;
    return false;
}

void PasseioParberry::adicionarAresta(int a, int b) {
    adj[a].push_back(b);
    adj[b].push_back(a);
}

void PasseioParberry::removerAresta(int a, int b) {
    adj[a].erase(find(adj[a].begin(), adj[a].end(), b));
    adj[b].erase(find(adj[b].begin(), adj[b].end(), a));
}

int PasseioParberry::metade(int lado) const {
    int m = lado / 2;
    if (m % 2 != 0) m--;
    return m;
}

vector<pair<int, int>> PasseioParberry::tourBaseEstruturada(int largura, int altura) {
    auto chave = make_pair(largura, altura);
    auto it = cacheBase.find(chave);
    if (it != cacheBase.end())
        return it->second;

    int w = largura, h = altura;

    auto local = [&](int x, int y) { return x * h + y; };
    auto aresta = [&](int x1, int y1, int x2, int y2) {
        int a = local(x1, y1), b = local(x2, y2);
        return make_pair(min(a, b), max(a, b));
    };
    set<pair<int, int>> exigidas = {
        aresta(0, 2, 1, 0), aresta(0, 1, 2, 0),
        aresta(0, h - 3, 1, h - 1), aresta(0, h - 2, 2, h - 1),
        aresta(w - 1, 2, w - 2, 0), aresta(w - 1, 1, w - 3, 0),
        aresta(w - 1, h - 3, w - 2, h - 1), aresta(w - 1, h - 2, w - 3, h - 1)
    };

    vector<vector<bool>> visitado(w, vector<bool>(h, false));
    vector<pair<int, int>> seq;

    auto dentro = [&](int x, int y) {
        return x >= 0 && x < w && y >= 0 && y < h;
    };
    auto grau = [&](int x, int y) {
        int c = 0;
        for (int i = 0; i < 8; i++) {
            int nx = x + DX[i], ny = y + DY[i];
            if (dentro(nx, ny) && !visitado[nx][ny]) c++;
        }
        return c;
    };

    const int Ax = 1, Ay = 2, Bx = 2, By = 1;

    long long nos = 0;
    const long long orcamento = 200000;
    bool estouro = false;

    function<bool(int, int, int)> rec = [&](int x, int y, int cnt) -> bool {
        Monitor::marcarPilha(&x);
        if (++nos > orcamento) { estouro = true; return false; }
        if (cnt == w * h)
            return x == Bx && y == By;

        vector<pair<int, int>> cand;
        for (int i = 0; i < 8; i++) {
            int nx = x + DX[i], ny = y + DY[i];
            if (dentro(nx, ny) && !visitado[nx][ny])
                cand.push_back({grau(nx, ny), i});
        }
        shuffle(cand.begin(), cand.end(), rng);
        stable_sort(cand.begin(), cand.end(),
                    [](const pair<int, int>& a, const pair<int, int>& b) {
                        return a.first < b.first;
                    });

        int restantesApos = w * h - (cnt + 1);
        for (auto& pr : cand) {
            int nx = x + DX[pr.second], ny = y + DY[pr.second];
            if (nx == Bx && ny == By && cnt + 1 != w * h) continue;

            bool beco = false;
            if (restantesApos >= 2) {
                for (int j = 0; j < 8 && !beco; j++) {
                    int wx = nx + DX[j], wy = ny + DY[j];
                    if (dentro(wx, wy) && !visitado[wx][wy] && grau(wx, wy) - 1 == 0)
                        beco = true;
                }
            }
            if (beco) continue;

            visitado[nx][ny] = true;
            seq.push_back({nx, ny});
            if (rec(nx, ny, cnt + 1)) return true;
            visitado[nx][ny] = false;
            seq.pop_back();
            if (estouro) return false;
        }
        return false;
    };

    while (true) {
        for (auto& col : visitado) fill(col.begin(), col.end(), false);
        seq.clear();
        nos = 0;
        estouro = false;
        visitado[0][0] = true;   seq.push_back({0, 0});
        visitado[Ax][Ay] = true; seq.push_back({Ax, Ay});

        if (rec(Ax, Ay, 2) && (int)seq.size() == w * h) {
            set<pair<int, int>> arestas;
            int k = (int)seq.size();
            for (int p = 0; p < k; p++) {
                int a = local(seq[p].first, seq[p].second);
                int b = local(seq[(p + 1) % k].first, seq[(p + 1) % k].second);
                arestas.insert({min(a, b), max(a, b)});
            }
            bool ok = true;
            for (auto& e : exigidas)
                if (!arestas.count(e)) { ok = false; break; }
            if (ok) break;
        }
    }

    cacheBase[chave] = seq;
    return seq;
}

void PasseioParberry::colocarBase(int x0, int y0, int largura, int altura) {
    vector<pair<int, int>> seq = tourBaseEstruturada(largura, altura);
    int k = (int)seq.size();
    for (int p = 0; p < k; p++) {
        auto a = seq[p];
        auto b = seq[(p + 1) % k];
        adicionarAresta(id(x0 + a.first, y0 + a.second),
                        id(x0 + b.first, y0 + b.second));
    }
}

bool PasseioParberry::unirRetangulos(int ax0, int ay0, int aw, int ah,
                                     int bx0, int by0, int bw, int bh) {
    for (int x = ax0; x < ax0 + aw; x++) {
        for (int y = ay0; y < ay0 + ah; y++) {
            int u = id(x, y);
            for (int i = 0; i < 8; i++) {
                int vx = x + DX[i], vy = y + DY[i];
                if (vx < bx0 || vx >= bx0 + bw || vy < by0 || vy >= by0 + bh) continue;
                int v = id(vx, vy);

                for (int un : adj[u]) {
                    for (int vn : adj[v]) {
                        if (un == v || vn == u || un == vn) continue;
                        if (salto(un, vn) && !existeAresta(u, v) && !existeAresta(un, vn)) {
                            removerAresta(u, un);
                            removerAresta(v, vn);
                            adicionarAresta(u, v);
                            adicionarAresta(un, vn);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool PasseioParberry::construir(int x0, int y0, int largura, int altura) {
    Monitor::marcarPilha(&x0);
    if (min(largura, altura) <= 10) {
        colocarBase(x0, y0, largura, altura);
        return true;
    }

    int w1 = metade(largura), w2 = largura - w1;
    int h1 = metade(altura), h2 = altura - h1;

    bool ok = true;
    ok &= construir(x0, y0, w1, h1);
    ok &= construir(x0 + w1, y0, w2, h1);
    ok &= construir(x0, y0 + h1, w1, h2);
    ok &= construir(x0 + w1, y0 + h1, w2, h2);

    ok &= unirRetangulos(x0, y0, w1, h1, x0 + w1, y0, w2, h1);
    ok &= unirRetangulos(x0, y0 + h1, w1, h2, x0 + w1, y0 + h1, w2, h2);
    ok &= unirRetangulos(x0, y0, w1, h1, x0, y0 + h1, w1, h2);

    return ok;
}

bool PasseioParberry::iniciar(int inicioX, int inicioY, const string& arquivoJson,
                              bool salvar) {
    adj.assign(N, {});

    bool ok = construir(0, 0, n, n);

    if (inicioX < 0 || inicioX >= n || inicioY < 0 || inicioY >= n) {
        inicioX = 0;
        inicioY = 0;
    }
    int start = id(inicioX, inicioY);

    vector<bool> visitado(N, false);
    int prev = -1, cur = start;
    bool valido = true;

    for (int i = 0; i < N; i++) {
        if (adj[cur].size() != 2 || visitado[cur]) { valido = false; break; }
        visitado[cur] = true;
        registrador.registrar({coordX(cur), coordY(cur)}, i);

        int nxt = (adj[cur][0] != prev) ? adj[cur][0] : adj[cur][1];
        if (!salto(cur, nxt)) valido = false;
        prev = cur;
        cur = nxt;
    }
    if (cur != start) valido = false;

    if (salvar)
        registrador.salvar(arquivoJson);
    return ok && valido;
}
