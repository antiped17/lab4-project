"""Построение графиков T(n) по результатам benchmark (results/results.csv).

Запуск:  python plot.py [каталог_с_results.csv]
Нужен matplotlib:  pip install matplotlib
"""
import csv
import math
import sys
from collections import defaultdict
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt  # noqa: E402
from matplotlib.ticker import FuncFormatter  # noqa: E402

DATA_TITLES = {
    "random": "случайные массивы",
    "fragments": "массивы из упорядоченных фрагментов",
    "nearly_sorted": "почти упорядоченные массивы (5 % перестановок)",
}
STYLE = {
    "insertion_sort": dict(color="#d62728", marker="o", label="insertion_sort, O(n²)"),
    "merge_sort": dict(color="#1f77b4", marker="s", label="merge_sort, O(n log n)"),
    "std::sort": dict(color="#2ca02c", marker="^", label="std::sort, O(n log n)"),
}
THEORY = {
    "insertion_sort": (lambda n: n * n, "c·n²"),
    "merge_sort": (lambda n: n * math.log2(n), "c·n·log n"),
    "std::sort": (lambda n: n * math.log2(n), "c·n·log n"),
}


def thousands(x, _pos=None):
    """50000 -> '50 000'."""
    return f"{int(x):,}".replace(",", " ")


def load(path):
    # data -> algorithm -> [(n, avg_ms)]
    table = defaultdict(lambda: defaultdict(list))
    with open(path, encoding="utf-8") as f:
        for row in csv.DictReader(f):
            table[row["data"]][row["algorithm"]].append((int(row["n"]), float(row["avg_ms"])))
    for algs in table.values():
        for pts in algs.values():
            pts.sort()
    return table


def fit(points, g):
    """Коэффициент c для T(n) ≈ c·g(n) по методу наименьших квадратов."""
    num = sum(t * g(n) for n, t in points)
    den = sum(g(n) ** 2 for n, _ in points)
    return num / den


def draw(ax, algs, names, n_max, with_theory=True):
    for name in names:
        pts = [(n, t) for n, t in algs.get(name, []) if n <= n_max]
        if not pts:
            continue
        xs, ys = zip(*pts)
        ax.plot(xs, ys, linewidth=1.8, markersize=5, **STYLE[name])
        if with_theory:
            g, label = THEORY[name]
            c = fit(pts, g)
            grid = [xs[0] + (xs[-1] - xs[0]) * i / 200 for i in range(201)]
            ax.plot(grid, [c * g(n) for n in grid], linestyle="--", linewidth=1,
                    color=STYLE[name]["color"], alpha=0.6,
                    label=f"{name}: {label}, c = {c:.2e}")
    ax.set_xlabel("n — число элементов")
    ax.set_ylabel("T(n), мс")
    ax.xaxis.set_major_formatter(FuncFormatter(thousands))
    ax.grid(True, alpha=0.3)
    ax.legend(fontsize=8)


def main():
    folder = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("results")
    table = load(folder / "results.csv")
    all_algs = list(STYLE)
    fast_algs = ["merge_sort", "std::sort"]

    for data, algs in table.items():
        quad_max = max(n for n, _ in algs["insertion_sort"])
        fig, (left, right) = plt.subplots(1, 2, figsize=(14, 5.5))
        draw(left, algs, all_algs, quad_max)
        left.set_title(f"Все алгоритмы, n ≤ {thousands(quad_max)}")
        draw(right, algs, fast_algs, float("inf"))
        right.set_title("Алгоритмы O(n log n) на больших n")
        fig.suptitle(f"Среднее время сортировки: {DATA_TITLES.get(data, data)}", fontsize=13)
        fig.tight_layout()
        out = folder / f"T_n_{data}.png"
        fig.savefig(out, dpi=130)
        plt.close(fig)
        print("сохранён", out)

    # Как тип входных данных влияет на каждый алгоритм.
    fig, axes = plt.subplots(1, 3, figsize=(17, 5))
    for ax, name in zip(axes, all_algs):
        for data, algs in table.items():
            pts = algs.get(name, [])
            if pts:
                xs, ys = zip(*pts)
                ax.plot(xs, ys, marker="o", markersize=4, label=DATA_TITLES.get(data, data))
        ax.set_title(name)
        ax.xaxis.set_major_formatter(FuncFormatter(thousands))
        ax.set_xlabel("n — число элементов")
        ax.set_ylabel("T(n), мс")
        ax.grid(True, alpha=0.3)
        ax.legend(fontsize=8)
    fig.suptitle("Влияние упорядоченности данных на время работы", fontsize=13)
    fig.tight_layout()
    out = folder / "T_n_by_data.png"
    fig.savefig(out, dpi=130)
    plt.close(fig)
    print("сохранён", out)

    # Логарифмический масштаб: наклон прямой показывает степень роста.
    fig, ax = plt.subplots(figsize=(8, 6))
    for name in all_algs:
        pts = table["random"].get(name, [])
        if pts:
            xs, ys = zip(*pts)
            ax.loglog(xs, ys, linewidth=1.8, markersize=5, **STYLE[name])
    ax.set_title("Случайные массивы, логарифмический масштаб")
    ax.set_xlabel("n (log)")
    ax.set_ylabel("T(n), мс (log)")
    ax.grid(True, which="both", alpha=0.3)
    ax.legend()
    fig.tight_layout()
    out = folder / "T_n_loglog.png"
    fig.savefig(out, dpi=130)
    plt.close(fig)
    print("сохранён", out)


if __name__ == "__main__":
    main()
