"""Графики T(n) по results.csv, который записывает Alg5.exe.

Запуск:  python plot.py      (нужен matplotlib: pip install matplotlib)
В Google Colab: загрузите results.csv в файлы слева и запустите ячейку —
графики появятся прямо под ней.
"""
import csv

import matplotlib.pyplot as plt

ALGS = ["insertionSort", "mergeSort", "std::sort"]
COLORS = {"insertionSort": "#d62728", "mergeSort": "#1f77b4", "std::sort": "#2ca02c"}
TITLES = {"random": "случайные массивы", "fragments": "массивы из 10 упорядоченных фрагментов"}

# (тип, данные) -> алгоритм -> список (n, время)
table = {}
with open("results.csv", encoding="utf-8") as f:
    for row in csv.DictReader(f, delimiter=";"):
        for alg in ALGS:
            if row[alg]:
                table.setdefault((row["type"], row["data"]), {}).setdefault(alg, []).append(
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


# 1. Для каждого типа (int, double) и каждого вида данных — свой график
for (type_, data), algs in table.items():
    fig, (left, right) = plt.subplots(1, 2, figsize=(13, 5))
    draw(left, algs, 50000)
    left.set_title("Все алгоритмы, n до 50 000")
    draw(right, {a: algs[a] for a in ["mergeSort", "std::sort"]}, 10 ** 9)
    right.set_title("O(n log n) на больших n")
    fig.suptitle(f"{type_}: {TITLES.get(data, data)}")
    fig.tight_layout()
    plt.show()

# 2. Каждый алгоритм: int против double на случайных массивах
fig, axes = plt.subplots(1, 3, figsize=(16, 4.5))
for ax, alg in zip(axes, ALGS):
    for type_ in ["int", "double"]:
        points = table.get((type_, "random"), {}).get(alg, [])
        ax.plot([p[0] for p in points], [p[1] for p in points], marker="o", label=type_)
    ax.set_title(f"{alg}: int и double (случайные)")
    ax.set_xlabel("n — число элементов")
    ax.set_ylabel("T(n), мс")
    ax.grid(alpha=0.3)
    ax.legend()
fig.tight_layout()
plt.show()

# 3. Каждый алгоритм: случайные данные против фрагментов (int)
fig, axes = plt.subplots(1, 3, figsize=(16, 4.5))
for ax, alg in zip(axes, ALGS):
    for data in ["random", "fragments"]:
        points = table.get(("int", data), {}).get(alg, [])
        ax.plot([p[0] for p in points], [p[1] for p in points], marker="o", label=TITLES[data])
    ax.set_title(f"{alg}: случайные и фрагменты (int)")
    ax.set_xlabel("n — число элементов")
    ax.set_ylabel("T(n), мс")
    ax.grid(alpha=0.3)
    ax.legend(fontsize=8)
fig.tight_layout()
plt.show()
