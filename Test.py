import zhinst.toolkit as zh
import numpy as np
import matplotlib.pyplot as plt
import time
from sys import stdin
import tkinter as tk

if __name__ == "__main__":
    # a=b=0
    # for line in stdin:
    #     line = line.strip()
    #     if line == "a":
    #         a-=0.1
    #     elif line == "d":
    #         a+=0.1
    #     elif line == "s":
    #         b-=0.1
    #     elif line == "w":
    #         b+=0.1
    #     elif line == "q":
    #         break
    #     elif line.startswith("x "):
    #         a=float(line[2:])
    #     elif line.startswith("y "):
    #         b=float(line[2:])
    #     else:
    #         print("Entrée invalide")
    #     print(a, b)
    # print(stdin)

    import tkinter as tk

    def update_variable1(value):
        variable1.set(value)

    def update_variable2(value):
        variable2.set(value)

    # Création de la fenêtre principale
    window = tk.Tk()
    window.title("Curseurs")
    window.configure(bg="sky blue")  # Couleur d'arrière-plan

    # Variables pour stocker les valeurs ajustées par les curseurs
    variable1 = tk.DoubleVar()
    variable2 = tk.DoubleVar()

    # Frame pour contenir les curseurs et les étiquettes
    frame = tk.Frame(window, bg="sky blue")
    frame.pack(pady=20)

    # Curseur 1 (horizontal)
    scale1 = tk.Scale(frame, from_=-100, to=100, orient=tk.HORIZONTAL, command=update_variable1, variable=variable1, troughcolor="sky blue")
    scale1.grid(row=0, column=0, padx=10)

    label1 = tk.Label(frame, text="x: harm 1", bg="sky blue")
    label1.grid(row=1, column=0)

    # Curseur 2 (vertical)
    scale2 = tk.Scale(frame, from_=-100, to=100, orient=tk.VERTICAL, command=update_variable2, variable=variable2, troughcolor="sky blue")
    scale2.grid(row=0, column=1, padx=10)

    label2 = tk.Label(frame, text="y: harm 2", bg="sky blue")
    label2.grid(row=1, column=1)

    # Fonction de redimensionnement de la fenêtre
    def on_window_resize(event):
        frame.place(relx=0.5, rely=0.5, anchor=tk.CENTER)

    # Associer la fonction de redimensionnement à l'événement de redimensionnement de la fenêtre
    window.bind('<Configure>', on_window_resize)

    # Lancement de la boucle principale de la fenêtre
    window.mainloop()


