import json
import os
import time

ARQUIVO = "passeio_parberry.json"
DELAY = 0.15

with open(ARQUIVO, "r", encoding="utf-8") as f:
    dados = json.load(f)

n = dados["boardSize"]
algoritmo = dados["algorithm"]
tabuleiro = [["." for _ in range(n)] for _ in range(n)]

for item in dados["iterations"]:
    x = item["position"]["x"]
    y = item["position"]["y"]
    counter = item["counter"]

    tabuleiro[x][y] = str(counter)

    os.system("cls")

    print(f"{algoritmo} - visualização pelo JSON")
    print(f"Iteração: {item['iteration']}")
    print(f"Posição atual: ({x}, {y})")
    print()

    for linha in tabuleiro:
        print("\t".join(linha))

    time.sleep(DELAY)

print("\nVisualização finalizada!")