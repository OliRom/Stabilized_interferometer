import sys
import time


def is_float(string):
    try:
        float(string)
        return True
    except ValueError:
        return False


def keyboard_listener(syst):
    for line in sys.stdin:
        comm = line.strip()

        if is_float(comm):
            syst.set_phase(float(comm))
            print(f"Nouvelle phase à {round(syst.get_phase(), 2)} degrés.")

        elif comm.lower() == "q":
            print("Fin de l'exécution du programme.")
            syst.quit()
            return

        else:
            print("Commande non reconnue.")
