import time
import zhinst.toolkit as zh
import numpy as np
import tkinter as tk
import threading
import serial as seri
from serial.tools.list_ports import comports
import socket
import parameters as para


def get_arduino_port():
    arduino_ports = list()
    for port in comports():
        if "Arduino Leonardo" in port.description:
            arduino_ports.append(port)

    return arduino_ports


class StabilizedInterferometer:
    def __init__(self):
        """
        Interféromètre de Michelson stabilisé en phase
        """
        self.device = None
        self.phase = 0
        self.k1, self.k2 = para.k1, para.k2
        self.frange_defilation = False

    def connect_lkin(self, device_name, host="localhost"):
        """
        Ouvrir une session avec un lockin connecté à l'ordinateur.
        :param device_name: nom du lockin
        :param host: hôte
        """
        session = zh.Session(host, hf2=True)
        self.device = session.connect_device(device_name)

    def connect_arduino(self, port=None):
        """
        Fonction qui scanne tous les ports de l'ordi pour trouver un Arduino. Les paramètres peuvent être utilisés
        pour faciliter ou cibler la recherche avec l'ordi.
        :param port: port de l'ordi sur lequel le Arduino est branché
        """

        if port is not None:
            self.ardui = seri.Serial(port, 9600)
        else:
            self.ardui = seri.Serial(get_arduino_port()[0].name)

    def connect_socket(self, host=para.host, port=para.port):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind((host, port))
        self.sock.listen(1)
        print("En attente d'une connection...")
        self.sock_conn, client_address = self.sock.accept()
        print(f"Connection établie avec: {client_address}")
        self.sock_conn.settimeout(0.1)

    def lkin_setup(self, pid_params=para.pid_param):
        """
        Fonction à appeler au début de la prise de mesures pour initialiser le lockin avec les paramètres à utiliser.
        :param pid_params: paramètres du pid (P, I, D)
        """
        # Génération du signal à 1kHz
        self.device.oscs[0].freq(para.osc_freq)
        sig = self.device.sigouts[0]
        sig.amplitudes(para.osc_amp / 10)  # Il faut diviser par 10 pour avoir la bonne valeur (???)
        sig.range(10)
        sig.offset(0.0)
        sig.add(True)

        # Réception du signal de la photodiode
        sig = self.device.sigins[0]
        sig.range(2)
        sig.ac(False)
        sig.imp50(False)
        sig.diff(False)

        # Setup des démodulateurs
        for i in range(2):
            demod = self.device.demods[i]
            demod.oscselect(0)
            demod.harmonic(i + 1)
            demod.order(para.demod_filter_order)
            demod.timeconstant(para.demod_tm_cste)

        # Initialiser le PID
        pid = self.device.pids[0]
        pid.input(4)  # input = 4: entrée auxin
        pid.inputchanne(0)
        pid.output(3)  # output = 3: sortie auxout
        pid.outputchannel(2)
        pid.outputdefaultenable(True)
        pid.outputdefault(0)
        pid.p(pid_params[0])
        pid.i(pid_params[1])
        pid.d(pid_params[2])
        pid.setpointselect(0)
        pid.setpoint(0)
        pid.monitoroffset(0)
        pid.monitorscale(1)
        pid.center(0)
        pid.range(3)

        # Configurer les sorties auxiliaires
        auxs = self.device.auxouts
        for i in range(2):
            aux = auxs[i]
            aux.outputselect(0)  # outputselect = 0: composante x du signal du démodulateur (X)
            aux.demodselect(i)
            aux.scale(1.0)
            aux.offset(0.0)

        aux = auxs[2]
        aux.outputselect(-2)  # outputselect = -2: PID 1

    def lkin_activation(self):
        """
        Activation des canaux du lockin.
        """
        self.device.sigouts[0].on(1)
        self.activate_pid()

    def activate_pid(self, enable=1):
        """
        Activation du PID du lockin.
        :param enable: True si le PID est activé
        """
        self.device.pids[0].enable(enable)

    def set_phase(self, phase):
        """
        Définit la phase de l'interféromètre en degrés. Détecte la petite variation de phase causée par la
        digitalisation du lockin et la corrige dans l'attribut "self.phase".
        :param phase: phase (degrés)
        """
        auxs = self.device.auxouts
        auxs[0].scale(self.k1 * 100 * np.cos(phase * np.pi / 180))
        auxs[1].scale(self.k2 * 100 * np.sin(phase * np.pi / 180))

        self.phase = self.get_phase()

    def get_phase(self):
        """
        Retourne la phase de l'interféromètre
        :return: phase de l'interféromètre (degrés)
        """
        auxs = self.device.auxouts
        x = auxs[0].scale() / self.k1
        y = auxs[1].scale() / self.k2

        return np.arctan2(y, x) * 180 / np.pi

    def set_k1(self, value):
        """
        Définit la constante k1 pour la normalisation des signaux de démodulation.
        :param value: valeur de k1
        """
        self.k1 = -float(value)
        self.set_phase(45)

    def set_k2(self, value):
        """
        Définit la constante k2 pour la normalisation des signaux de démodulation.
        :param value: valeur de k2
        """
        self.k2 = float(value)
        self.set_phase(45)

    def defile_franges(self):
        """
        Fonction qui arrête le PID pour faire défiler des franges à l'interféromètre. Lorsque la fonction arrête, le PID
        est reparti à son état initial. Il faut mettre l'attribut "self.frange_defilation" à False pour que les franges
        arrêtent de défiler.
        """
        self.frange_defilation = True

        aux = self.device.auxouts[2]
        offset = 0
        increase = True

        pid = self.device.pids[0]
        pid_state = pid.enable()
        pid.enable(0)

        while self.frange_defilation:
            if increase:
                offset += 0.005
            else:
                offset -= 0.005

            aux.offset(offset)
            pid.monitoroffset(offset)
            time.sleep(0.02)

            if offset >= 3:
                increase = False
            elif offset <= -3:
                increase = True

        pid.monitoroffset(0)
        pid.enable(pid_state)

    def demod_calibration(self):
        """
        Procédure pour calibrer les constantes k1 et k2 qui font que les 2 harmoniques démodulées tracent un cercle
        à l'oscilloscope lorsque la phase de l'interféromètre varie.
        """
        # Création de la fenêtre principale
        window = tk.Tk()
        window.geometry("400x250")
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
        scale_x = tk.Scale(frame, from_=0.01, to=20, resolution=0.05, orient=tk.HORIZONTAL, command=self.set_k1,
                           variable=self.k1, troughcolor="gold", bg="pale green", highlightthickness=0)
        scale_x.set(self.k1)
        scale_x.grid(row=1, column=0, padx=10)

        label_x = tk.Label(frame, text="x: harm 1", bg="sky blue")
        label_x.grid(row=2, column=0)

        # Curseur 2 (vertical)
        scale_y = tk.Scale(frame, from_=200, to=0.1, resolution=0.5, orient=tk.VERTICAL, command=self.set_k2,
                           variable=self.k2, troughcolor="gold", bg="pale green", highlightthickness=0)
        scale_y.set(self.k2)
        scale_y.grid(row=1, column=1, padx=10)

        label_y = tk.Label(frame, text="y: harm 2", bg="sky blue")
        label_y.grid(row=2, column=1)

        # Bouton de validation
        def validate():
            self.frange_defilation = False
            window.destroy()

        validate_but = tk.Button(frame, text="Valider", command=validate)
        validate_but.grid(row=3, columnspan=2)

        # Fonction de redimensionnement de la fenêtre
        def on_window_resize(event):
            frame.place(relx=0.5, rely=0.5, anchor=tk.CENTER)

        # Associer la fonction de redimensionnement à l'événement de redimensionnement de la fenêtre
        window.bind('<Configure>', on_window_resize)

        # Faire défiler les franges durant la calibration
        running = True
        thread = threading.Thread(target=self.defile_franges)
        thread.start()

        # Lancement de la boucle principale de la fenêtre
        window.mainloop()

    def go_to(self, pos, precision):
        self.send_arduino_command(self.format_command("@", [pos, precision]))

    def get_position(self):
        self.send_arduino_command(self.format_command("?"))

    def get_max_pos(self):
        self.send_arduino_command(self.format_command("["))

    def calibrate(self):
        self.send_arduino_command(self.format_command("("))

    def go_to_resp(self, pos):
        self.send_socket_command(self.format_command(" ", [pos]))

    def get_position_resp(self, pos):
        self.send_socket_command(self.format_command("!", [pos]))

    def get_max_pos_resp(self, pos):
        self.send_socket_command(self.format_command("]", [pos]))

    def calibrate_resp(self):
        self.send_socket_command(self.format_command(")"))

    def read_arduino_command(self, n=1):
        for i in range(n):
            if self.ardui.inWaiting() > 0:
                code, args = self.decod_command(self.ardui.readline().decode("ascii"))

                if code == "!":
                    self.get_position_resp(*args)
                elif code == " ":
                    self.go_to_resp(*args)
                elif code == "]":
                    self.get_max_pos_resp(*args)
                elif code == ")":
                    self.calibrate_resp()

    def read_socket_command(self, n=1):
        for i in range(n):
            try:
                data = self.sock_conn.recv(1024).decode("ascii")
                if data == "":
                    print("Le socket a été déconnecté.")
                    self.connect_socket()
                    break

                code, args = self.decod_command(data)

                if code == "?":
                    self.sock_conn.sendall("Message recu".encode("ascii"))
                    self.get_position()
                elif code == "@":
                    self.go_to(*args)
                elif code == "[":
                    self.get_max_pos()
                elif code == "(":
                    self.calibrate()

            except:
                pass

    def send_arduino_command(self, command):
        self.ardui.write(command.encode("ascii"))

    def send_socket_command(self, command):
        self.sock_conn.sendall(command.encode("ascii"))

    @staticmethod
    def format_command(commande, args=()):
        comm = "\x01" + commande
        for a in args:
            comm += str(a) + "\x2C"

        if len(args) > 0:
            return comm[:-1] + "\n"
        else:
            return comm + "\n"

    @staticmethod
    def decod_command(commande):
        try:
            commande = commande.strip()
            commande = commande.partition(chr(0x01))[2]
            code = commande[0]
            args = commande[1:].split(",")

            return code, args

        except:
            return None, None


if __name__ == "__main__":
    syst = StabilizedInterferometer()
    syst.connect_lkin(para.lkin_name)
    syst.lkin_setup()
    syst.lkin_activation()
    if not para.bypass_calibration:
        syst.demod_calibration()
    # syst.connect_arduino()
    # syst.connect_socket()

    # while True:
    #     syst.read_socket_command()
    #     syst.read_arduino_command()

    # syst.calibrate()
    # syst.get_position()
    # syst.go_to(50000, 20)
    # syst.get_max_pos()

    # while True:
    #     syst.read_arduino_command()

    while True:
        comm = input("Phase: ").strip()
        if comm == "q":
            break
        syst.set_phase(float(comm))
        print(syst.get_phase())
