import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('resultado_relativo_ao_melhor_conhecido.csv', delimiter=';')
df = df.drop('Melhor Conhecido', axis=1)
data = []

for column in df.columns:
    dataColumn = df[column]
    dataColumn = [ str(x) for x in dataColumn ]
    dataColumn = [ float(x.replace(',', '.')) for x in dataColumn ]
    data.append(dataColumn)

fig, ax = plt.subplots(1, 1)
ax.boxplot(data)
plt.tight_layout()
plt.grid()
plt.savefig('boxplot2.png')
plt.close()
