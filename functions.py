import sys


def is_float(string):
    """
    Détermine si un str peut être converti en float.
    :param string: str à évaluer
    :return: True si le str peut être converti en float
    """
    try:
        float(string)
        return True
    except ValueError:
        return False


def keyboard_listener(syst):
    """
    Écoute pour les commandes claviers envoyées durant l'exécution du programme et exécute certaines méthodes d'une
    instance de la classe StabilizedInterferometer lorsque certaines touches sont pesées.
    :param syst: instance de la classe StabilizedInterferometer
    """
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
