"""Просмотр results.csv в виде таблиц (для Google Colab).

Загрузите results.csv в файлы Colab слева и запустите ячейку.
"""
import pandas as pd

df = pd.read_csv("results.csv", sep=";")

for type_ in ["int", "double"]:
    for data, name in [("random", "случайные массивы"),
                       ("fragments", "массивы из 10 упорядоченных фрагментов")]:
        print(f"{type_}, {name}, среднее время в мс:")
        table = df[(df["type"] == type_) & (df["data"] == data)]
        display(table.drop(columns=["type", "data"]).set_index("n").fillna("—"))
