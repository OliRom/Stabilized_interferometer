import zhinst.toolkit as zh
import numpy as np
import tkinter as tk


class StabilizedInterferometer:
    def __init__(self):
        self.device = None
        self.phase = 0
        self.k1, self.k2 = 1, 10

    def connect_lkin(self, device_name, host="localhost"):
        session = zh.Session(host, hf2=True)
        self.device = session.connect_device(device_name)

    def connect_arduino(self):
        pass

    def lkin_setup(self, pid_params=(10, 1, 1)):
        # Génération du signal à 1kHz
        self.device.oscs[0].freq(1000)
        sig = self.device.sigouts[0]
        sig.amplitudes(0.02)
        sig.range(10)
        sig.offset(0.0)
        sig.add(True)

        # Réception du signal de la photodiode
        sig = self.device.sigins[0]
        sig.range(1.2)
        sig.ac(False)
        sig.imp50(False)
        sig.diff(False)

        # Setup des démodulateurs
        for i in range(2):
            demod = self.device.demods[i]
            demod.oscselect(i)
            demod.harmonic(i+1)

        # Envoyer la sortie des démodulateurs aux sorties auxiliaires
        for i in range(2):
            aux = self.device.auxouts[i]
            aux.outputselect(2)  # outputselect = 2: module du signal du démodulateur (R)
            aux.demodselect(i)
            aux.scale(1.0)
            aux.offset(0.0)

        # Initialiser le PID
        pid = self.device.pids[0]
        pid.input(4)  # input = 4: entrée auxin
        pid.inputchanne(0)
        pid.output(3)  # output = 3: sortie auxout
        pid.outputchannel(2)
        pid.outputdefaultenable(False)
        pid.p(pid_params[0])
        pid.i(pid_params[1])
        pid.d(pid_params[2])
        pid.setpointselect(0)
        pid.setpoint(0)
        pid.monitoroffset(0)
        pid.monitorscale(1)
        pid.center(0)
        pid.range(3)

    def lkin_activation(self):
        # Activation des canaux du lockin
        self.device.sigouts[0].on(True)
        self.device.pids[0].enable(True)

    def set_phase(self, phase):
        """
        Définit la phase de l'interféromètre en degrés.

        :param phase: phase (degrés)
        """
        auxs = self.device.auxouts
        auxs[0].scale(self.k1 * np.cos(phase * np.pi / 180))
        auxs[1].scale(self.k2 * np.sin(phase * np.pi / 180))

        self.phase = self.get_phase()

    def get_phase(self):
        auxs = self.device.auxouts
        x = auxs[0].scale() / self.k1
        y = auxs[1].scale() / self.k2

        return np.arctan2(y, x)

    def set_k1(self, value):
        self.k1 = float(value)
        self.set_phase(self.phase)

    def set_k2(self, value):
        self.k2 = float(value)
        self.set_phase(self.phase)

    def demod_calibration(self):
        """
        Procédure pour calibrer les constantes k1 et k2 qui font que les 2 harmoniques démodulées tracent un cercle
        à l'oscilloscope lorsque la phase de l'interféromètre varie.
        """
        # Création de la fenêtre principale
        window = tk.Tk()
        window.title("Calibration")
        window.configure(bg="sky blue")  # Couleur d'arrière-plan
        window.iconbitmap("Anneaux_interference.ico")

        # Frame pour contenir les curseurs et les étiquettes
        frame = tk.Frame(window, bg="sky blue")
        frame.pack(pady=20)

        # Texte au-dessus des curseurs
        texte = """
        Ajuster la position des curseurs pour que les 2
        premières harmoniques du signal tracent un cercle à
        l'oscilloscope lorsque la phase de l'interféromètre varie.
        """
        text_label = tk.Label(frame, text=texte, bg="sky blue")
        text_label.grid(row=0, columnspan=2)

        # Curseur 1 (horizontal)
        scale_x = tk.Scale(frame, from_=0.01, to=100, orient=tk.HORIZONTAL, command=self.set_k1, variable=self.k1,
                           troughcolor="gold", bg="pale green", highlightthickness=0)
        scale_x.set(self.k1)
        scale_x.grid(row=1, column=0, padx=10)

        label_x = tk.Label(frame, text="x: harm 1", bg="sky blue")
        label_x.grid(row=2, column=0)

        # Curseur 2 (vertical)
        scale_y = tk.Scale(frame, from_=100, to=0.01, orient=tk.VERTICAL, command=self.set_k2, variable=self.k2,
                           troughcolor="gold", bg="pale green", highlightthickness=0)
        scale_y.set(self.k2)
        scale_y.grid(row=1, column=1, padx=10)

        label_y = tk.Label(frame, text="y: harm 2", bg="sky blue")
        label_y.grid(row=2, column=1)


        # Fonction de redimensionnement de la fenêtre
        def on_window_resize(event):
            frame.place(relx=0.5, rely=0.5, anchor=tk.CENTER)


        # Associer la fonction de redimensionnement à l'événement de redimensionnement de la fenêtre
        window.bind('<Configure>', on_window_resize)

        # Lancement de la boucle principale de la fenêtre
        window.mainloop()


if __name__ == "__main__":
    syst = StabilizedInterferometer()
    syst.connect_lkin("DEV415")
    syst.lkin_setup()
    syst.lkin_activation()
    syst.demod_calibration()
