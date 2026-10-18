"""Графики T(n) по results.csv, который записывает Alg5.exe.

Запуск:  python plot.py      (нужен matplotlib: pip install matplotlib)
"""
import csv

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ALGS = ["insertionSort", "mergeSort", "std::sort"]
COLORS = {"insertionSort": "#d62728", "mergeSort": "#1f77b4", "std::sort": "#2ca02c"}
TITLES = {"random": "Случайные массивы", "fragments": "Массивы из 10 упорядоченных фрагментов"}

# data -> алгоритм -> список (n, время)
table = {}
with open("results.csv", encoding="utf-8") as f:
    for row in csv.DictReader(f, delimiter=";"):
        for alg in ALGS:
            if row[alg]:
                table.setdefault(row["data"], {}).setdefault(alg, []).append(
                    (int(row["n"]), float(row[alg])))


def draw(ax, algs, max_n):
    for alg in ALGS:
        points = [(n, t) for n, t in algs.get(alg, []) if n <= max_n]
        if points:
            ax.plot([p[0] for p in points], [p[1] for p in points],
                    marker="o", color=COLORS[alg], label=alg)
    ax.set_xlabel("n — число элементов")
    ax.set_ylabel("T(n), мс")
    ax.grid(alpha=0.3)
    ax.legend()


for data, algs in table.items():
    fig, (left, right) = plt.subplots(1, 2, figsize=(13, 5))
    draw(left, algs, 50000)
    left.set_title("Все алгоритмы, n до 50 000")
    draw(right, {a: algs[a] for a in ["mergeSort", "std::sort"]}, 10 ** 9)
    right.set_title("O(n log n) на больших n")
    fig.suptitle(TITLES.get(data, data))
    fig.tight_layout()
    fig.savefig(f"T_n_{data}.png", dpi=120)
    print("saved", f"T_n_{data}.png")

# Каждый алгоритм: случайные данные против фрагментов
fig, axes = plt.subplots(1, 3, figsize=(16, 4.5))
for ax, alg in zip(axes, ALGS):
    for data, algs in table.items():
        points = algs.get(alg, [])
        ax.plot([p[0] for p in points], [p[1] for p in points], marker="o",
                label=TITLES.get(data, data))
    ax.set_title(alg)
    ax.set_xlabel("n — число элементов")
    ax.set_ylabel("T(n), мс")
    ax.grid(alpha=0.3)
    ax.legend(fontsize=8)
fig.tight_layout()
fig.savefig("T_n_compare.png", dpi=120)
print("saved", "T_n_compare.png")
