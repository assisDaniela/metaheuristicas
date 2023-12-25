# Descrição
- O trabalho consiste na implementação de métodos heurísticos para resolver uma variante do Problema do Caxeiro Viajante (TSP)
- No TSP, dado um conjunto de *n* cidades e a distância entre cada par de cidade, o objetivo é determinar a rota de menor distância total que visita todas as cidades extamente uma vez
- Como variação, cada cidade pode indicar quando prefere receber a visita. As cidades que não forem visitadas conforme a preferência geram penalidades estipuladas por cada cidade. Sendo assim, o custo total de uma rota é então o custo de percorê-la somado ao custo das multas
- O trabalho foi dividido em 3 partes:
1. entender o problema, as entradas e formular a função de avaliação
2. gerar uma solução inicial (método construtivo)
    - aplicar a heurística do vizinho mais próximo de duas formas: acrescentando cidades sempre ao final da rota; acrescentando cidades nos dois extremos
3. melhorar a solução inicial até chegar em um ótimo local (busca local)
    - aplicação da vizinhança 2-opt e da estratégia *best improvement*

# Instâncias
- As instâncias utilizadas estão em *instancias.zip*

# Execução do código
Comando make no terminal gera três executáveis referentes a cada uma das três partes do trabalho: ./p0, ./p1 e ./p2
Ao executar ./px (x = 0, 1 ou 2), o programa espera pelo nome da instância e o tipo de penalidade. Se não houver penalidade, informe -1.

Para facilitar, basta executar o comando:
``` bash
# x = 0, 1 ou 2
./px < input.txt
```