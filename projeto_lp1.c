#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"

#define MAX 128



/*--------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------FUNÇÕES------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

/*-----------------------FUNÇÃO QUE CALCULA O NÚMERO DE CASAS QUE O TABULEIRO VAI TER, DEPENDENDO DAS LINHAS E COLUNAS*/
int calcular_numero_de_casas(int linhas, int colunas){
    return 2 * (colunas + linhas - 2);
}


/*--------------------------------------------------------------------------------TRADUÇÃO UM POUCO SCUFFED MAS PRONTO*/
int char_para_int(char peca_para_andar){
    if (peca_para_andar == 'a' || peca_para_andar == 'w')
    {
        return 0;
    } else if (peca_para_andar == 'b' || peca_para_andar == 'x')
    {
        return 1;
    } else if (peca_para_andar == 'c' || peca_para_andar == 'y')
    {
        return 2;
    } else
    {
        return 3;
    }
}


/*-----------------------------------------------------------------------------------------FUNÇÃO QUE TIREI DO board.c*/
casa * encontrar_a_casa(list theBoard, int idx)
{
	/* ponteiro para um node da lista */
	node *t;
	
	/* iterador do ciclo for */
	int i;

	for (t = theBoard.head, i = 0 ; t != NULL && i < idx ; t = t-> next, i++)
		;

	return t != NULL ? &(t -> item) : NULL;
}


/*---------------------------------------------------------------------------------------FUNÇÃO PARA CRIAR O TABULEIRO*/
void funcao_do_tabuleiro(list * tabuleiro, int numero_de_casas){
    int i;
    node * casa_anterior;
    node * node_aux; 

    /*---------------------------------------------------------PRIMEIRA CASA DO TABULEIRO (CASA DE PARTIDA DO JOGADOR1*/
    node_aux = (node * )malloc(sizeof(node));
    tabuleiro -> head = node_aux;
    tabuleiro -> length++;

    node_aux -> next = NULL;
    node_aux -> item.casaSegura = TRUE;
    node_aux -> item.jogador_peao[JOGADOR1][0] = TRUE;
    node_aux -> item.jogador_peao[JOGADOR1][1] = TRUE;
    node_aux -> item.jogador_peao[JOGADOR1][2] = TRUE;
    node_aux -> item.jogador_peao[JOGADOR1][3] = TRUE;

    node_aux -> item.jogador_peao[JOGADOR2][0] = FALSE;
    node_aux -> item.jogador_peao[JOGADOR2][1] = FALSE;
    node_aux -> item.jogador_peao[JOGADOR2][2] = FALSE;
    node_aux -> item.jogador_peao[JOGADOR2][3] = FALSE;

    casa_anterior = node_aux;

    /*-----------------------------------------------------------------------CASAS RESTANTES ATÉ AO FINAL DO TABULEIRO*/
    for (i = 0; i < numero_de_casas - 1; i++)
    {
        node_aux = (node * )malloc(sizeof(node));
        node_aux -> next = NULL;

        if (i == numero_de_casas / 2 - 1)
        {
            node_aux -> item.casaSegura = TRUE;

            node_aux -> item.jogador_peao[JOGADOR1][0] = FALSE;
            node_aux -> item.jogador_peao[JOGADOR1][1] = FALSE;
            node_aux -> item.jogador_peao[JOGADOR1][2] = FALSE;
            node_aux -> item.jogador_peao[JOGADOR1][3] = FALSE;

            node_aux -> item.jogador_peao[JOGADOR2][0] = TRUE;
            node_aux -> item.jogador_peao[JOGADOR2][1] = TRUE;
            node_aux -> item.jogador_peao[JOGADOR2][2] = TRUE;
            node_aux -> item.jogador_peao[JOGADOR2][3] = TRUE;
        } else {
            node_aux -> item.casaSegura = FALSE;
            
            node_aux -> item.jogador_peao[JOGADOR1][0] = FALSE;
            node_aux -> item.jogador_peao[JOGADOR1][1] = FALSE;
            node_aux -> item.jogador_peao[JOGADOR1][2] = FALSE;
            node_aux -> item.jogador_peao[JOGADOR1][3] = FALSE;

            node_aux -> item.jogador_peao[JOGADOR2][0] = FALSE;
            node_aux -> item.jogador_peao[JOGADOR2][1] = FALSE;
            node_aux -> item.jogador_peao[JOGADOR2][2] = FALSE;
            node_aux -> item.jogador_peao[JOGADOR2][3] = FALSE;
        }

        casa_anterior -> next = node_aux;
        tabuleiro -> tail = node_aux;
        tabuleiro -> length++;
        casa_anterior = node_aux;
    }

    tabuleiro -> tail -> next = tabuleiro -> head;
}


/*-----------------------------------------------------------------------FUNÇÃO PARA LER UM FICHEIRO COM CASAS SEGURAS*/
void lerFicheiro(char * nome_do_ficheiro, list * tabuleiro){
    int numero_no_ficheiro;
    FILE * ficheiro;
    ficheiro = fopen(nome_do_ficheiro, "r"); /*--------------------------------------ABERTURA DO FICHEIRO PARA LEITURA*/

    if (ficheiro == NULL) /*------------------------------------------------------VERIFICAR SE O FICHEIRO É ENCONTRADO*/
    {
        fputs(FILE_ERR1, stdout);
    }
    
    while (fscanf(ficheiro, "%d\n", &numero_no_ficheiro) != EOF) /*--------------------LEITURA DOS NÚMEROS NO FICHEIRO*/
    {
        encontrar_a_casa(*tabuleiro, numero_no_ficheiro) -> casaSegura = TRUE; /*--------PASSAR A CASA PARA CASA SEGURA*/
    }

    fclose(ficheiro); /*-----------------------------------------------------------------------------FECHAR O FICHEIRO*/

    printf("fich %s\n", nome_do_ficheiro);
}


/*------------------------------------------------------------------------------FUNÇÃO PARA LIBERTAR OS NODES DA LISTA*/
void libertar(list * tabuleiro){
    node * tmp;

    tabuleiro -> tail -> next = NULL; /*--------------------------------------------------------SEPARAR A TAIL DA HEAD*/

    while (tmp != tabuleiro -> tail) /*---------------------------------------------LIBERTAR CADA NODE INDIVIDUALMENTE*/
    {
        tmp = tabuleiro -> head;
        tabuleiro -> head = tabuleiro -> head -> next;

        free(tmp);
    }   
}


/*-----------------------------------------------------------------------------FUNÇÃO PARA UMA PEÇA ANDAR NO TABULEIRO*/
void mover_casas(list * tabuleiro, int numero_de_casas_para_andar, char peca_para_andar, int id_do_jogador){
    int i, j;
    int peca_na_matriz = char_para_int(peca_para_andar);
    node * casa_do_jogador2, * tmp;

    /*-----------------------------------------IR ATÉ METADE DO TABULEIRO E DEFINIR COMO A CASA DE PARTIDA DO JOGADOR2*/
    for (casa_do_jogador2 = tabuleiro -> head, i = 0; i < (tabuleiro -> length / 2); casa_do_jogador2 = casa_do_jogador2 -> next, i++);

    /*-----------------------------------------------------------------------------------ENCONTRAR A PEÇA NO TABULEIRO*/
    for (tmp = tabuleiro -> head; tmp -> item.jogador_peao[id_do_jogador][peca_na_matriz] == FALSE; tmp = tmp -> next);
    
    if (tmp -> item.jogador_peao[id_do_jogador][peca_na_matriz] == WIN)
    {
        puts(INVAL_MOVE);
        return; /*-----------------------------------------------------------------SE A PEÇA JÁ GANHOU, A FUNÇÃO ACABA*/
    } else {
        tmp -> item.jogador_peao[id_do_jogador][peca_na_matriz] = FALSE; /*----------------RETIRAR A PEÇA DO TABULEIRO*/
    }
    
    for (i = 0; i < numero_de_casas_para_andar; i++) /*--------------------PERCORRER A LISTA O NÚMERO DE CASAS ANDADAS*/
    {
        /*-------------------------------------------------------VER SE EXISTEM CASAS DO JOGADOR ADVERSÁRIO E COMÊ-LAS*/
        if (i > 0 && id_do_jogador == JOGADOR1 && tmp -> item.casaSegura == FALSE) 
        {
            for (j = 0; j < 4; j++)
            {
                if (tmp -> item.jogador_peao[JOGADOR2][j] == TRUE)
                {
                    tmp -> item.jogador_peao[JOGADOR2][j] = FALSE;
                    casa_do_jogador2 -> item.jogador_peao[JOGADOR2][j] = TRUE;
                }
            }           
        }
        if (i > 0 && id_do_jogador == JOGADOR2 && tmp -> item.casaSegura == FALSE)
        {
            for (j = 0; j < 4; j++)
            {
                if (tmp -> item.jogador_peao[JOGADOR1][j] == TRUE)
                {
                    tmp->item.jogador_peao[JOGADOR1][j] = FALSE;
                    tabuleiro->head->item.jogador_peao[JOGADOR1][j] = TRUE;
                }
            }   
        }

        if (i > 0 && id_do_jogador == JOGADOR1 && tmp == tabuleiro -> head) /*-----------------VER SE A PEÇA JA GANHOU*/
        {
            tmp -> item.jogador_peao[id_do_jogador][peca_na_matriz] = WIN;
            return;
        } else if (i > 0 && id_do_jogador == JOGADOR2 && tmp == casa_do_jogador2) /*-----------VER SE A PEÇA JÁ GANHOU*/
        {
            tmp -> item.jogador_peao[id_do_jogador][peca_na_matriz] = WIN;
            return;
        } else {
            tmp = tmp -> next; /*--------------------------------------SE NADA SE CONFIRMAR, PASSA PARA O PRÓXIMO NODE*/
        }        
    }

    tmp -> item.jogador_peao[id_do_jogador][peca_na_matriz] = TRUE; /*---------------------COLOCAR A PEÇA NO TABULEIRO*/
}


/*-------------------------------------------------VERIFICAR SE O JOGADOR PODE MOVER A PEÇA (A PEÇA AINDA NÃO GANHOU)*/
int peca_ainda_nao_ganhou(list * tabuleiro, int id_do_jogador, char peca_para_andar){
    int peca_em_int = char_para_int(peca_para_andar);
    node * tmp;

    for (tmp = tabuleiro -> head; tmp -> item.jogador_peao[id_do_jogador][peca_em_int] == FALSE; tmp = tmp -> next);

    if (tmp->item.jogador_peao[id_do_jogador][peca_em_int] == WIN)
    {
        return 0;
    } else {
        return 1;
    }
}


/*----------------------------------------------FUNÇÃO PARA VERIFICAR SE UM JOGADOR PODE ESCOLHER UMA DETERMINADA PEÇA*/
int pode_andar_com_a_peca(int id_do_jogador, char peca){
    if (id_do_jogador == JOGADOR1) /*------SE FOR O JOGADOR1 A JOGAR, VERIFICAR SE A PEÇA QUE SELECIONOU É DELE/EXISTE*/
    {
        if (peca != 'a' && peca != 'b' && peca != 'c' && peca != 'd')
        {
            return 0;
        }        
    }
    
    if (id_do_jogador == JOGADOR2) /*------SE FOR O JOGADOR2 A JOGAR, VERIFICAR SE A PEÇA QUE SELECIONOU É DELE/EXISTE*/
    {
        if (peca != 'w' && peca != 'x' && peca != 'y' && peca != 'z')
        {
            return 0;
        }
    }

    return 1;
}


/*-----------------------------------------------------------------------------------FUNÇÃO PARA FAZER O PRINT DO MENU*/
void print_do_menu(){
    puts("+------------------------------------+");
    puts("|         Nao Te Constipes           |");
    puts("+------------------------------------+");
    puts("| <id do peao> (abcd, xyzw)          |");
    puts("| s - sair                           |");
    puts("| h - imprimir menu                  |");
    puts("+------------------------------------+");
}


/*------------------------------------------------------------------------------FUNÇÃO PARA VER SE O JOGADOR JÁ GANHOU*/
int vitoria(list * tabuleiro, int id_do_jogador){
    int i;
    node * tmp;

    if (id_do_jogador == JOGADOR1) 
    {
        tmp = tabuleiro -> head; /*-------------------------SE FOR O JOGADOR1, A SUA CASA DE PARTIDA É A HEAD DA LISTA*/
    } else {
        /*----------------------------------------SE FOR O JOGADOR2, A CASA DE PARTIDA É A CASA NA METADE DO TABULEIRO*/
        for (tmp = tabuleiro -> head, i = 0; i < (tabuleiro -> length / 2); tmp = tmp -> next, i++);
    }

    /*---------------------------------------------------------VERIFICAR SE TODAS AS PEÇAS NA CASA ESTÃO NO ESTADO WIN*/
    if (tmp->item.jogador_peao[id_do_jogador][0] == WIN && tmp->item.jogador_peao[id_do_jogador][1] == WIN && 
    tmp->item.jogador_peao[id_do_jogador][2] == WIN && tmp->item.jogador_peao[id_do_jogador][3] == WIN)
    {
        return 1;
    } else {
        return 0;
    }
    
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------F_MAIN-------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------argc dá o numero de argumento (default é 1 por causa do ./main)*/
/*---------------------------------------------------------------------------------argv[x] dá o argumento na posição x*/
int main(int argc, char* argv[]){
    char nome_do_ficheiro[MAX], opcao;
    int colunas, linhas, modo_do_tabuleiro, numero_que_sai_nos_dados;
    int jogador_atual = JOGADOR1;
    list tabuleiro;

    /*----------------VER QUANTOS ARGUMENTOS SÃO PASSADOS NO TERMINAL E ATRIBUIR OS RESPETIVOS VALORES UM POUCO SCUFFED*/
    if (argc < 2) /*-----------------------------------------------------------------------------------VALORES DEFAULT*/
    {
        modo_do_tabuleiro = 0;
        linhas = 3;
        colunas = 7;
    } else if (argc == 2)
    {
        modo_do_tabuleiro = atoi(argv[1]);
        if (modo_do_tabuleiro != 0 && modo_do_tabuleiro != 1)
        {
            puts(INVAL_PARAMS);
            exit(0);
        }

        linhas = 3;
        colunas = 7;
    } else if (argc == 3)
    {
        modo_do_tabuleiro = atoi(argv[1]);
        if (modo_do_tabuleiro != 0 && modo_do_tabuleiro != 1)
        {
            puts(INVAL_PARAMS);
            exit(0);
        }

        linhas = atoi(argv[2]);
        if (linhas < 3 || linhas % 2 == 0)
        {
            puts(INVAL_PARAMS);
            exit(0);
        }

        colunas = 7;
    } else if (argc == 4)
    {
        modo_do_tabuleiro = atoi(argv[1]);
        if (modo_do_tabuleiro != 0 && modo_do_tabuleiro != 1)
        {
            puts(INVAL_PARAMS);
            exit(0);
        }

        linhas = atoi(argv[2]);
        if (linhas < 3 || linhas % 2 == 0)
        {
            puts(INVAL_PARAMS);
            exit(0);
        }

        colunas = atoi(argv[3]);
        if (linhas < 4)
        {
            puts(INVAL_PARAMS);
            exit(0);
        }
    } else if (argc == 5)
    {
        modo_do_tabuleiro = atoi(argv[1]);
        if (modo_do_tabuleiro != 0 && modo_do_tabuleiro != 1)
        {
            puts(INVAL_PARAMS);
            exit(0);
        }

        linhas = atoi(argv[2]);
        if (linhas < 3 || linhas % 2 == 0)
        {
            puts(INVAL_PARAMS);
            exit(0);
        }

        colunas = atoi(argv[3]);
        if (linhas < 4)
        {
            puts(INVAL_PARAMS);
            exit(0);
        }

        strcpy(nome_do_ficheiro, argv[4]);
    }

    if (calcular_numero_de_casas(linhas, colunas) > 128)
    {
        puts(INVAL_PARAMS);
        exit(0);
    }
    

    srand(1); /*---------------------------------------------------------------------------------------INITIALIZE SEED*/

    /*---------------------------------------------------------------------------------INICIALIZAÇÃO DA LIST TABULEIRO*/
    tabuleiro.head = NULL;
    tabuleiro.tail = NULL;
    tabuleiro.length = 0;

    funcao_do_tabuleiro(&tabuleiro, calcular_numero_de_casas(linhas, colunas)); /*-------------------CRIAR O TABULEIRO*/

    if (argc == 5) /*-----CASO O UTILIZADOR TENHA INSERIDO UM FICHEIRO NO TERMINAL, VAI LER E COLOCAR AS CASAS SEGURAS*/
    {
        lerFicheiro(nome_do_ficheiro, &tabuleiro);
    }

    boardPrint(linhas, colunas, tabuleiro, modo_do_tabuleiro); /*-----------------------------------PRINT DO TABULEIRO*/

    numero_que_sai_nos_dados = rolldice(2); /*---------------------------------------------------------LANÇAR OS DADOS*/

    print_do_menu(jogador_atual, numero_que_sai_nos_dados);

    do
    {
        if (jogador_atual == JOGADOR1)
        {
            puts(PL1_MOVE);
        } else {
            puts(PL2_MOVE);
        }
        printf("Jogador lancou dados com valor %d\n", numero_que_sai_nos_dados);
        printf("> ");
        scanf(" %c", &opcao);

        if (opcao == 'h') /*--------------------------------------------------------------OPCAO DE FAZER PRINT DO MENU*/
        {
            print_do_menu(jogador_atual, numero_que_sai_nos_dados);
            scanf("%c", &opcao);
        } else if (opcao == 'a' || opcao == 'b'|| opcao == 'c'|| opcao == 'd'|| opcao == 'w'|| opcao == 'x'|| opcao == 'y'|| opcao == 'z')
        {
            /*-----------VERIFICAR SE O UTILIZADOR SELECIONOU UMA PEÇA QUE LHE COMPETE E SE ESSA PEÇA AINDA NÃO GANHOU*/
            if (pode_andar_com_a_peca(jogador_atual, opcao) && peca_ainda_nao_ganhou(&tabuleiro, jogador_atual, opcao))
            {
                mover_casas(&tabuleiro, numero_que_sai_nos_dados, opcao, jogador_atual);

                boardPrint(linhas, colunas, tabuleiro, modo_do_tabuleiro); /*-----------------------PRINT DO TABULEIRO*/
                
                if (!vitoria(&tabuleiro, jogador_atual))
                {
                    /*---------------------------------------------------------------------MUDANÇA DO TURNO DO JOGADOR*/
                    if (jogador_atual == JOGADOR1)
                    {
                        jogador_atual = JOGADOR2;
                    } else {
                        jogador_atual = JOGADOR1;
                    }

                    numero_que_sai_nos_dados = rolldice(2); /*------------ROLAR OS DADOS PARA SAIR UM NÚMERO ALEATÓRIO*/
                } else {
                    if (jogador_atual == JOGADOR1)
                    {
                        puts(PL1_WINS);
                    } else {
                        puts(PL2_WINS);
                    }

                    libertar(&tabuleiro);
                    exit(0);
                }
            } else{
                puts(INVAL_MOVE);
            }         
        } else if (opcao == 's') /*-------------------------------------------OPÇÃO DE ENCERRAR O PROGRAMA DO PROGRAMA*/
        {
            puts(EXIT_MSG);
            libertar(&tabuleiro);
            break;
        } else { /*--------------------------------------------------------CASO O UTILIZADOR INSIRA UMA OPÇÃO INVÁLIDA*/
            puts(INVAL_MOVE);
        }     
    } while (opcao != 's');

    return 0;
}