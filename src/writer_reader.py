import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# writer/reader

df = pd.read_csv('../data/pthread_writer_reader.csv')

mean_values = np.array([np.mean(df[elem]) for elem in df.columns[0:6]])
variance_values = np.array([np.var(df[elem]) for elem in df.columns[0:6]])

plt.figure(figsize=(10, 6))

plt.plot(df.columns[0:6], mean_values, 'o-', label='POSIX', color='tab:blue')
plt.fill_between(df.columns[0:6], mean_values[0:6] - variance_values[0:6],
                 mean_values[0:6] + variance_values[0:6], alpha=0.3, label='POSIX variance', color='blue')

plt.title('Execution time by thread number - writer/reader')
plt.xlabel('Thread number')
plt.ylabel('Time (s)')
plt.legend()
plt.grid(True)
plt.savefig('../data/pthread_writer_reader.pdf')
plt.show()