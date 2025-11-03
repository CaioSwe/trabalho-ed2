#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qry.h"
#include "geo.h"

// MUDAR TAMANHO DA TABELA HASH DOS COMANDOS DE BLOQUEIO PARA ADAPTABILIDADE.

// Estrutura de ponto da cidade que contem No', face e numero.
typedef struct Ponto{
    Node node;
    char face;
    int num;
}Ponto;

// Estrutura de percurso com as informacoes do percurso associado.
typedef struct Percurso{
    Ponto* origem;
    Ponto* destino;

    char cmc[256];
    char cmr[256];
}Percurso;

// Estrutura a ser armazenada na tabela Hash para os comandos [blq, rbl].
typedef struct BloqueioStr{
    Lista lista;
    char* bloqueio;
}BloqueioStr;

// Desabilitar as todas as arestas que (estao dentro / cruzam) a regiao especificada.
static void removerQuadras(Item item, void* extra){
    // desabillitarArestas(item);
    // ou alguma coisa assim
}

// Desabilita todas as arestas passadas para esta funcao.
static void bloquearSentido(Item item, void* extra){
    char* sentido = (char*)extra;
    Node node = (Node)item;



    // blockAresta(item, sentido);
    // ou alguma coisa assim
}

// Habilita todas as arestas passadas para esta funcao.
static void desbloquearSentido(Item item, void* extra){
    char* sentido = (char*)extra;



    // desbloquearArestas(item, sentido);
    // ou alguma coisa assim
}

Lista processQryFile(Graph grafo, Quadras quadras, STreap arvore, const char* path){
    // Checa se o caminho dado contam a extensao .qry
    if(strstr(path, ".qry") == NULL){
        printf("\n- processQryFile() -> path: \"%s\" nao a um arquivo .qry -", path);
        return NULL;
    }

    // Abre o arquivo de entrada no modo de leitura
    FILE* fEntrada = fopen(path, "r");
    printf("\nLendo arquivo: %s\n", path);

    // Variaveis utilizadas na leitura do QRY:
    // Operacao.
    char op[256];
    
    // Cep, face e numero.
    char cep[256];
    char face;
    int num;
    
    // Quadrado [x, y, width, height].
    double x, y, w, h;

    // Cor de preenchimento e cor de borda.
    char cfill[256];
    char cstrk[256];

    // Nome e sentido.
    char nome[256];
    char sentido[2];

    // Fator escalar.
    double fator;

    // Cores do caminho mais curto e caminho mais ra'pido.
    char cmc[256];
    char cmr[256];

    // Cria uma tabela Hash para a associacao de nome-lista para os comandos de bloqueio de remocao de bloqueio.
    Hash tabelaHash = criaHash(256, true);

    Percurso* percurso = (Percurso*)malloc(sizeof(Percurso));

    percurso->origem = NULL;
    percurso->destino = NULL;

    // Itera sobre as linhas do arquivo .qry
    while(fscanf(fEntrada, "%s", op) > 0){
        if(strcmp(op, "@o?") == 0){
            // Pega [cep, face, num] da operacao de origem.
            fscanf(fEntrada, "%s %s %d\n", cep, face, &num);

            // Pega o node com o cep registrado no grafo.
            Node node = getNode(grafo, cep);

            percurso->origem = (Ponto*)malloc(sizeof(Ponto));

            // Registra as informacoes do node encontrado na origem do percurso.
            percurso->origem->node = node;
            percurso->origem->face = face;
            percurso->origem->num = num;
        }
        else if(strcmp(op, "catac") == 0){
            // Pega [x, y, w, h] da operacao de cataclisma.
            fscanf(fEntrada, "%lf %lf %lf %lf\n", &x, &y, &w, &h);

            Lista lista = criaLista();

            // Pega a lista das quadras dentro da regiao [x, y, w, h].
            getNodeRegiaoSTrp(arvore, x, y, w, h, lista);
            // Percorre a lista das quadras da regiao, desabilitando-as.
            percorrerLista(lista, removerQuadras, NULL);

            limparLista(lista, false);
        }
        else if(strcmp(op, "pnt") == 0){
            // Pega [cep, cfill, cstrk] da operacao de paint.
            fscanf(fEntrada, "%s %s %s\n", cep, cfill, cstrk);

            // Pega a quadra associada ao cep
            Quadra quadra = getQuadraByID(quadras, cep);

            // Muda as propriedades da quadra
            setQuadraCFill(quadra, cfill);
            setQuadraCStrk(quadra, cstrk);
        }
        else if(strcmp(op, "blq") == 0){
            // Pega [nome, sentido, x, y, w, h] da operacao de bloqueio.
            fscanf(fEntrada, "%s %s %lf %lf %lf %lf\n", nome, sentido, &x, &y, &w, &h);
        
            Lista lista = criaLista();

            // Pega as quadras dentro da regiao [x, y, w, h].
            getNodeRegiaoSTrp(arvore, x, y, w, h, lista);

            // Percorre a lista das quadras encontradas, bloqueando-as no sentido fornecido.
            percorrerLista(lista, bloquearSentido, sentido);

            // Inicializa a estrutura de bloqueios a ser armazenada na tabela Hash.
            BloqueioStr* listaBlq = (BloqueioStr*)malloc(sizeof(BloqueioStr));

            listaBlq->bloqueio = (char*)malloc(sizeof(char) * strlen(sentido));
            strcpy(listaBlq->bloqueio, sentido);
            
            listaBlq->lista = lista;

            // Insere a lista das quadras na tabela Hash para busca do comando [rbl].
            inserirHash(tabelaHash, nome, listaBlq);
        }
        else if(strcmp(op, "rbl") == 0){
            // Pega [nome] da operacao de remocao de bloqueio.
            fscanf(fEntrada, "%s\n", nome);
        
            // Busca a lista associada ao nome na tabela Hash.
            Lista lista = (Lista)getHashValue(tabelaHash, nome);

            percorrerLista(lista, desbloquearSentido, NULL);
        }
        else if(strcmp(op, "b") == 0){
            // Pega [x, y, fator] da operacao de boost(?)
            fscanf(fEntrada, "%lf %lf %s\n", &x, &y, fator);
        
            // Pega o node mais praximo das coordenadas.
            // getClosestNode(graph, x, y);

            // Comeca o percurso em largura do node.
            // bfs(g, node, discoverNode, extra)
        }
        else if(strcmp(op, "p?") == 0){
            // Pega [cep, face, num, cmc, cmr] da operacao de percurso.
            fscanf(fEntrada, "%s %c %d %s %s\n", cep, &face, &num, cmc, cmr);

            if(percurso->origem == NULL){
                printf("\n- processQryFile() -> ponto de origem nao definido antes da operacao \"(p?)\". -");
                continue;
            }

            percurso->destino = (Ponto*)malloc(sizeof(Ponto));

            percurso->destino->node = getNode(grafo, cep);
            percurso->destino->face = face;
            percurso->destino->num = num;

            strcpy(percurso->cmc, cmc);
            strcpy(percurso->cmr, cmr);

            // continuar (dijkstra)
        }
    }

    printf("\n");

    // Fecha o arquivo de entrada
    fclose(fEntrada);
    
    return criaLista();
}