"""Просмотр results.csv в виде таблиц (для Google Colab).

Загрузите results.csv в файлы Colab слева и запустите ячейку.
"""
import pandas as pd

df = pd.read_csv("results.csv", sep=";")

for data, name in [("random", "Случайные массивы"),
                   ("fragments", "Массивы из 10 упорядоченных фрагментов")]:
    print(name + ", среднее время в мс:")
    table = df[df["data"] == data].drop(columns="data").set_index("n")
    display(table.fillna("—"))
