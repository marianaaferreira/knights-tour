import matplotlib.pyplot as plt


dados = {
    "n": [8, 10, 12, 16, 20, 24, 30, 40],

    "backtracking_tempo": [146.4, 258.8, 414.8, 946.8, 1391.1, 2422.5, 4913.6, 8132.5],
    "parberry_tempo":     [68.9, 86.1, 131.6, 210.4, 317.1, 626.3, 1224.5, 1552.3],

    "backtracking_stack": [11.2, 17.4, 25.0, 44.2, 69.0, 99.2, 154.9, 275.2],
    "parberry_stack":     [43.3, 65.8, 25.9, 3.1, 3.1, 3.2, 33.5, 3.2],

    "backtracking_heap": [3.3, 6.2, 12.0, 12.7, 24.6, 47.9, 49.7, 97.9],
    "parberry_heap":     [4.5, 7.6, 13.5, 17.0, 30.6, 54.1, 64.2, 122.2],

    "backtracking_ram": [5.00, 5.11, 5.14, 5.19, 5.26, 5.29, 5.36, 5.52],
    "parberry_ram":     [5.11, 5.14, 5.18, 5.25, 5.27, 5.31, 5.37, 5.50],
}


def grafico_tempo():
    plt.figure(figsize=(10, 6))

    plt.plot(dados["n"], dados["backtracking_tempo"], marker="o", linewidth=2,
             label="Backtracking + Warnsdorff")

    plt.plot(dados["n"], dados["parberry_tempo"], marker="o", linewidth=2,
             label="Parberry")

    plt.title("Crescimento do Tempo de Execução")
    plt.xlabel("Tamanho do tabuleiro (n x n)")
    plt.ylabel("Tempo de execução (microssegundos)")
    plt.grid(True)
    plt.legend()

    plt.savefig("grafico_tempo_execucao.png", dpi=300, bbox_inches="tight")
    plt.show()


def grafico_memoria_heap():
    plt.figure(figsize=(10, 6))

    plt.plot(dados["n"], dados["backtracking_heap"], marker="o", linewidth=2,
             label="Backtracking + Warnsdorff")

    plt.plot(dados["n"], dados["parberry_heap"], marker="o", linewidth=2,
             label="Parberry")

    plt.title("Crescimento do Uso de Heap")
    plt.xlabel("Tamanho do tabuleiro (n x n)")
    plt.ylabel("Heap utilizado (KB)")
    plt.grid(True)
    plt.legend()

    plt.savefig("grafico_heap.png", dpi=300, bbox_inches="tight")
    plt.show()


def grafico_memoria_stack():
    plt.figure(figsize=(10, 6))

    plt.plot(dados["n"], dados["backtracking_stack"], marker="o", linewidth=2,
             label="Backtracking + Warnsdorff")

    plt.plot(dados["n"], dados["parberry_stack"], marker="o", linewidth=2,
             label="Parberry")

    plt.title("Crescimento do Uso de Stack")
    plt.xlabel("Tamanho do tabuleiro (n x n)")
    plt.ylabel("Stack utilizada (KB)")
    plt.grid(True)
    plt.legend()

    plt.savefig("grafico_stack.png", dpi=300, bbox_inches="tight")
    plt.show()


def grafico_memoria_ram():
    plt.figure(figsize=(10, 6))

    plt.plot(dados["n"], dados["backtracking_ram"], marker="o", linewidth=2,
             label="Backtracking + Warnsdorff")

    plt.plot(dados["n"], dados["parberry_ram"], marker="o", linewidth=2,
             label="Parberry")

    plt.title("Crescimento do Uso de RAM")
    plt.xlabel("Tamanho do tabuleiro (n x n)")
    plt.ylabel("RAM utilizada (MB)")
    plt.grid(True)
    plt.legend()

    plt.savefig("grafico_ram.png", dpi=300, bbox_inches="tight")
    plt.show()


def main():
    grafico_tempo()
    grafico_memoria_heap()
    grafico_memoria_stack()
    grafico_memoria_ram()

    print("Gráficos gerados com sucesso!")


if __name__ == "__main__":
    main()