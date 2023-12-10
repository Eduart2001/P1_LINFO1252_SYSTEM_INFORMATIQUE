import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# producer/consumer

df = pd.read_csv('../data/pthrerad_prod_cons.csv')

mean_values = np.array([np.mean(df[elem]) for elem in df.columns[0:6]])
variance_values = np.array([np.var(df[elem]) for elem in df.columns[0:6]])

plt.figure(figsize=(10, 6))

plt.plot(df.columns[0:6], mean_values, 'o-', label='POSIX', color='tab:blue')
plt.fill_between(df.columns[0:6], mean_values[0:6] - variance_values[0:6],
                 mean_values[0:6] + variance_values[0:6], alpha=0.3, label='POSIX variance', color='blue')

plt.title('Execution time by thread number - producer/consumer')
plt.xlabel('Thread number')
plt.ylabel('Time (s)')
plt.legend()
plt.grid(True)
plt.savefig('../data/pthread_prod_cons.pdf')
plt.show()