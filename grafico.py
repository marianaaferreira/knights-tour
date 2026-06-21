import matplotlib.pyplot as plt


dados = {
    "n": [8, 10, 12, 16, 20, 24, 30, 40, 50, 70, 100],

    "backtracking_tempo": [192.2, 294.8, 511.8, 749.2, 1283.4, 1964.7, 3665.2, 6924.5, 20327.4, 21151.0, None],
    "parberry_tempo":     [73.9, 89.1, 124.4, 189.4, 293.7, 412.9, 701.7, 1093.8, 2472.0, 4023.4, 7625.8],

    "backtracking_stack": [11.2, 17.4, 25.0, 44.2, 69.0, 99.2, 154.9, 275.2, 429.9, 842.4, None],
    "parberry_stack":     [43.3, 65.8, 25.9, 3.1, 3.1, 3.2, 33.5, 3.2, 3.3, 53.6, 3.5],

    "backtracking_heap": [3.3, 6.2, 12.0, 12.7, 24.6, 47.9, 49.7, 97.9, 192.1, 381.7, None],
    "parberry_heap":     [4.5, 7.6, 13.5, 17.0, 30.6, 54.1, 64.2, 122.2, 222.4, 441.7, 889.7],

    "backtracking_ram": [5.01, 5.12, 5.15, 5.19, 5.27, 5.33, 5.37, 5.51, 5.68, 6.25, None],
    "parberry_ram":     [5.10, 5.14, 5.19, 5.25, 5.28, 5.30, 5.37, 5.52, 5.70, 6.54, 6.42],
}


def plotar_grafico(chave_backtracking, chave_parberry, titulo, eixo_y, nome_arquivo):
    plt.figure(figsize=(10, 6))

    plt.plot(
        dados["n"],
        dados[chave_backtracking],
        marker="o",
        linewidth=2,
        label="Backtracking + Warnsdorff"
    )

    plt.plot(
        dados["n"],
        dados[chave_parberry],
        marker="o",
        linewidth=2,
        label="Parberry"
    )

    plt.title(titulo)
    plt.xlabel("Tamanho do tabuleiro (n x n)")
    plt.ylabel(eixo_y)
    plt.grid(True)
    plt.legend()

    plt.savefig(nome_arquivo, dpi=300, bbox_inches="tight")
    plt.show()


def main():
    plotar_grafico(
        "backtracking_tempo",
        "parberry_tempo",
        "Crescimento do Tempo de Execução",
        "Tempo de execução (microssegundos)",
        "grafico_tempo_execucao.png"
    )

    plotar_grafico(
        "backtracking_stack",
        "parberry_stack",
        "Crescimento do Uso de Stack",
        "Stack utilizada (KB)",
        "grafico_stack.png"
    )

    plotar_grafico(
        "backtracking_heap",
        "parberry_heap",
        "Crescimento do Uso de Heap",
        "Heap utilizado (KB)",
        "grafico_heap.png"
    )

    plotar_grafico(
        "backtracking_ram",
        "parberry_ram",
        "Crescimento do Uso de RAM",
        "RAM utilizada (MB)",
        "grafico_ram.png"
    )

    print("Gráficos gerados com sucesso!")


if __name__ == "__main__":
    main()