#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Constantes
const int TAMANHO_BLOCO = 6;
const int TAMANHO_LINHA = 60;
const int MAX_BLOCOS = 1000000; // Estimativa de número máximo de blocos únicos possíveis

// Estrutura para armazenar um bloco de sequência
struct BlocoSequencia {
    char sequencia[TAMANHO_BLOCO + 1]; // +1 para o caractere nulo no final
    int contagem;
    int proximo; // Índice do próximo elemento (para resolução de colisões)
};

class AnalisadorGenoma {
private:
    // Array de blocos de sequência
    BlocoSequencia* blocos;
    
    // Array para a tabela hash
    int* tabelaHash;
    
    // Tamanho da tabela hash
    int tamanhoDaTabela;
    
    // Contador para o próximo índice disponível no array de blocos
    int proximoIndiceLivre;
    
    // Função hash personalizada
    unsigned int calcularHash(const char* sequencia) {
        unsigned int hash = 0;
        int i = 0;
        
        while (i < TAMANHO_BLOCO) {
            // Rotação para esquerda de 5 bits e XOR com o caractere atual
            hash = ((hash << 5) | (hash >> 27)) ^ sequencia[i];
            i++;
        }
        
        return hash % tamanhoDaTabela;
    }
    
    // Compara duas strings de comprimento fixo (TAMANHO_BLOCO)
    bool compararSequencias(const char* seq1, const char* seq2) {
        for (int i = 0; i < TAMANHO_BLOCO; i++) {
            if (seq1[i] != seq2[i]) return false;
        }
        return true;
    }
    
    // Copia uma sequência para outra (de comprimento fixo)
    void copiarSequencia(char* destino, const char* origem) {
        for (int i = 0; i < TAMANHO_BLOCO; i++) {
            destino[i] = origem[i];
        }
        destino[TAMANHO_BLOCO] = '\0';
    }
    
public:
    AnalisadorGenoma(int tamanhoTabela = 100003) {
        tamanhoDaTabela = tamanhoTabela;
        
        // Inicializa o array de blocos de sequência
        blocos = new BlocoSequencia[MAX_BLOCOS];
        
        // Inicializa a tabela hash com -1 (indicando posição vazia)
        tabelaHash = new int[tamanhoDaTabela];
        for (int i = 0; i < tamanhoDaTabela; i++) {
            tabelaHash[i] = -1;
        }
        
        proximoIndiceLivre = 0;
    }
    
    ~AnalisadorGenoma() {
        delete[] blocos;
        delete[] tabelaHash;
    }
    
    // Adiciona um bloco à tabela ou incrementa sua contagem se já existir
    void registrarBloco(const char* sequencia) {
        unsigned int indiceHash = calcularHash(sequencia);
        
        // Se a posição estiver vazia
        if (tabelaHash[indiceHash] == -1) {
            // Adiciona novo bloco
            copiarSequencia(blocos[proximoIndiceLivre].sequencia, sequencia);
            blocos[proximoIndiceLivre].contagem = 1;
            blocos[proximoIndiceLivre].proximo = -1;
            
            // Atualiza a tabela hash
            tabelaHash[indiceHash] = proximoIndiceLivre;
            
            // Avança para a próxima posição livre
            proximoIndiceLivre++;
            return;
        }
        
        // Procura na lista encadeada
        int indiceAtual = tabelaHash[indiceHash];
        
        while (indiceAtual != -1) {
            // Verifica se é o mesmo bloco
            if (compararSequencias(blocos[indiceAtual].sequencia, sequencia)) {
                blocos[indiceAtual].contagem++;
                return;
            }
            
            // Se chegou ao fim da lista, adiciona um novo elemento
            if (blocos[indiceAtual].proximo == -1) {
                // Cria um novo bloco
                copiarSequencia(blocos[proximoIndiceLivre].sequencia, sequencia);
                blocos[proximoIndiceLivre].contagem = 1;
                blocos[proximoIndiceLivre].proximo = -1;
                
                // Atualiza o ponteiro do último elemento
                blocos[indiceAtual].proximo = proximoIndiceLivre;
                
                // Avança para a próxima posição livre
                proximoIndiceLivre++;
                return;
            }
            
            indiceAtual = blocos[indiceAtual].proximo;
        }
    }
    
    // Salva os resultados em um arquivo
    void exportarResultados(const char* nomeArquivo) {
        ofstream arquivo(nomeArquivo);
        
        if (!arquivo.is_open()) {
            cerr << "Erro ao abrir arquivo de saída!" << endl;
            return;
        }
        
        // Escreve cabeçalho
        arquivo << "Sequencia Ocorrencias\n";
        
        // Percorre todos os blocos registrados
        for (int i = 0; i < proximoIndiceLivre; i++) {
            arquivo << blocos[i].sequencia << " " << blocos[i].contagem << "\n";
        }
        
        arquivo.close();
    }
};

// Função para processar o arquivo do genoma
void processarArquivoGenoma(const char* arquivoEntrada, const char* arquivoSaida) {
    ifstream entrada(arquivoEntrada);
    
    if (!entrada.is_open()) {
        cerr << "Erro ao abrir arquivo de entrada!" << endl;
        return;
    }
    
    AnalisadorGenoma analisador;
    char linha[TAMANHO_LINHA + 1]; // +1 para o caractere nulo
    char bloco[TAMANHO_BLOCO + 1]; // +1 para o caractere nulo
    
    // Processa o arquivo linha a linha
    while (entrada.getline(linha, TAMANHO_LINHA + 1)) {
        int comprimento = 0;
        while (linha[comprimento] != '\0' && comprimento < TAMANHO_LINHA) {
            comprimento++;
        }
        
        // Ignora linhas curtas demais
        if (comprimento < TAMANHO_LINHA) {
            continue;
        }
        
        // Processa cada bloco na linha
        for (int i = 0; i <= TAMANHO_LINHA - TAMANHO_BLOCO; i += TAMANHO_BLOCO) {
            // Copia o bloco
            for (int j = 0; j < TAMANHO_BLOCO; j++) {
                bloco[j] = linha[i + j];
            }
            bloco[TAMANHO_BLOCO] = '\0';
            
            // Registra o bloco
            analisador.registrarBloco(bloco);
        }
    }
    
    entrada.close();
    
    // Salva os resultados
    analisador.exportarResultados(arquivoSaida);
}

int main() {
    const char* arquivoEntrada = "sequences.fasta.txt"; // sequences.fasta.txt para o genoma da covid  e sequences.fasta2.txt para o genoma do ebola
    const char* arquivoSaida = "resultados_analise.txt"; // resultados_analise.txt para o genoma da covid e resultados_analise2.txt para o genoma do ebola
    
    processarArquivoGenoma(arquivoEntrada, arquivoSaida);
    cout << "Análise concluída! Resultados salvos em: " << arquivoSaida << endl;
    
    return 0;
}