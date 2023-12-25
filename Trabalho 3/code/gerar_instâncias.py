import os
from random import randint

N = ['100', '300', '500', '1000']
C = ['50', '200', '400', '750', '1000']
minT = '0.1'
maxT = '0.7'
Q = [2, 5, 7]

def findNextNotColored(coloreds, curr):
    i = curr
    while True:
        if not coloreds[i]:
            return i
        
        i = 0 if i+1 == len(coloreds) else i+1

        if i == curr:
            return -1
        

for n in N:
    _n = int(n)
    for c in C:
        for q in Q:
            _q = int(q)
            instanceId = randint(0, 9)
            fileName = f'Randomly_Generated/BPP_{n}_{c}_{minT}_{maxT}_{instanceId}.txt'
            instance = []
            with open(fileName) as txtfile:
                instance = txtfile.readlines()

            instance = [ instance[i].strip() for i in range(2, len(instance)) ]
            for i in range(3):
                cbppInstance = [n, c]
                coloreds = [ False for j in range(_n) ]
                for j in range(1, _q+1):
                    colored = randint(0, _n-1)
                    colored = findNextNotColored(coloreds, colored)
                    
                    if colored == -1: break
                    item = [str(instance[colored]), str(j)]
                    item = ' '. join(item)
                    cbppInstance.append(item)

                for index, colored in enumerate(coloreds):
                    if colored: continue
                    color = str(randint(1, q))
                    item = [str(instance[index]), color]
                    item = ' '. join(item)
                    cbppInstance.append(item)

                fileCbppName = f'instances/{n}_{c}_{q}_{i}.txt'
                print(fileCbppName)
                with open(fileCbppName, 'w') as txtfile:
                    for line in cbppInstance:
                        txtfile.write(line)
                        txtfile.write('\n')
