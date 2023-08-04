# Lock-In parameters
lkin_name = "DEV415"  # Nom d'identification du lockin
osc_freq = 2e3  # Fréquence d'oscillation du piezo
osc_amp = 30e-3  # Amplitude de l'oscillation du piezo
demod_tm_cste = 0.00346145642  # Constante de temps des démodulateurs
demod_filter_order = 4  # Ordre des filtres passe-bas des démodulateurs
pid_param = (75e-3, 7, 0)  # Paramètres du PID du lockin (P, I, D)
k1, k2 = 1.65, 38.0  # Valeurs par défaut pour multiplier les 2 harmoniques démodulées
bypass_calibration = False  # Mettre à True pour ignorer la calibration des 2 harmoniques (déconseillé)

# Socket parameters
host = "127.0.0.1"
port = 65432
