#include "PasseioParberry.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdlib>

using namespace std;

// Os 8 saltos do cavalo.
static const int DX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
static const int DY[8] = {1, 2, 2, 1, -1, -2, -2, -1};

PasseioParberry::PasseioParberry(int tamanho_tabuleiro)
    : registrador("Parberry (Divide and Conquer)", tamanho_tabuleiro) {
    n = tamanho_tabuleiro;
    N = n * n;
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

// Decompoe um lado par em pedacos de {6,8,10}. Como o lado e par e > 6:
//   lado % 6 == 0 -> so 6s;  == 2 -> um 8 + 6s;  == 4 -> um 10 + 6s.
vector<int> PasseioParberry::decompor(int tamanho) const {
    vector<int> lados;
    int resto = tamanho % 6;
    if (resto == 2) {
        lados.push_back(8);
        tamanho -= 8;
    } else if (resto == 4) {
        if (tamanho == 10) {
            lados.push_back(10);    // unico caso que precisa de bloco 10
            tamanho -= 10;
        } else {
            lados.push_back(8);     // dois 8s evitam o bloco 10 (mais lento)
            lados.push_back(8);
            tamanho -= 16;
        }
    }
    while (tamanho > 0) {
        lados.push_back(6);
        tamanho -= 6;
    }
    return lados;
}

// Acha um passeio fechado num bloco largura x altura, por backtracking com
// duas restricoes que o deixam rapido ate em 6x6 (caso patologico):
//   - ordem de Warnsdorff (tenta a casa de menor grau primeiro);
//   - poda de beco sem saida (nao isola nenhuma casa nao visitada);
//   - fechamento embutido: como (0,0) num bloco >=6 tem exatamente os vizinhos
//     A=(1,2) e B=(2,1), e toda tour fechada por (0,0) usa as duas arestas,
//     fixamos o 1o passo em A e exigimos terminar em B. Sem isso, o
//     backtracking explodia procurando o fechamento.
// O resultado e memorizado por (largura, altura).
vector<pair<int, int>> PasseioParberry::tourBase(int largura, int altura) {
    auto chave = make_pair(largura, altura);
    auto it = cacheBase.find(chave);
    if (it != cacheBase.end())
        return it->second;

    int w = largura, h = altura;
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

    // Vizinhos do canto (0,0): A (primeiro passo) e B (passo final).
    const int Ax = 1, Ay = 2, Bx = 2, By = 1;

    function<bool(int, int, int)> rec = [&](int x, int y, int cnt) -> bool {
        if (cnt == w * h)
            return x == Bx && y == By; // cobriu tudo terminando em B

        // candidatos ordenados por Warnsdorff (menor grau antes)
        vector<pair<int, int>> cand; // (grau, indice do salto)
        for (int i = 0; i < 8; i++) {
            int nx = x + DX[i], ny = y + DY[i];
            if (dentro(nx, ny) && !visitado[nx][ny])
                cand.push_back({grau(nx, ny), i});
        }
        sort(cand.begin(), cand.end());

        int restantesApos = w * h - (cnt + 1);
        for (auto& pr : cand) {
            int nx = x + DX[pr.second], ny = y + DY[pr.second];

            // B fica reservado para o ultimo passo (fechamento)
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
        }
        return false;
    };

    visitado[0][0] = true;   seq.push_back({0, 0});
    visitado[Ax][Ay] = true; seq.push_back({Ax, Ay});
    rec(Ax, Ay, 2);

    cacheBase[chave] = seq;
    return seq;
}

// Une o ciclo do bloco A ao ciclo do bloco B por uma troca de arestas: acha uma
// casa u em A e uma casa v em B que sejam um salto de cavalo, cujas vizinhas no
// passeio (un, vn) tambem sejam um salto entre si; remove (u,un) e (v,vn) e
// adiciona (u,v) e (un,vn). Isso funde os dois ciclos em um so.
bool PasseioParberry::unirBlocos(int linhaA, int colA, int linhaB, int colB,
                                 const vector<int>& deslocamento,
                                 const vector<int>& lados) {
    int Ax0 = deslocamento[colA], Ax1 = deslocamento[colA] + lados[colA];
    int Ay0 = deslocamento[linhaA], Ay1 = deslocamento[linhaA] + lados[linhaA];
    int Bx0 = deslocamento[colB], Bx1 = deslocamento[colB] + lados[colB];
    int By0 = deslocamento[linhaB], By1 = deslocamento[linhaB] + lados[linhaB];

    for (int x = Ax0; x < Ax1; x++) {
        for (int y = Ay0; y < Ay1; y++) {
            int u = id(x, y);
            for (int i = 0; i < 8; i++) {
                int vx = x + DX[i], vy = y + DY[i];
                if (vx < Bx0 || vx >= Bx1 || vy < By0 || vy >= By1) continue;
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

bool PasseioParberry::iniciar(int inicioX, int inicioY, const string& arquivoJson,
                              bool salvar) {
    vector<int> lados = decompor(n);
    int B = (int)lados.size();

    vector<int> deslocamento(B + 1, 0);
    for (int i = 0; i < B; i++)
        deslocamento[i + 1] = deslocamento[i] + lados[i];

    adj.assign(N, {});

    // 1) coloca um ciclo fechado em cada bloco
    for (int bi = 0; bi < B; bi++) {
        for (int bj = 0; bj < B; bj++) {
            int h = lados[bi], w = lados[bj];
            int oy = deslocamento[bi], ox = deslocamento[bj];

            vector<pair<int, int>> t = tourBase(w, h);
            int k = (int)t.size();
            for (int p = 0; p < k; p++) {
                auto a = t[p];
                auto b = t[(p + 1) % k];
                adicionarAresta(id(ox + a.first, oy + a.second),
                                id(ox + b.first, oy + b.second));
            }
        }
    }

    // 2) une os blocos: cada linha vira um ciclo, depois unem-se as linhas
    bool ok = true;
    for (int bi = 0; bi < B; bi++)
        for (int bj = 0; bj + 1 < B; bj++)
            ok &= unirBlocos(bi, bj, bi, bj + 1, deslocamento, lados);
    for (int bi = 0; bi + 1 < B; bi++)
        ok &= unirBlocos(bi, 0, bi + 1, 0, deslocamento, lados);

    if (!ok)
        cerr << "Aviso: falha ao unir algum bloco.\n";

    // 3) percorre o ciclo a partir da casa inicial e registra o passeio
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
    // passeio fechado: depois de N passos deve voltar a casa inicial
    if (cur != start) valido = false;

    if (salvar)
        registrador.salvar(arquivoJson);
    return ok && valido;
}
