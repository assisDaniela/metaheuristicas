# Descrição
- O trabalho consiste na implementação de métodos heurísticos para resolver uma variante do Problema do Caxeiro Viajante (TSP)
- No TSP, dado um conjunto de *n* cidades e a distância entre cada par de cidade, o objetivo é determinar a rota de menor distância total que visita todas as cidades extamente uma vez
- Como variação, cada cidade pode indicar quando prefere receber a visita. As cidades que não forem visitadas conforme a preferência geram penalidades estipuladas por cada cidade. Sendo assim, o custo total de uma rota é então o custo de percorê-la somado ao custo das multas
- Para este trabalho, deve ser aplicado metaheurísticas para melhorar os resultados obtidos no *Trabalho 1*
- Foram aplicadas 3 metaheurísticas
    - Simulated Annealing (arquivo *SA.cpp*)
    - GRASP (arquivo *Grasp.cpp*)
    - Iterated Local Search (arquivo *ILS.cpp*)
- Também foi analisada interferência causada pelo método utilizado para construção de uma solução inicial

# Instâncias
- As instâncias utilizadas estão na pasta *instancias*

# Execução do código
- Comando make no terminal gera três executáveis referentes a cada uma das três metaheurísticas aplicadas: ils, sa e grasp
- Ao executar, o programa espera pelo nome da instância e o tipo de penalidade. Se não houver penalidade, informe -1.

Para facilitar, basta executar o comando:
``` bash
./nome_executavel < input.txt
```