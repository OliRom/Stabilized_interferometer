import system_class as sc
import parameters as para
import functions as fun
import threading
import time
import serial
import numpy as np

if __name__ == "__main__":
    # Création du l'objet qui contrôle tout le système
    syst = sc.StabilizedInterferometer()

    # Connection avec les autres appareils
    syst.connect_lkin(para.lkin_name)
    syst.connect_arduino()
    syst.connect_socket()

    # Initialisation et activation du lockin
    syst.lkin_setup()
    syst.lkin_activation()

    # Calibration de l'amplitude des 2 harmoniques
    if not para.bypass_calibration:
        syst.demod_calibration()

    # Activation du monitorage de stdin pour commander le système à partir du clavier de l'ordinateur
    listener_thread = threading.Thread(target=fun.keyboard_listener, args=[syst])
    listener_thread.start()

    # Boucle principale du programme
    while syst.running:
        syst.read_socket_command()
        syst.read_arduino_command()
