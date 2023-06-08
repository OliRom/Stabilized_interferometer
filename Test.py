import zhinst.toolkit as zh
import numpy as np
import matplotlib.pyplot as plt
import time

if __name__ == "__main__":
    lkin_session = zh.Session("localhost", hf2=True)
    lkin = lkin_session.connect_device("DEV415")

    lkin.demods[0].enable(1)
    lkin.demods[0].sample.subscribe()
    time.sleep(1)
    lkin.demods[0].sample.unsubscribe()
    results = lkin_session.poll()
    demod_sample = results[lkin.demods[0].sample]

    plt.plot(demod_sample["x"])
    plt.show()

