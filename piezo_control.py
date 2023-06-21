import zhinst.toolkit as zh


def connect_lkin(device_name, host="localhost"):
    session = zh.Session(host, hf2=True)
    device = session.connect_device(device_name)
    return device


def lkin_setup(device, pid_params=(10, 1, 1)):
    # Génération du signal à 1kHz
    device.oscs[0].freq(1000)
    sig = device.sigouts[0]
    sig.amplitudes(0.02)
    sig.range(10)
    sig.offset(0.0)
    sig.add(True)

    # Réception du signal de la photodiode
    sig = device.sigins[0]
    sig.range(1.2)
    sig.ac(False)
    sig.imp50(False)
    sig.diff(False)

    # Setup des démodulateurs
    for i in range(2):
        demod = device.demods[i]
        demod.oscselect(i)
        demod.harmonic(i+1)

    # Envoyer la sortie des démodulateurs aux sorties auxiliaires
    for i in range(2):
        aux = device.auxouts[i]
        aux.outputselect(2)  # outputselect = 2: module du signal du démodulateur (R)
        aux.demodselect(i)
        aux.scale(1.0)
        aux.offset(0.0)

    # Initialiser le PID
    pid = device.pids[0]
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

    # Activation des canaux
    device.sigouts[0].on(True)
    device.pids[0].enable(True)


if __name__ == "__main__":
    lkin = connect_lkin("DEV415")
    lkin_setup(lkin)
