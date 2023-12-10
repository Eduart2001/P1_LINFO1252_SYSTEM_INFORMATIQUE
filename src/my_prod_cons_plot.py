import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# producer/consumer

df2 = pd.read_csv('../data/my_prod_cons.csv')

mean_values_2 = np.array([np.mean(df2[elem]) for elem in df2.columns[0:6]])

plt.figure(figsize=(10, 6))

plt.plot(df2.columns[0:6], mean_values_2, 'o-', label='TAS & my_semaphore', color='orange')


plt.title('Execution time by thread number - producer/consumer')
plt.xlabel('Thread number')
plt.ylabel('Time (s)')
plt.legend()
plt.grid(True)
plt.savefig('../data/my_prod_cons.pdf')
plt.show()