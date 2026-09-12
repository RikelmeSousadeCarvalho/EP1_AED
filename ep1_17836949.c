/* ============================================================================
    EP1 - Planilha Esparsa com Histórico de Alterações
    TEMPLATE - preencha os TODOs abaixo. Não altere assinaturas de função,
    nomes ou ordem de campos de struct.
 
    Uso: ./ep_XXXX arquivo_entrada.txt arquivo_saida.txt
 ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------
  Estruturas de dados (não altere nomes e ordem de campos)
 ---------------------------------------------------------------------- */

typedef struct celula {
    int linha;
    int coluna;
    int valor;
    struct celula *proxima_linha;
    struct celula *proxima_coluna;
} celula_t;

typedef struct fileira {
    int indice;
    celula_t* primeiro;
    struct fileira *proximo;
} fileira_t;

typedef struct {
    bool transposicao;
    int linha;
    int coluna;
    union {
        int valor_anterior;
        int tamanho;
    };
} operacao_t;

typedef struct elo_pilha {
    operacao_t op;
    struct elo_pilha *proximo;
} elo_pilha_t;

typedef struct {
    elo_pilha_t* topo;
} pilha_t;

typedef struct {
    fileira_t* primeira_linha;
    fileira_t* primeira_coluna;
    int total_celulas;
    pilha_t historico;
} planilha_t;

/* ---------------------------------------------------------------------- *
    Funções auxiliar para leitura das strings dos comandos
 ---------------------------------------------------------------------- */

int igual(char* a, char* b) {
    int i = 0;
    while (a[i] == b[i] && a[i] != '\0') i++;

    return (a[i] == b[i]);
}

/* ---------------------------------------------------------------------- *
    Funções obrigatórias (não altere as assinaturas)
 ---------------------------------------------------------------------- */

 // acho que tenho que revisar
void inicializar_planilha(planilha_t *p) {
    // TODO: inicialize primeiraLinha, primeiraColuna, total_celulas e historico.topo


    //fazendo bem por cima, acho que devo VALIDAR algo antes
    p->primeira_linha = NULL;
    p->primeira_coluna = NULL;
    p->total_celulas = 0;
    p->historico.topo = NULL;
}

celula_t* buscar_celula(planilha_t *p, int lin, int col,
                      celula_t** cel_ant_linha, celula_t** cel_ant_coluna,
                      fileira_t** fil_ant_linha, fileira_t** fil_ant_coluna) {
    /* TODO: localize a celula (lin,col), preencha os quatro antecessores 
        por referencia, retorne NULL se a celula nao existir */

    *cel_ant_linha = NULL;
    *cel_ant_coluna = NULL;
    *fil_ant_linha = NULL;
    *fil_ant_coluna = NULL;
                    
    celula_t* celRetorno = NULL;

    fileira_t* linAtual = p->primeira_linha;
    fileira_t* linAnterior = NULL;
    while(linAtual != NULL && linAtual->indice <= lin){ // enquanto o indice da linha for < de lin, nao achamos a linha que esta sendo buscada

        if(linAtual->indice == lin){ // se o indice da linha atual for igual a lin, chegamos na linha da celula que sera buscada
            celula_t* celAtual = linAtual->primeiro;
            celula_t* celAnterior = NULL;
            while(celAtual != NULL && celAtual->coluna < col){ // enquanto a coluna da celula for < de col, nao achamos a celula que esta sendo buscada
                celAnterior = celAtual;
                celAtual = celAtual->proxima_linha;
            }
            *cel_ant_linha = celAnterior; // quando celula atual coluna == col, passamos o o anterior como referencia
            break; // saimos do while, pois ja achamos a linha que queremos
        }else{ // se for diferente de lin
            linAnterior = linAtual; 
            linAtual = linAtual->proximo;
        }
    }
    *fil_ant_linha = linAnterior;


    fileira_t* colAtual = p->primeira_coluna;
    fileira_t* colAnterior = NULL;
    while(colAtual != NULL && colAtual->indice <= col){ // enquanto o indice da coluna for < de col, nao achamos a coluna que esta sendo buscada

            if(colAtual->indice == col){ // se o indice da coluna atual for igual a col, chegamos na coluna da celula que sera buscada
                celula_t* celAtualCol = colAtual->primeiro;
                celula_t* celAnteriorCol = NULL;
                while(celAtualCol != NULL && celAtualCol->linha < lin){ // enquanto a linha da celula for < de lin, nao achamos a celula que esta sendo buscada
                    celAnteriorCol = celAtualCol;
                    celAtualCol = celAtualCol->proxima_coluna;
                }
                *cel_ant_coluna = celAnteriorCol; // quando celula atual coluna == col, passamos o o anterior como referencia
                celRetorno = celAtualCol; // passamos a celula atual como retorno
                break; // saimos do while, pois ja achamos a coluna que queremos
            }else{ // se for diferente de lin
                colAnterior = colAtual; 
                colAtual = colAtual->proximo;
            }
        }
    *fil_ant_coluna = colAnterior;
    

    if(celRetorno != NULL && celRetorno->linha == lin && celRetorno->coluna == col){ // se a celula atual for diferente de NULL e a linha e coluna forem iguais, retornamos a celula atual
        return celRetorno;
    }else{
        return NULL; // se nao achou a celula, retornamos NULL
    }
}

int obter_valor(planilha_t *p, int linha, int coluna) {
    // TODO: use buscarCelula; retorne 0 se a celula nao existir
    return 0;
}

int somar_intervalo(planilha_t* p, int linha_ini, int linha_fim, int coluna_ini, int coluna_fim) {
    // TODO: some os valores das celulas nao nulas no intervalo dado
    return 0;
}


// acho que ta pronto
int contar_nao_nulas(planilha_t* p) {
    // TODO: retorne a quantidade de celulas nao nulas



    int naoNulas = 0;
    fileira_t* proxLin = p->primeira_linha;  // aponta para a primeira fileira (ou NULL se a planilha estiver vazia)

    while(proxLin != NULL){
        naoNulas++; // adiciona na quantidade de celulas não nulas
        if(proxLin->primeiro != NULL){
            celula_t* proxCel = proxLin->primeiro; // aponta para a primeira célula da fileira
            while(proxCel != NULL){ // se a primeira celula não for nula, pode passar pra proxima celula
                naoNulas++; // adiciona na quantidade de celulas não nulas
                proxCel = proxCel->proxima_coluna; // passa pra proxima celula da mesma linha
            }
        }
        proxLin = proxLin->proximo; // passa pra proxima linha quando as celulas da linha atual acabarem
    }
    return naoNulas;
}

bool definir_celula(planilha_t* p, int lin, int col, int valor) {
    /* TODO: implemente os 4 casos (atualizar/criar/remover/nulo),
       empilhando em p->historico quando houver alteracao efetiva.
       Retorna true se houve alteracao, false se foi operacao nula. */

    /*
    //se a célula existe: 
    if(){
        if(valor != 0){ // se a célula existe e se o valor é diferente de 0:
            
        }else{  // se a célula existe e se o valor é igual a 0: 
            
        }
    }else{ //se a célula não existe: 
        if(valor != 0){ // se a célula não existe e se o valor é diferente de 0:

        }else{  // se a célula existe não e se o valor é igual a 0: 

            // nenhuma operação ocorre (NOP)
        }
    }
    */

    return false;
}

bool remover_celula(planilha_t* p, int lin, int col) {
    // TODO: remova a celula (lin,col)
    return false;
}

bool transpor(planilha_t* p, int lin, int col, int tamanho) {
    /* TODO: transpoe uma matriz quadrada que está localizada entre
       as linhas [lin, lin + tamanho) e colunas [col, col + tamanho). */
    return false;
}

bool desfazer(planilha_t* p) {
    /* TODO: desempilhe de p->historico e restaure o valor anterior.
       Retorna false se o historico estiver vazio, true caso contrario. */
    return false;
}

void exibir_planilha(planilha_t *p) {
    /* TODO: imprima "linha coluna valor" por linha, em ordem crescente
       de linha e, dentro de cada linha, de coluna. Se vazia, imprima
       "PLANILHA VAZIA" */
}

void exibir_historico(planilha_t* p) {
    /* TODO: imprima "linha coluna valor_anterior" por linha, do topo
       para a base. Se vazio, imprima "HISTORICO VAZIO" */
}

void liberar_tudo(planilha_t* p) {
    // TODO: libere toda a memória alocada por fileira, celula, e pilha.
}

/* ---------------------------------------------------------------------- *
    Main para leitura de arquivos (já pronta no caso, pode ser que na versão final deixemos sem)
 ---------------------------------------------------------------------- */

 /*
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso do comando eh: %s arquivo_entrada.txt arquivo_saida.txt\n", argv[0]);
        return 1;
    }
    
    FILE* entrada = fopen(argv[1], "r");
    FILE* saida = freopen(argv[2], "w", stdout);

    if (!entrada || !saida) {
        fprintf(stderr, "Erro ao tentar abrir os arquivos.\n");
        return 1;
    }

    planilha_t p;
    inicializar_planilha(&p);

    int n;
    fscanf(entrada, "%d", &n);

    char cmd[20];

    while (fscanf(entrada, "%s", cmd) != EOF) {
 
        if (igual(cmd, "DEF")) {
            int lin, col, valor;
            fscanf(entrada, "%d %d %d", &lin, &col, &valor);
            definir_celula(&p, lin, col, valor);
        } else if (igual(cmd, "REM")) {
            int lin, col;
            fscanf(entrada, "%d %d", &lin, &col);
            remover_celula(&p, lin, col);
        } else if (igual(cmd, "GET")) {
            int lin, col;
            fscanf(entrada, "%d %d", &lin, &col);
            fprintf(saida, "GET %d %d %d\n", lin, col, obter_valor(&p, lin, col));
        } else if (igual(cmd, "SOMA")) {
            int li, lf, ci, cf;
            fscanf(entrada, "%d %d %d %d", &li, &lf, &ci, &cf);
            fprintf(saida, "SOMA %d %d %d %d %d\n", li, lf, ci, cf, somar_intervalo(&p, li, lf, ci, cf));
        } else if (igual(cmd, "CONT")) {
            fprintf(saida, "CONT %d\n", contar_nao_nulas(&p));
        } else if (igual(cmd, "DESFAZER")) {
            if (!desfazer(&p)) {
                fprintf(saida, "HISTORICO VAZIO\n");
            }
        } else if (igual(cmd, "EXIBIR")) {
            if (contar_nao_nulas(&p)) {
                printf("PLANILHA\n");
                exibir_planilha(&p);
            } else {
                printf("PLANILHA VAZIA\n");
            }
        } else if (igual(cmd, "HIST")) {
            if (p.historico.topo) {
                printf("HISTORICO\n");
                exibir_historico(&p);
            }
            else {
                printf("HISTORICO VAZIO\n");
            }
        } else if (igual(cmd, "TRANS")) {
            int lin, col, tam;
            fscanf(entrada, "%d %d %d", &lin, &col, &tam);
            transpor(&p, lin, col, tam);
        }
    }

    fclose(entrada);
    fclose(saida);

    liberar_tudo(&p);
    return 0;
}
*/



//minha main de testes
int main(){
    printf("Testando\n");
    return 0;
}