# Análise de Genoma do COVID-19

## Descrição

Este projeto realiza uma análise importante no campo da Biologia Molecular Computacional, focando na identificação de repetições de segmentos genômicos do coronavírus. Repetições em genomas podem fornecer informações cruciais; por exemplo, repetições concatenadas podem estar associadas a condições como a esclerose lateral amiotrófica (ELA).

## Objetivo

O programa lê o genoma do coronavírus em blocos consecutivos de 6 bases nitrogenadas (A, T, C, G) e conta quantas vezes cada bloco aparece no genoma completo. Cada linha do arquivo de entrada contém 60 caracteres, resultando em 10 blocos por linha.

### Exemplo:

Para a linha:

```
ATTAAAGGTTTATACCTTCCCAGGTAACAAACCAACCAACTTTCGATCTCTTGTAGATCT
```

Os blocos são:

1. ATTAAA
2. AGGTTT
3. ATACCT
   ...e assim por diante.

## Implementação

O projeto utiliza uma tabela de espalhamento (hash table) para armazenar e contar eficientemente os blocos de 6 bases. A implementação inclui:

- Uma função de espalhamento personalizada que converte cada bloco de 6 bases em um inteiro
- Tratamento de colisões através de encadeamento externo ou interno
- Processamento eficiente de arquivos de entrada grandes

## Entrada e Saída

- **Entrada**: Arquivo texto contendo o primeiro genoma completo sequenciado do Coronavirus (sequência NC_045512.2 do NCBI)
- **Saída**: Arquivo texto com duas colunas:
  - Blocos de 6 bases
  - Número de ocorrências de cada bloco

## Como Executar

1. Compile o código usando g++:
   ```
   g++ Main.cpp -o analisador_genoma
   ```
2. Execute o programa:
   ```
   ./analisador_genoma
   ```
3. Para alternar entre diferentes genomas, modifique as seguintes linhas no arquivo Main.cpp:
   ```
   // Para analisar o genoma do COVID-19
   const char* arquivoEntrada = "sequences.fasta.txt";
   const char* arquivoSaida = "resultados_covid.txt";
   
   // Para analisar o genoma do Ebola
   // const char* arquivoEntrada = "sequences.fasta2.txt";
   // const char* arquivoSaida = "resultados_ebola.txt";
   ```
## Referências

- Sequência NC_045512.2 no NCBI: https://www.ncbi.nlm.nih.gov
- Informações sobre funções de espalhamento: https://www.ime.usp.br/~pf/algoritmos/aulas/hash.html
