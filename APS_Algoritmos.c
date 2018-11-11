#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define RECUO_PADRAO 4 // parâmetro de renderização (espaço de recuo do tabuleiro à esquerda da tela)
#define BLOCOS_POR_LINHA 10 // parâmetro de renderização
#define MIN_CASAS 20 // número especificado na APS
#define MAX_CASAS 48 // número especificado na APS
#define NUMERO_ARMADILHAS 3 // número especificado na APS
#define NUMERO_JOGADORES 3 // número especificado na APS; possível um máximo de 6 para renderizar corretamente

/* para tornar mais legiveis algumas estruturas de escolha */
enum boolean {
    FALSO = 0,
    VERDADEIRO = 1
};

/* para tornar mais legiveis os valores de retorno da função jogar(...) */
enum status_jogo {
    ABORTADO = 1,
    FINALIZADO = 2
};

void randomizar();
int mostrarCabecalho();
int iniciarTabuleiro();
int iniciarArmadilhas(int casas, int * armadilhas);
int jogar(int casas, int * armadilhas);
int rolar();
int mover(int posicao, int lance, int totalCasas);
void lHorizontal(int blocos);
void bVertical(int primeiro, int ultimo, int superior, char * conteudo);
void lVertical(int blocos, int superior, int inicial, int final, int * posicoesArmadilhas, int * posicoesJogadores, char * nomes);
void desenharLinha(int blocos, int inicial, int final, int par, int * posicoesArmadilhas, int * posicoesJogadores, char * nomes);
void renderizar(int casas, int * armadilhas, int rodada, int jogada, int jogadores, char * nomes, int * posicoes, int dado1, int dado2, int posicaoAnterior);

int main()
{
    randomizar(); // semente para geração de números aleatórios;
    mostrarCabecalho(); // imprime na tela os dados do grupo

    int status;
    int casas = iniciarTabuleiro(); // colhe do usuário um número de casas válido para o tabuleiro
    int armadilhas[casas];

    iniciarArmadilhas(casas, armadilhas); // colhe do usuário e valida o posicionamento das armadilhas

    /*
        Depois de colhidos os parâmetros do jogo, faz um loop que só termina quando
        alguém vence ou quando o jogo é abortado.
    */
    do {
        status = jogar(casas, armadilhas);
    } while (status != ABORTADO && status != FINALIZADO);

    return 0;
}

void randomizar() {
    /*
        Quando esta funcao é executada mais de uma vez no mesmo segundo (o que é bastante comum ocorrer),
        a semente torna-se inócua, porque o número é sempre o mesmo dentro do mesmo segundo.
        Assim, optamos por executar a semente uma única vez no início da execução do jogo para aumentar o grau de
        aleatoriedade dos dados.
    */
    srand(time(NULL));
}

int mostrarCabecalho() {

    char * nomes[] = {
        "Caio Moyses de Lima",
        "Leonardo B N Siqueira",
        "Lucas M L Cunha ",
        "Pedro H C Silva"
    };
    char * RA[] = {
        "21194833",
        "21216660",
        "21188361",
        "21201325"
    };
    int i;

    printf("\n%*s", RECUO_PADRAO, "");
    printf("******************************************************");
    printf("\n%*s", RECUO_PADRAO, "");
    printf("*                                                    *");
    printf("\n%*s", RECUO_PADRAO, "");
    printf("*            APS ALGORITMOS E PROGRAMACAO            *");
    printf("\n%*s", RECUO_PADRAO, "");
    printf("*                                                    *");
    printf("\n%*s", RECUO_PADRAO, "");
    printf("******************************************************");
    printf("\n%*s", RECUO_PADRAO, "");
    printf("*                                                    *");
    printf("\n%*s", RECUO_PADRAO, "");
    printf("* GRUPO:                                             *");
    printf("\n%*s", RECUO_PADRAO, "");
    printf("*                                                    *");
    printf("\n%*s", RECUO_PADRAO, "");
    printf("*                                                    *");
    for (i = 0; i <= 3; i++) {
        printf("\n%*s", RECUO_PADRAO, "");
        printf("* Aluno %i: %-21s (RA %-8s) %7c", (i+1), nomes[i], RA[i], '*');
    }
    printf("\n%*s", RECUO_PADRAO, "");
    printf("*                                                    *");
    printf("\n%*s", RECUO_PADRAO, "");
    printf("******************************************************\n");
    printf("\n%*s", RECUO_PADRAO, "");
    printf("Bem vindo ao Jogo de Trilhas!!\n\n");
    printf("\n%*s", RECUO_PADRAO, "");
    printf("Este e um jogo para %i jogadores.\n", NUMERO_JOGADORES);
    printf("\n%*s", RECUO_PADRAO, "");
    printf("Primeiro vamos configurar o tabuleiro e depois iniciar a partida.\n\n");

    return 0;
}

int iniciarTabuleiro() {
    /*
        Esta funcao colhe do usuario um numero valido de casas para o tabuleiro (excluidas as casas
        de início e fim, conforme especificado na APS.
        Para melhor organizacao do codigo, optou-se por especificar os limites minimo e maximo
        por meio de constantes.
    */

    int tamanho;

    do {
        printf("\n%*s", RECUO_PADRAO, "");
        printf("Entre com o tamanho do tabuleiro (de %i a %i casas): ", MIN_CASAS, MAX_CASAS);
        scanf("%i", &tamanho);
        if (tamanho < MIN_CASAS || tamanho > MAX_CASAS) {
            printf("\n%*s", RECUO_PADRAO, "");
            printf("Tamanho invalido\n");
        }
    } while (tamanho < MIN_CASAS || tamanho > MAX_CASAS);

    return tamanho;
}

int iniciarArmadilhas(int casas, int * armadilhas) {

    /*
        Esta funcao colhe do usuario e valida a posicao das armadilhas, conforme especificado na APS.
        Para melhor organizacao do codigo, optou-se por especificar o numero de armadilhas em uma constante.
    */

    int i, min, max;

    for (i = 0; i < NUMERO_ARMADILHAS; i++) {
        min = (i == 0) ? 1 : armadilhas[i - 1] + 3;
        max = casas - ((NUMERO_ARMADILHAS - 1 - i) * 3);
        do {
            printf("\n%*s", RECUO_PADRAO, "");
            printf("Entre com a posicao da armadilha %i (entre %i e %i): ", (i + 1), min, max);
            scanf("%i", &armadilhas[i]);
            if (armadilhas[i] < min || armadilhas[i] > max) {
                printf("\n%*s", RECUO_PADRAO, "");
                printf("Posicao invalida\n");
            }
        } while (armadilhas[i] < min || armadilhas[i] > max);
    }

    return 0;
}

int jogar(int casas, int * armadilhas) {

    /*
        Esta funcao é o coracao do programa. Ela tem argumentos tao somente o numero de casas do tabuleiro
        e a posicao das armadilhas (fornecida num vetor).
    */

    int i, j, rodada = 0, dado1 = 0, dado2 = 0, posicaoAnterior;
    int jogadores = NUMERO_JOGADORES, posicoes[NUMERO_JOGADORES], apanhados[NUMERO_JOGADORES];
    char nomes[NUMERO_JOGADORES];
    char comando;

    for (i = 0; i < NUMERO_JOGADORES; i++) {
        nomes[i] = 65 + i; // Atribui um caractere ASCII a cada jogador, a partir do A (=65)
        posicoes[i] = 0; // Inicializa o vetor que armazenara as posicoes dos jogadores ao longo da partida
        apanhados[i] = FALSO; // Inicializa o vetor que indica se o jogador caiu numa armadilha
    }

    /* inicializa renderização, com valores zerados. O -1 como parametro de jogada é arbitrario. Poderia ser qualquer valor */
    renderizar(casas, armadilhas, rodada, -1, jogadores, nomes, posicoes, 0, 0, 0);

    do {
        rodada++; // Cada rodada corresponde a um loop completo. Todos jogam, exceto os que tiverem sido apanhados em armadilha na rodada anterior.

        for (i = 0; i < NUMERO_JOGADORES; i++) {

            if (!apanhados[i]) { // se o jogador não tiver sido apanhado na rodada anterior, o bloco é executado

                printf("\n%*s", RECUO_PADRAO, ""); // efeito estetico apenas
                printf("Jogador %c: pressione qualquer tecla para jogar (Q para sair): ", nomes[i]); // o jogo pode ser abortado em qualquer jogada
                fflush(stdin);
                scanf("%c", &comando);

                if (comando == 'q' || comando == 'Q') {
                    printf("\n%*s", RECUO_PADRAO, "");
                    printf("JOGO ABORTADO...\n\n");
                    return ABORTADO;
                }

                /*
                    Esta e a parte nuclear da jogada
                    Os dados sao rolados separadamente e e seguida é feito o calculo
                    da posicao atual pela funcao mover(...) e verificado se o jogador
                    foi apanhado numa armadilha
                */
                dado1 = rolar();
                dado2 = rolar();
                posicaoAnterior = posicoes[i];
                // o parametro casas + 1 decorre da necessidade de que o jogo termine na casa "Final"
                posicoes[i] = mover(posicoes[i], dado1 + dado2, casas + 1);

                for (j = 0; j < NUMERO_ARMADILHAS; j++) {
                    if (armadilhas[j] == posicoes[i]) {
                        apanhados[i] = VERDADEIRO;
                        break;
                    }
                }

                // Esta funcao mostra a situacao da partida a cada jogada
                renderizar(casas, armadilhas, rodada, i, jogadores, nomes, posicoes, dado1, dado2, posicaoAnterior); //

                if (posicoes[i] == casas + 1) {
                    printf("\n%*s", RECUO_PADRAO, "");
                    printf("**** %c VENCEU NA RODADA %i ****\n\n", nomes[i], rodada);
                    getch();
                    return FINALIZADO;
                }
            } else {
                // Aqui apenas alteramos o estado do flag para falso, liberando o jogador a voltar
                // a rolar os dados na proxima rodada apos uma rodada preso na armadilha.
                // Nao há renderizacao, porque o estado do jogo permanece inalterado em relacao
                // aa rodada anterior.
                apanhados[i] = FALSO;
            }
        }

    } while (VERDADEIRO);

    return 0;
}

int rolar() {
    // obtem um numero aletorio entre 1 e 6
    return 1 + (rand() % 6);
}

int mover(int posicao, int lance, int totalCasas) {
    // move a peca do jogador levando em conta a especificacao da APS
    // segundo a qual so vence o jogador que terminar exatamente na
    // casa "Final", devendo ser recuada a peca pelo numero de casas em excesso
    // caso o valor obtido nos dados ultrapasse o necessario
    return totalCasas - abs(totalCasas - (posicao + lance));
}

/*
    A partir daqui as funcoes apenas renderizam o status do jogo para os usuarios.
    Apenas a funcao renderizar(...) e invocada pelo jogo. Todas as demais sao
    invocadas somente pela propria funcao renderizar(...). Isso traz o beneficio
    de tornar a renderizacao destacavel do restante do codigo, facilitando sua
    futura substituicao em caso de aperfeicoamentos futuros (por exemplo, se
    o grupo decidisse utilizar uma biblioteca de graficos para criar uma
    visualziacao mais sofisticada)
*/

void lHorizontal(int blocos) {
    // Esta funcao cria as linhas horizontais que iniciam e
    // encerram cada fileira de de casas do tabuleiro
    int i;
    for (i = 0; i < blocos; i++) {
        printf("-------");
        printf("%*s", RECUO_PADRAO, "");
    }
}

void bVertical(int primeiro, int ultimo, int superior, char * conteudo) {
    // Esta funcao cria cada um dos blocos de linhas verticais que delimitam
    // as casas, cria os conectores entre as casas, quando eh o caso, e
    // preenche o interior das casas (o "miolo") fornecido como argumento da funcao
    printf("%s%s%s",
        primeiro ? "|" : superior ? "__|" : "  |",
        conteudo,
        ultimo ? "|" : superior ? "|__" : "|  "
    );
}

void lVertical(int blocos, int superior, int inicial, int final, int * posicoesArmadilhas, int * posicoesJogadores, char * nomes) {

    // Esta funcao cria uma fileira de casas do tabuleiro invocando sucessivamente, segundo
    // o numero de casas por fileira, a funcao bVertical.
    // Aqui tambem eh definido como deve ser exibido o conteudo de cada casa, conforme haja ou nao uma armadilha e
    // verificado quais jogadores devem ser exibidos em cada casa
    // A existencia de uma armadilha é indicada pelo caractere "@".
    // Os jagadores são enfileirados e tem uma ordem fixa de posicionamento, baseada na posicao que ocupam no vetor.
    // Para uma correta renderizacao, admite-se um total de ate seis jogadores, mas esse numero poderia ser facilmente
    // ampliado alterando-se os outros parametros de renderizacao que definem o tamanho de cada casa.

    int i, j, armadilha;
    char conteudo[6] = "     \0";

    for (i = 0; i < blocos; i++) { // percorre bloco por bloco (conforme parametro fornecido como argumento da funcao)
        armadilha = FALSO;
        for (j = 0; j < NUMERO_ARMADILHAS; j++) { // verifica se o bloco tem armadilha (conforme parametro fornecido como argumento da funcao)
            if (posicoesArmadilhas[j] == i) {
                armadilha = VERDADEIRO;
                break;
            }
        }
        if (superior) {  // a renderizacao da parte superior eh mais facil, porque nessa parte nao aparecem as pecas dos jogadores
            bVertical(i == 0, i == (blocos - 1), superior, armadilha ? "@@@@@\0" : "     \0");
        } else {
            for (j = 0; j < 5; j++) { // nesta parte (inferior) é que sao posicionadas as pecas dos jogadores
                if (j <= NUMERO_JOGADORES - 1 && posicoesJogadores[j] == i) {
                    conteudo[j] = nomes[j];
                } else {
                    conteudo[j] = armadilha ? '@' : ' ';
                }
            }
            conteudo[5] = '\0';
            bVertical(i == 0, i == (blocos - 1), superior, conteudo);
        }
    }
}

void desenharLinha(int blocos, int inicial, int final, int par, int * posicoesArmadilhas, int * posicoesJogadores, char * nomes) {

    // Esta é a funcao utilizada para desenhar cada linha de casas do tabuleiro.
    // A construcao eh feita em camadas, invocando-se as funcoes lHorizontal + lVertical + lVertical + lHorizontal

    // Se a linha eh par, o movimento das pecas se da da direita para a esquerda
    // Por isso eh necessario definir uma formula diferente de recuo para as fileiras pares,
    // ja que elas iniciam na extremidade direita do tabuleiro e podem terminar antes de chegar
    // aa extremidade esquerda
    int recuo = par ? (BLOCOS_POR_LINHA * 11) - (blocos * 11) + RECUO_PADRAO : RECUO_PADRAO;

    // se é a fileira inicial, importante posicionar o rotulo "Iniciar"
    // logo acima da primeira casa.
    if (inicial) {
        printf("%*s", RECUO_PADRAO, "");
        printf("INICIO\n");
    }

    //     -------    -------    -------    -------    -------    -------    -------    -------    -------    -------
    printf("%*s", recuo, "");
    lHorizontal(blocos);
    printf("\n");

    //     |     |____|@@@@@|____|     |____|     |____|@@@@@|____|     |____|     |____|@@@@@|____|     |____|     |
    printf("%*s", recuo, "");
    lVertical(blocos, VERDADEIRO, inicial, final, posicoesArmadilhas, posicoesJogadores, nomes);
    printf("\n");

    //     |ABC  |    |@@@@@|    |     |    |     |    |@@@@@|    |     |    |     |    |@@@@@|    |     |    |     |
    printf("%*s", recuo, "");
    lVertical(blocos, FALSO, inicial, final, posicoesArmadilhas, posicoesJogadores, nomes);
    printf("\n");

    //     -------    -------    -------    -------    -------    -------    -------    -------    -------    -------
    printf("%*s", recuo, "");
    lHorizontal(blocos);

    // se é a fileira final, importante posicionar o rotulo "Final"
    // logo abaixo da ultima casa.
    if (!final) {
        if (par) {
            printf("\n%8s\n%8s\n", "|", "|");
        } else {
            printf("\n%*s\n%*s\n", (blocos * 11), "|   ", (blocos * 11), "|   ");
        }
    } else {
        printf("\n%*s\n", par ? recuo + 7 : (blocos * 11), "FINAL ");
    }
}

void renderizar(int casas, int * armadilhas, int rodada, int jogada, int jogadores, char * nomes, int * posicoes, int dado1, int dado2, int posicaoAnterior) {

    // Esta eha a funcao que organiza toda a montagem da visualizacao do jogo.
    // Como ela eh quanse totalmente desacoplada das funcoes do jogo em si (compartilha somente algumas constantes), poderia ser
    // substituida com outra funcao que tivesse a mesmas assinatura de modo a permitir o aprimoramento da interface visual sem afetar
    // o restante do codigo.

    int resto = (casas + 2) % BLOCOS_POR_LINHA; // aqui calculamos se a ultima linha tem menos casas que as demais
    int linhas = ((casas + 2) / BLOCOS_POR_LINHA) + (resto > 0 ? 1 : 0); // aqui calculamos o numero total de linhas do tabuleiro
    int posicoesArmadilhas[NUMERO_ARMADILHAS]; // para renderizar corretamente as armadilhas
    int posicoesJogadores[NUMERO_JOGADORES]; // para renderizar corretamente a posicao dos jogadores
    int i, j, par, venceu, armadilha, inicial, final, blocos;

    system("cls"); // Limpa o console a cada nova alteracao do status do jogo

    // se a rodada eh 0, significa que o jogo nao comecou. Nesse caso, mostra a tela inicial da partida
    if (rodada == 0) {
        printf("\n\n\n%*s", RECUO_PADRAO, "");
        printf("!!! ATENCAO !!! Vamos comecar\n\n\n\n\n\n");

    // A partir daqui ocorre a renderizacao das jogadas
    // Esse loop inicial monta o cabecalho posicionado acima do tabuleiro,
    // com a situacao de cada jogador
    } else {
        printf("\n%*s", RECUO_PADRAO, "");
        printf("Rodada %i, Jogada %i\n\n", rodada, jogada);

        for (i = 0; i < NUMERO_JOGADORES; i++) {
            printf("\n%*s", RECUO_PADRAO, "");
            if (i == jogada) {
                printf("Jogador %c: joga os dados %i + %i = %i ", nomes[i], dado1, dado2, (dado1 + dado2));
                if (rodada == 1) {
                    printf("(vai para %i)", posicoes[i]);
                } else {
                    printf("(%s de %i para %i)", posicaoAnterior <= posicoes[i] ? "vai" : "volta", posicaoAnterior, posicoes[i]);
                }
            } else {
                if (rodada >= 1 && posicoes[i] > 0) {
                    printf("Jogador %c: posicao %i", nomes[i], posicoes[i]);
                }
            }
            venceu = posicoes[i] == (casas + 1);
            armadilha = FALSO;
            for (j = 0; j < NUMERO_ARMADILHAS; j++) {
                if (armadilhas[j] == posicoes[i]) {
                    armadilha = VERDADEIRO;
                    break;
                }
            }
            if (venceu) {
                printf(" !!! VENCEU !!!\n");
            } else {
                if (armadilha) {
                    printf(" @@@ ARMADILHA @@@\n");
                } else {
                    printf("\n");
                }
            }
        }
    }
    printf("\n%*s", RECUO_PADRAO, "");
    printf("==========================================================================================================\n\n");

    // A partir daqui eh renderizado o tabuleiro em si
    // O programa percorre linha a linha, calcula em cada linha se ha armadilhas e jogadores,
    // define os blocos em que estao situados e invoca as funcoes responsaveis por desenhar essas
    // linhas passando-lhes os parametros adequados.
    for (i = 0; i < linhas; i++){

        inicial = (i == 0);
        final = (i == (linhas - 1));
        par = i % 2;
        // se ha resto, a linha final eh menor que as outras; caso contrario, eh uma linha com
        // numero de casas igual aas demais
        blocos = (final && resto) ? resto : BLOCOS_POR_LINHA;

        for (j = 0; j < NUMERO_ARMADILHAS; j++) {
            // Mapeia quais armadilhas estao nesta linha i, guardando as informacoes no vetor posicoesArmadilhas.
            // Se a armadilha esta na linha, armazena o numero correspondente a sua posicao. Se nao ha, define
            // o valor como -1.
            posicoesArmadilhas[j] = (armadilhas[j] / BLOCOS_POR_LINHA) == i ? armadilhas[j] % BLOCOS_POR_LINHA : -1;
            // Se a linha eh par, a contagem eh da direita para a esquerda
            if (par && posicoesArmadilhas[j] >= 0) {
                posicoesArmadilhas[j] = blocos - (posicoesArmadilhas[j] + 1);
            }
        }

        for (j = 0; j < NUMERO_JOGADORES; j++) {
            // O mesmo metodo utilizado para mapear as armadilhas eh utilizado para mapear as posicoes dos jogadores.
            posicoesJogadores[j] = (posicoes[j] / BLOCOS_POR_LINHA) == i ? posicoes[j] % BLOCOS_POR_LINHA : -1;
            // Se a linha eh par, a contagem eh da direita para a esquerda
            if (par && posicoesJogadores[j] >= 0) {
                posicoesJogadores[j] = blocos - (posicoesJogadores[j] + 1);
            }
        }

        // Definidos os parametros, cada linha eh efetivamente desenhada pela funcao desenharLinhar(...)
        desenharLinha(blocos, inicial, final, i % 2, posicoesArmadilhas, posicoesJogadores, nomes);
    }
}

