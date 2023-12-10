import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# writer/reader

df2 = pd.read_csv('../data/my_writer_reader.csv')

mean_values_2 = np.array([np.mean(df2[elem]) for elem in df2.columns[0:6]])
variance_values_2 = np.array([np.var(df2[elem]) for elem in df2.columns[0:6]])

plt.figure(figsize=(10, 6))

plt.plot(df2.columns[0:6], mean_values_2, 'o-', label='TAS & my_semaphore', color='orange')
plt.fill_between(df2.columns[0:6], mean_values_2 - variance_values_2,
                 mean_values_2 + variance_values_2, alpha=0.3, color='orange', label='TAS & my_semaphore variance')

plt.title('Execution time by thread number - writer/reader')
plt.xlabel('Thread number')
plt.ylabel('Time (s)')
plt.legend()
plt.grid(True)
plt.savefig('../data/my_writer_reader.pdf')
plt.show()