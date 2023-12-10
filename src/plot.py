import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv("../data/pthread_philosophes.csv")

threads_data = ["1", "2", "4", "8"]
mean_values = list()
variance_values = list()
for elem in threads_data:
    mean_values.append(np.mean(df[elem]))
    variance_values.append(np.var(df[elem]))

plt.title("Calcul du temps de compilation du projet\n \"Modest\" en fonction du nombre de threads utilises")
plt.plot(threads_data, mean_values)
#plt.plot(threads_data, variance_values)
#plt.legend(["Means", "Variances"])
plt.xlabel("Nombre de threads")
plt.ylabel("Vitesse de compilation (en secondes)")
plt.show()