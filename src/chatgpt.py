import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('data/result.csv')

# Calcular el tiempo medio por hilo
df['tiempo_medio'] = np.mean(df.iloc[:, 1:], axis=1)

# Calcular la variación
df['variacion'] = np.std(df.iloc[:, 1:], axis=1)

# Crear el gráfico de tiempo medio por hilo
plt.figure(figsize=(10, 6))
plt.errorbar(df.columns[1:-1], df['tiempo_medio'][1:-1], yerr=df['variacion'][1:-1], fmt='o-', label='Tiempo Medio')
plt.title('Tiempo Medio de Ejecución por Hilo con Variación (Usando NumPy)')
plt.xlabel('Número de Hilos')
plt.ylabel('Tiempo Medio (s)')
plt.legend()
plt.grid(True)
plt.savefig('data/tiempo_medio_por_hilo_numpy.pdf')
plt.show()

# Crear el gráfico de variación
plt.figure(figsize=(10, 6))
plt.plot(df.columns[1:-1], df['variacion'][1:-1], 'o-', label='Variación')
plt.title('Variación en el Tiempo de Ejecución por Hilo (Usando NumPy)')
plt.xlabel('Número de Hilos')
plt.ylabel('Variación (s)')
plt.legend()
plt.grid(True)
plt.savefig('data/variacion_por_hilo_numpy.pdf')
plt.show()