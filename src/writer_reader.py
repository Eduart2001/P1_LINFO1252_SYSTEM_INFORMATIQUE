import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# writer/reader

df = pd.read_csv('../data/pthread_writer_reader.csv')
df2 = pd.read_csv('../data/my_writer_reader.csv')

mean_values = np.array([np.mean(df[elem]) for elem in df.columns[0:6]])
variance_values = np.array([np.var(df[elem]) for elem in df.columns[0:6]])

mean_values_2 = np.array([np.mean(df[elem]) for elem in df2.columns[0:6]])
variance_values_2 = np.array([np.var(df[elem]) for elem in df2.columns[0:6]])

plt.figure(figsize=(10, 6))

plt.plot(df.columns[0:6], mean_values, 'o-', label='POSIX')
plt.fill_between(df.columns[0:6], mean_values[0:6] - variance_values[0:6],
                 mean_values[0:6] + variance_values[0:6], alpha=0.3, label='POSIX variance', color='red')

plt.plot(df2.columns[0:6], mean_values_2, 'o-', label='TAS & my_semaphore')
plt.fill_between(df2.columns[0:6], mean_values_2 - variance_values_2,
                 mean_values_2 + variance_values_2, alpha=0.3, color='blue', label='TAS & my_semaphore variance')

plt.title('Execution time by thread number - writer/reader')
plt.xlabel('Thread number')
plt.ylabel('Time (s)')
plt.legend()
plt.grid(True)
plt.savefig('../data/pthread_writer_reader.pdf')
plt.show()