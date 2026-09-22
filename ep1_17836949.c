/* ============================================================================
    EP1 - Planilha Esparsa com Histórico de Alterações
    TEMPLATE - preencha os TODOs abaixo. Não altere assinaturas de função,
    nomes ou ordem de campos de struct.
 
    Uso: ./ep_XXXX arquivo_entrada.txt arquivo_saida.txt
 ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h> // usado pra contas de INT_MAX

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

 // acho que ta certo
void inicializar_planilha(planilha_t *p) {
    // TODO: inicialize primeiraLinha, primeiraColuna, total_celulas e historico.topo


    //fazendo bem por cima, acho que devo VALIDAR algo antes
    p->primeira_linha = NULL;
    p->primeira_coluna = NULL;
    p->total_celulas = 0;
    p->historico.topo = NULL;
}

// acho que ta certo
celula_t* buscar_celula(planilha_t *p, int lin, int col,
                      celula_t** cel_ant_linha, celula_t** cel_ant_coluna,
                      fileira_t** fil_ant_linha, fileira_t** fil_ant_coluna) {
    /* TODO: localize a celula (lin,col), preencha os quatro antecessores 
        por referencia, retorne NULL se a celula nao existir */



    // inicializando os ponteiros de celula e fileira como NULL
    if (cel_ant_linha != NULL) {
        *cel_ant_linha = NULL;
    }
    if(cel_ant_coluna != NULL){
        *cel_ant_coluna = NULL;
    }
    if(fil_ant_linha != NULL){
        *fil_ant_linha = NULL;
    }
    if(fil_ant_coluna != NULL){
        *fil_ant_coluna = NULL;
    }
                    
    // inicializando a celula de retorno como NULL
    celula_t* celRetorno = NULL;

    fileira_t* linAtual = p->primeira_linha;
    fileira_t* linAnterior = NULL;
    while(linAtual != NULL && linAtual->indice <= lin){ // enquanto o indice da linha for <= de lin, nao achamos a linha que esta sendo buscada
        if(linAtual->indice == lin){ // se o indice da linha atual for igual a lin, chegamos na linha da celula que sera buscada
            celula_t* celAtual = linAtual->primeiro;
            celula_t* celAnterior = NULL;
            while(celAtual != NULL && celAtual->coluna < col){ // enquanto a coluna da celula for < de col, nao achamos a celula que esta sendo buscada
                celAnterior = celAtual;
                celAtual = celAtual->proxima_linha;
            }
            if(cel_ant_linha != NULL){
                *cel_ant_linha = celAnterior; // quando celula atual coluna == col, passamos o o anterior como referencia
            }
            break; // saimos do while, pois ja achamos a linha que queremos
        }else{ // se for diferente de lin
            linAnterior = linAtual; 
            linAtual = linAtual->proximo;
        }
    }
    if(fil_ant_linha != NULL){
        *fil_ant_linha = linAnterior;
    }


    fileira_t* colAtual = p->primeira_coluna;
    fileira_t* colAnterior = NULL;
    while(colAtual != NULL && colAtual->indice <= col){ // enquanto o indice da coluna for <= de col, nao achamos a coluna que esta sendo buscada
            if(colAtual->indice == col){ // se o indice da coluna atual for igual a col, chegamos na coluna da celula que sera buscada
                celula_t* celAtualCol = colAtual->primeiro;
                celula_t* celAnteriorCol = NULL;
                while(celAtualCol != NULL && celAtualCol->linha < lin){ // enquanto a linha da celula for < de lin, nao achamos a celula que esta sendo buscada
                    celAnteriorCol = celAtualCol;
                    celAtualCol = celAtualCol->proxima_coluna;
                }
                if(cel_ant_coluna != NULL){
                    *cel_ant_coluna = celAnteriorCol; // quando celula atual coluna == col, passamos o o anterior como referencia
                }
                celRetorno = celAtualCol; // passamos a celula atual como retorno
                break; // saimos do while, pois ja achamos a coluna que queremos
            }else{ // se for diferente de lin
                colAnterior = colAtual; 
                colAtual = colAtual->proximo;
            }
        }
    if (fil_ant_coluna != NULL) {
        *fil_ant_coluna = colAnterior;
    }
    

    if(celRetorno != NULL && celRetorno->linha == lin && celRetorno->coluna == col){ // se a celula atual for diferente de NULL e a linha e coluna forem iguais, retornamos a celula atual
        return celRetorno;
    }else{
        return NULL; // se nao achou a celula, retornamos NULL
    }
}

// acho que ta certo
int obter_valor(planilha_t *p, int linha, int coluna) {
    // TODO: use buscarCelula; retorne 0 se a celula nao existir

    celula_t* celula = buscar_celula(p, linha, coluna, NULL, NULL, NULL, NULL);
    if(celula == NULL){
        return 0;
    }else{
        return celula->valor;
    }
}

int somar_intervalo(planilha_t* p, int linha_ini, int linha_fim, int coluna_ini, int coluna_fim) {
    // TODO: some os valores das celulas nao nulas no intervalo dado


    int soma = 0;
    fileira_t* linha = p->primeira_linha;

    while(linha != NULL){
        if(linha->indice >= linha_ini && linha->indice <= linha_fim){ // se a linha atual esta dentro do intervalo das linhas
            celula_t* cel = linha->primeiro;
            while(cel != NULL){
                if(cel->coluna >= coluna_ini && cel->coluna <= coluna_fim){
                    soma += cel->valor;
                }
                cel = cel->proxima_linha;
            }
        }
        linha = linha->proximo;
    }
    return soma;
}


// acho que ta pronto
int contar_nao_nulas(planilha_t* p) {
    // TODO: retorne a quantidade de celulas nao nulas
    return p->total_celulas;
}

//acho que ta pronto
bool definir_celula(planilha_t* p, int lin, int col, int valor) {
    /* TODO: implemente os 4 casos (atualizar/criar/remover/nulo),
       empilhando em p->historico quando houver alteracao efetiva.
       Retorna true se houve alteracao, false se foi operacao nula. */

    
    fileira_t* linAtual = p->primeira_linha; //APAGAR
    fileira_t* colAtual = p->primeira_coluna; //APAGAR


    celula_t* cel_ant_linha = NULL;
    celula_t* cel_ant_coluna = NULL;
    fileira_t* fil_ant_linha = NULL;
    fileira_t* fil_ant_coluna = NULL;

    celula_t* celBusca = buscar_celula(p, lin, col, &cel_ant_linha, &cel_ant_coluna, &fil_ant_linha, &fil_ant_coluna); // celula buscada na posicao lin col



    if(celBusca != NULL){ //se a célula existe: 
        if(valor != 0){ // se a célula existe e se o valor é diferente de 0:

            int valorAnterior = celBusca->valor;

            if(valorAnterior != valor){
                celBusca->valor = valor; // apenas muda o valor dela

                operacao_t novaOp;
                novaOp.transposicao = false;
                novaOp.linha = lin;
                novaOp.coluna = col;
                novaOp.valor_anterior = valorAnterior;


                elo_pilha_t* novoElo = malloc(sizeof(elo_pilha_t));
                novoElo->op = novaOp;
                novoElo->proximo = p->historico.topo;

                p->historico.topo = novoElo;
                return true;
            }
            
            return false;

        }else{  // se a célula existe e se o valor é igual a 0: 

            int valorAnterior = celBusca->valor;

            //equivale a remover uma celula

            //para as linhas
            if(celBusca->proxima_linha == NULL && cel_ant_linha == NULL){ // a celula é a unica da linha

                if(fil_ant_linha == NULL){ // ou a fileira é a primeira ou é a única

                    fileira_t* primeiraLinha = p->primeira_linha;
                    if(p->primeira_linha->proximo == NULL){ // a fileira é unica
                        p->primeira_linha = NULL;
                    }else{ // a fileira é a primeira, mas não é a unica
                        p->primeira_linha = p->primeira_linha->proximo;
                    }

                    free(primeiraLinha);

                }else{ // se a fileira anterior nao é nula
                    fileira_t* filAtual = fil_ant_linha->proximo;
                    if(filAtual->proximo == NULL){ // a fileira atual é a ultima
                        fil_ant_linha->proximo = NULL;
                        free(filAtual);
                    }else{ // a fileira atual esta no meio de duas
                        fil_ant_linha->proximo = filAtual->proximo;
                        free(filAtual);
                    }

                }

            }else{ // a celula nao é unica na linha

                if(cel_ant_linha == NULL){ // celBusca é a primeira celula

                    if(fil_ant_linha == NULL){ // se for a primeira linha
                        p->primeira_linha->primeiro = celBusca->proxima_linha;
                    }else{
                        fil_ant_linha->proximo->primeiro = celBusca->proxima_linha;
                    }
                    celBusca->proxima_linha = NULL; //acho que nem preciso fazer isso 

                }else if(cel_ant_linha != NULL && celBusca->proxima_linha != NULL){ // ela ta no meio
                    cel_ant_linha->proxima_linha = celBusca->proxima_linha;
                    celBusca->proxima_linha = NULL; //acho que nem preciso fazer isso 
                }else{ // ela é a ultima   
                    cel_ant_linha->proxima_linha = NULL; // a anterior vira a nova ultima, apontando pra NULL
                }

            }



            //para as colunas
            if(celBusca->proxima_coluna == NULL && cel_ant_coluna == NULL){ // a celula é a unica da coluna

                if(fil_ant_coluna == NULL){ // ou a fileira é a primeira ou é a única

                    fileira_t* primeiraColuna = p->primeira_coluna;
                    if(p->primeira_coluna->proximo == NULL){ // a fileira é unica
                        p->primeira_coluna = NULL;
                    }else{ // a fileira é a primeira, mas não é a unica
                        p->primeira_coluna = p->primeira_coluna->proximo;
                    }

                    free(primeiraColuna);

                }else{ // se a fileira anterior nao é nula
                    fileira_t* filAtual = fil_ant_coluna->proximo;
                    if(filAtual->proximo == NULL){ // a fileira atual é a ultima
                        fil_ant_coluna->proximo = NULL;
                        free(filAtual);
                    }else{ // a fileira atual esta no meio de duas
                        fil_ant_coluna->proximo = filAtual->proximo;
                        free(filAtual);
                    }

                }

            }else{ // a celula nao é unica na coluna

                if(cel_ant_coluna == NULL){ // celBusca é a primeira celula

                    if(fil_ant_coluna == NULL){ // se for a primeira coluna
                        p->primeira_coluna->primeiro = celBusca->proxima_coluna;
                    }else{
                        fil_ant_coluna->proximo->primeiro = celBusca->proxima_coluna;
                    }
                    celBusca->proxima_coluna = NULL; //acho que nem preciso fazer isso 

                }else if(cel_ant_coluna != NULL && celBusca->proxima_coluna != NULL){ // ela ta no meio
                    cel_ant_coluna->proxima_coluna = celBusca->proxima_coluna;
                    celBusca->proxima_coluna = NULL; //acho que nem preciso fazer isso 
                }else{ // ela é a ultima   
                    cel_ant_coluna->proxima_coluna = NULL; // a anterior vira a nova ultima, apontando pra NULL
                }

            }


            operacao_t novaOp;
            novaOp.transposicao = false;
            novaOp.linha = lin;
            novaOp.coluna = col;
            novaOp.valor_anterior = valorAnterior;


            elo_pilha_t* novoElo = malloc(sizeof(elo_pilha_t));
            novoElo->op = novaOp;
            novoElo->proximo = p->historico.topo;

            p->historico.topo = novoElo;


            p->total_celulas = p->total_celulas - 1;
            free(celBusca); // depois de tudo, limpo a celula
            return true;

        }
    }else{ //se a célula não existe: 
        if(valor != 0){ // se a célula não existe e se o valor é diferente de 0:

            celula_t* novaCel = malloc(sizeof(celula_t));
            novaCel->valor = valor;


            //para as linhas

            if(fil_ant_linha == NULL){ // se a fileira nn tem antecessor
                //pode ser a primeira, pode ser a unica ou a planilha pode estar vazia

                if(p->total_celulas == 0){ // planilha vazia
                    fileira_t* novaLinha = malloc(sizeof(fileira_t));
                    p->primeira_linha = novaLinha;
                    novaLinha->primeiro = novaCel;
                    novaLinha->indice = lin;
                    novaLinha->proximo = NULL;
                    novaCel->linha = lin;
                    novaCel->coluna = col;
                    novaCel->proxima_linha = NULL;
                }else{// pode ser a primeira, pode ser a unica - vai funcionar da mesma forma

                    fileira_t* primeiraLinha = p->primeira_linha;

                    if(cel_ant_linha != NULL){ // se a fileira existir e preciso colocar uma celula no meio ou final
                        novaCel->proxima_linha = cel_ant_linha->proxima_linha;
                        cel_ant_linha->proxima_linha = novaCel;

                    }else{ // se a fileira da linha existir e precisar colocar na primeira posição 
                        if(p->primeira_linha->indice == lin){ // se o indice da primeira linha for igual a lin
                            novaCel->proxima_linha = primeiraLinha->primeiro;
                            primeiraLinha->primeiro = novaCel;
                        }else{
                            fileira_t* novaLinha = malloc(sizeof(fileira_t));
                            novaCel->proxima_linha = NULL;
                            novaLinha->proximo = p->primeira_linha;
                            p->primeira_linha = novaLinha;
                            novaLinha->primeiro = novaCel;
                            novaLinha->indice = lin;
                            novaCel->linha = lin;
                            novaCel->coluna = col;
                        }
                    }

                }

            }else{ // se a fileira tem antecessor

                if(fil_ant_linha->proximo != NULL && fil_ant_linha->proximo->indice == lin){ // se o indice de linha passado é igual ao indice da fileira seguinte ao antecessor(a atual), a linha existe. O != NULL é pra garantir que existe fil_ant_linha->proximo

                    if(cel_ant_linha != NULL){ // se a fileira existir e preciso colocar uma celula no meio ou no fim
                        novaCel->proxima_linha = cel_ant_linha->proxima_linha;
                        cel_ant_linha->proxima_linha = novaCel;

                    }else{ // se a fileira da linha existir e precisar colocar na primeira posição (ou seja, nao tem anterior)
                        novaCel->proxima_linha = fil_ant_linha->proximo->primeiro; // fileira atual
                        fil_ant_linha->proximo->primeiro = novaCel;

                    }

                }else{ // nao existe e precisamos criar a fileira 
                    fileira_t* novaLinha = malloc(sizeof(fileira_t));
                    novaLinha->proximo = fil_ant_linha->proximo;
                    novaLinha->primeiro = novaCel;
                    novaLinha->indice = lin;
                    fil_ant_linha->proximo = novaLinha;
                    novaCel->linha = lin;
                    novaCel->coluna = col;
                    novaCel->proxima_linha = NULL;
                }
            }


            //parte das colunas

            if(fil_ant_coluna == NULL){ // se a fileira nn tem antecessor
                //pode ser a primeira, pode ser a unica ou a planilha pode estar vazia

                if(p->total_celulas == 0){ // planilha vazia
                    fileira_t* coluna = malloc(sizeof(fileira_t));
                    p->primeira_coluna = coluna;
                    coluna->primeiro = novaCel;
                    coluna->indice = col;
                    coluna->proximo = NULL;
                    novaCel->linha = lin;
                    novaCel->coluna = col;
                    novaCel->proxima_coluna = NULL;
                }else{// pode ser a primeira, pode ser a unica - vai funcionar da mesma forma

                    fileira_t* primeiraColuna = p->primeira_coluna;

                    if(cel_ant_coluna != NULL){ // se a fileira existir e preciso colocar uma celula no meio ou final
                        novaCel->proxima_coluna = cel_ant_coluna->proxima_coluna;
                        cel_ant_coluna->proxima_coluna = novaCel;

                    }else{ // se a fileira da coluna existir e precisar colocar na primeira posição 
                        if(p->primeira_coluna->indice == col){
                            novaCel->proxima_coluna = primeiraColuna->primeiro;
                            primeiraColuna->primeiro = novaCel;
                        }else{
                            fileira_t* novaColuna = malloc(sizeof(fileira_t));
                            novaCel->proxima_coluna = NULL;
                            novaColuna->proximo = p->primeira_coluna;
                            p->primeira_coluna = novaColuna;
                            novaColuna->primeiro = novaCel;
                            novaColuna->indice = col;
                            novaCel->linha = lin;
                            novaCel->coluna = col;
                        }
                    }

                }

            }else{ // se a fileira tem antecessor

                if(fil_ant_coluna->proximo != NULL && fil_ant_coluna->proximo->indice == col){ // se o indice de coluna passado é igual ao indice da fileira seguinte ao antecessor(a atual), a coluna existe. O != NULL é pra garantir que existe fil_ant_coluna->proximo

                    if(cel_ant_coluna != NULL){ // se a fileira existir e preciso colocar uma celula no meio ou no fim
                        novaCel->proxima_coluna = cel_ant_coluna->proxima_coluna;
                        cel_ant_coluna->proxima_coluna = novaCel;

                    }else{ // se a fileira da linha existir e precisar colocar na primeira posição (ou seja, nao tem anterior)
                        novaCel->proxima_coluna = fil_ant_coluna->proximo->primeiro; // fileira atual
                        fil_ant_coluna->proximo->primeiro = novaCel;

                    }

                }else{ // nao existe e precisamos criar a fileira 
                    fileira_t* novaColuna = malloc(sizeof(fileira_t));
                    novaColuna->proximo = fil_ant_coluna->proximo;
                    novaColuna->primeiro = novaCel;
                    novaColuna->indice = col;
                    fil_ant_coluna->proximo = novaColuna;
                    novaCel->linha = lin;
                    novaCel->coluna = col;
                    novaCel->proxima_coluna = NULL;
                }
            }

            operacao_t novaOp;
            novaOp.transposicao = false;
            novaOp.linha = lin;
            novaOp.coluna = col;
            novaOp.valor_anterior = 0; // nao existe valor anterior, entao colocamos 0


            elo_pilha_t* novoElo = malloc(sizeof(elo_pilha_t));
            novoElo->op = novaOp;
            novoElo->proximo = p->historico.topo;

            p->historico.topo = novoElo;

            p->total_celulas = p->total_celulas + 1; // criamos uma celula, entao aumentamos o total_celulas
            
            return true;

        }else{  // se a célula existe não e se o valor é igual a 0: 
            // nenhuma operação ocorre (NOP)
            return false;
        }
    }

}

//acho que ta certo
bool remover_celula(planilha_t* p, int lin, int col) {
    // TODO: remova a celula (lin,col)

    if(definir_celula(p, lin, col, 0)){
        return true;
    }

    return false;
}

bool transpor(planilha_t* p, int lin, int col, int tamanho) {
    /* TODO: transpoe uma matriz quadrada que está localizada entre
       as linhas [lin, lin + tamanho) e colunas [col, col + tamanho). */


       //O que falta: ajustar a logica para usar o definir celula - ele vai empilhar todas as mudanças, ai tenho que desempilhar e empilhar uma só manualmente

    if(lin > INT_MAX - tamanho || col > INT_MAX - tamanho){ // a soma dos dois ints estourou o limite(INT_MAX) 
        return false;
    }

    int alteracao = 0;
    for(int l = 0; l < tamanho; l++){
        for(int c = 0; c < tamanho; c++){
            if(l < c){
                int orig = obter_valor(p, lin + l, col + c);
                int transp = obter_valor(p, lin + c, col + l);
                if(orig != transp){
                    alteracao++;
                    int temp = orig;
                    orig = transp;
                    transp = temp;
                }
            }
        }
    }
    
    if(alteracao > 0){
        operacao_t novaOp;
        novaOp.transposicao = true;
        novaOp.linha = lin;
        novaOp.coluna = col;
        novaOp.tamanho = tamanho;

        elo_pilha_t* novoElo = malloc(sizeof(elo_pilha_t));
        novoElo->op = novaOp;
        novoElo->proximo = p->historico.topo;
        p->historico.topo = novoElo;

        return true;
    }

    return false;
}

bool desfazer(planilha_t* p) {
    /* TODO: desempilhe de p->historico e restaure o valor anterior.
       Retorna false se o historico estiver vazio, true caso contrario. */
    return false;
}


//acho que ta pronto
void exibir_planilha(planilha_t *p) {
    /* TODO: imprima "linha coluna valor" por linha, em ordem crescente
       de linha e, dentro de cada linha, de coluna. Se vazia, imprima
       "PLANILHA VAZIA" */


        int naoNulas = contar_nao_nulas(p);
        if(naoNulas == 0){
            printf("PLANILHA VAZIA\n");
        }else{
            fileira_t* linAtual = p->primeira_linha;

            while(linAtual != NULL){
                celula_t* proxCel = linAtual->primeiro;
                while(proxCel != NULL){
                    printf("%d %d %d\n", linAtual->indice, proxCel->coluna, proxCel->valor);
                    proxCel = proxCel->proxima_linha;
                }
                linAtual = linAtual->proximo;
            }
        }
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










#pragma region testes



/* ----------------------------------------------------------------------
   Funcoes auxiliares SO para montar a planilha de teste manualmente
   (nao fazem parte do EP - servem so para o harness poder testar
   buscar_celula/obter_valor/contar_nao_nulas sem depender de definir_celula,
   que ainda nao esta pronta)
 ---------------------------------------------------------------------- */
 
celula_t* nova_celula(int lin, int col, int valor) {
    celula_t* c = malloc(sizeof(celula_t));
    c->linha = lin;
    c->coluna = col;
    c->valor = valor;
    c->proxima_linha = NULL;
    c->proxima_coluna = NULL;
    return c;
}
 
fileira_t* nova_fileira(int indice, celula_t* primeiro) {
    fileira_t* f = malloc(sizeof(fileira_t));
    f->indice = indice;
    f->primeiro = primeiro;
    f->proximo = NULL;
    return f;
}
 
/* Monta a planilha do exemplo do enunciado (Figura 1, Secao 3.7):
   (0,1)=12, (2,0)=5, (1,1)=10, (1,3)=7 */
void montar_planilha_exemplo(planilha_t* p) {
    inicializar_planilha(p);
    p->total_celulas = 4;
 
    celula_t* c01 = nova_celula(0, 1, 12);
    celula_t* c10 = nova_celula(1, 1, 10);
    celula_t* c13 = nova_celula(1, 3, 7);
    celula_t* c20 = nova_celula(2, 0, 5);
 
    // Listas de LINHA (ordenadas por coluna crescente, ligadas por proxima_linha)
    c01->proxima_linha = NULL;          // linha 0: so c01
    c10->proxima_linha = c13;           // linha 1: c10 (col1) -> c13 (col3)
    c13->proxima_linha = NULL;
    c20->proxima_linha = NULL;          // linha 2: so c20
 
    // Listas de COLUNA (ordenadas por linha crescente, ligadas por proxima_coluna)
    c20->proxima_coluna = NULL;         // coluna 0: so c20
    c01->proxima_coluna = c10;          // coluna 1: c01 (lin0) -> c10 (lin1)
    c10->proxima_coluna = NULL;
    c13->proxima_coluna = NULL;         // coluna 3: so c13
 
    // Fileiras de LINHA
    fileira_t* f_lin0 = nova_fileira(0, c01);
    fileira_t* f_lin1 = nova_fileira(1, c10);
    fileira_t* f_lin2 = nova_fileira(2, c20);
    f_lin0->proximo = f_lin1;
    f_lin1->proximo = f_lin2;
    f_lin2->proximo = NULL;
    p->primeira_linha = f_lin0;
 
    // Fileiras de COLUNA
    fileira_t* f_col0 = nova_fileira(0, c20);
    fileira_t* f_col1 = nova_fileira(1, c01);
    fileira_t* f_col3 = nova_fileira(3, c13);
    f_col0->proximo = f_col1;
    f_col1->proximo = f_col3;
    f_col3->proximo = NULL;
    p->primeira_coluna = f_col0;
}
 
/* ----------------------------------------------------------------------
   Testes
 ---------------------------------------------------------------------- */
 
int testes_ok = 0;
int testes_total = 0;
 
void checar(const char* nome, int esperado, int obtido) {
    testes_total++;
    if (esperado == obtido) {
        testes_ok++;
        printf("[OK]     %s (esperado=%d, obtido=%d)\n", nome, esperado, obtido);
    } else {
        printf("[FALHOU] %s (esperado=%d, obtido=%d)\n", nome, esperado, obtido);
    }
}
 
void checar_ptr(const char* nome, void* esperado, void* obtido) {
    testes_total++;
    if (esperado == obtido) {
        testes_ok++;
        printf("[OK]     %s (ponteiros batem)\n", nome);
    } else {
        printf("[FALHOU] %s (esperado=%p, obtido=%p)\n", nome, esperado, obtido);
    }
}
 
int main() {
    planilha_t p;
    montar_planilha_exemplo(&p);
 
    printf("=== Testando obter_valor ===\n");
    checar("obter_valor(0,1) deve ser 12", 12, obter_valor(&p, 0, 1));
    checar("obter_valor(1,1) deve ser 10", 10, obter_valor(&p, 1, 1));
    checar("obter_valor(1,3) deve ser 7",  7, obter_valor(&p, 1, 3));
    checar("obter_valor(2,0) deve ser 5",  5, obter_valor(&p, 2, 0));
    checar("obter_valor(9,9) [nao existe] deve ser 0", 0, obter_valor(&p, 9, 9));
    checar("obter_valor(1,2) [buraco no meio] deve ser 0", 0, obter_valor(&p, 1, 2));
    checar("obter_valor(0,0) [linha existe, celula nao] deve ser 0", 0, obter_valor(&p, 0, 0));
 
    printf("\n=== Testando contar_nao_nulas ===\n");
    checar("contar_nao_nulas deve ser 4", 4, contar_nao_nulas(&p));
 
    printf("\n=== Testando buscar_celula (existente) ===\n");
    {
        celula_t *cel_ant_lin, *cel_ant_col;
        fileira_t *fil_ant_lin, *fil_ant_col;
        celula_t* r = buscar_celula(&p, 1, 3, &cel_ant_lin, &cel_ant_col, &fil_ant_lin, &fil_ant_col);
        checar("buscar_celula(1,3) deve retornar nao-NULL", 1, (r != NULL));
        if (r) {
            checar("celula (1,3) tem valor 7", 7, r->valor);
        }
        checar("cel_ant_linha(1,3) deve ter valor 10", 10, (cel_ant_lin ? cel_ant_lin->valor : -1));
        checar("fil_ant_linha(1,3) deve ter indice 0", 0, (fil_ant_lin ? fil_ant_lin->indice : -1));
        checar("fil_ant_coluna(1,3) deve ter indice 1", 1, (fil_ant_col ? fil_ant_col->indice : -1));
        checar_ptr("cel_ant_coluna(1,3) deve ser NULL", NULL, cel_ant_col);
    }
 
    printf("\n=== Testando buscar_celula (linha existe, celula nao) ===\n");
    {
        celula_t *cel_ant_lin, *cel_ant_col;
        fileira_t *fil_ant_lin, *fil_ant_col;
        celula_t* r = buscar_celula(&p, 1, 2, &cel_ant_lin, &cel_ant_col, &fil_ant_lin, &fil_ant_col);
        checar_ptr("buscar_celula(1,2) deve retornar NULL", NULL, r);
        checar("cel_ant_linha(1,2) deve ter valor 10 (celula col=1)", 10, (cel_ant_lin ? cel_ant_lin->valor : -1));
    }
 
    printf("\n=== Testando buscar_celula (linha nao existe) ===\n");
    {
        celula_t *cel_ant_lin, *cel_ant_col;
        fileira_t *fil_ant_lin, *fil_ant_col;
        celula_t* r = buscar_celula(&p, 5, 5, &cel_ant_lin, &cel_ant_col, &fil_ant_lin, &fil_ant_col);
        checar_ptr("buscar_celula(5,5) deve retornar NULL", NULL, r);
        checar("fil_ant_linha(5,5) deve ter indice 2", 2, (fil_ant_lin ? fil_ant_lin->indice : -1));
        checar("fil_ant_coluna(5,5) deve ter indice 3", 3, (fil_ant_col ? fil_ant_col->indice : -1));
    }
 
    printf("\n=== Resultado final: %d/%d testes passaram ===\n", testes_ok, testes_total);
 
    return 0;
}
 
