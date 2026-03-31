/* 
Jogo: wordz
Feito por: João Pedro Nunes Rodrigues
Matrícula: 252001181
*/

//Bibliotecas necessárias
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Macros do pré-processador
#ifdef _WIN32 
#define CLEAR "cls" 
#else 
#define CLEAR "clear" 
#endif 

#define MAX_JOGOS 50
#define GAME_FILE "wordz.txt"

typedef struct {
    char nickname[20];
    int pontuacao;
} Jogador;

// Variáveis globais
FILE *game;
char nickname[20], arquivo[20]; 
int pontuacao=0; 
char linha[100];
Jogador jogador;

// Funções
void lerCampo(const char *chave, char *dest, int tamanho) {
    for (;;) {
        game = fopen(GAME_FILE, "r");
        if (!game) { perror("Erro ao abrir arquivo"); exit(EXIT_FAILURE); }

        while (fgets(linha, sizeof(linha), game)) {
            if (strstr(linha, chave)) {
                char *p = strchr(linha, ':');
                if (p) {
                    if ((int)strlen(p) > 2) {
                        strncpy(dest, p + 2, tamanho);
                        dest[tamanho - 1] = '\0';
                        dest[strcspn(dest, "\n")] = '\0';
                        fclose(game);
                        return;
                    }
                }
            }
        }
        fclose(game);

    }
}

void escreverPrimeiraTela() {
    game = fopen(GAME_FILE, "w");
    if (!game) { perror("Erro ao abrir arquivo"); exit(EXIT_FAILURE); }

    fprintf(game, "=============================\n");
    fprintf(game, "            WORDZ            \n");
    fprintf(game, "=============================\n\n");
    fprintf(game, "Bem vindo(a)!!!!\n\n");
    fprintf(game, "Informe seu nick: ");
    fclose(game);
}

void primeiratela() {
    system(CLEAR);
    printf("Tela inicial do jogo\n");
    escreverPrimeiraTela();
    lerCampo("Informe seu nick", jogador.nickname, sizeof(jogador.nickname));
}

void escreverMenu(const char *mensagemErro) {
    game = fopen(GAME_FILE, "w");
    if (!game) { perror("Erro ao abrir arquivo"); exit(EXIT_FAILURE); }

    fprintf(game, "=============================\n");
    fprintf(game, "        MENU PRINCIPAL       \n");
    fprintf(game, "=============================\n\n");
    fprintf(game, "1 - Jogar\n");
    fprintf(game, "2 - Configuracoes\n");
    fprintf(game, "3 - Instrucoes\n");
    fprintf(game, "4 - Ranking\n");
    fprintf(game, "5 - Sair\n\n");

    if(mensagemErro!=NULL)
        fprintf(game, "%s\n\n", mensagemErro);

    fprintf(game, "Digite a opção desejada: ");
    fclose(game);
}

void instrucoes(){
    while(1){
    system(CLEAR);
    printf("Instrucoes do jogo\n");
    game = fopen(GAME_FILE, "w");
    fprintf(game, "=============================\n");
    fprintf(game, "         INSTRUCOES         \n");
    fprintf(game, "=============================\n\n");
   
    fprintf(game, "-- JOGABILIDADE --\n\n");
    fprintf(game, "1. O jogador deve tentar adivinhar as palavras secretas que contenham as letras presentes naquela fase.\n");
    fprintf(game, "2. A cada tentativa, o jogador recebe feedback sobre quais palavras estão corretas e quantas faltam.\n");
    fprintf(game, "3. O jogo termina quando:\n    - Todas as fases forem completadas;\n    - O jogador atinge pontos negativos (chega a zero);\n    - O jogador decidir sair da fase.\n\n");
    
    fprintf(game, "-- PONTUAÇÃO --\n\n");
    fprintf(game, "1. Cada palavra digitada corretamente, o jogador ganhará uma certa quantidade de pontos.\n");
    fprintf(game, "2. Cada palavra digitada incorretamente, o jogador perderá uma certa quantidade de pontos.\n");
    fprintf(game, "3. Ao terminar a fase, o jogador ganhará pontos extras baseado na quantidade de erros, então quanto menos erros, mais pontos!\n");
    fprintf(game, "4. A quantidade de pontos debitados ou creditados dependem do nível de dificuldade escolhido.\n\n");

    fprintf(game, "-- RANKING --\n\n");
    fprintf(game, "1. O ranking exibe os jogadores com as maiores pontuações.\n");
    fprintf(game, "2. O ranking é atualizado ao final de cada jogo.\n");
    fprintf(game, "3. O jogador pode visualizar o ranking a qualquer momento no menu principal.\n");
    fprintf(game, "4. Não será exibido no ranking jogadores com pontuação zero ou negativa.\n");

    fprintf(game, "\nLembre-se de ler o README.txt para mais detalhes sobre o jogo e como ele funciona!\n\n");

    fprintf(game, "Digite SAIR para voltar ao menu principal: ");
    fclose(game);

    char input[100];
    lerCampo("Digite SAIR para voltar ao menu principal", input, sizeof(input));

    if (strcmp(input, "SAIR") == 0)
    {
        printf("Saindo de instrucoes...\n");
        return;
    } else {
        printf("Entrada invalida. Por favor, digite SAIR para voltar ao menu principal.\n");
    }
}
}
// LOOP PRINCIPAL

void menuLoop() {
    system(CLEAR);
    printf("Menu principal\n");
    printf("Bem vindo, %s!\n", jogador.nickname);
    char opcao[16];
    int escolha;
    const char *mensagemErro = NULL; 

    while (1) {
        escreverMenu(mensagemErro);
        mensagemErro = NULL;

        lerCampo("Digite a opção desejada", opcao, sizeof(opcao));
        escolha = atoi(opcao);

        switch (escolha) {
            case 1:
                printf("Iniciando jogo...\n");
                break;
            case 2:
                printf("Abrindo configuracoes...\n");
                break;
            case 3:
                instrucoes();
                break;
            case 4:
                printf("Exibindo ranking...\n");
                break;
            case 5:
                system(CLEAR);
                printf("Obrigado por jogar, %s!\n", jogador.nickname);
                game = fopen(GAME_FILE, "w");
                fprintf(game, "O jogo foi encerrado.\n\nObrigado por jogar %s!\nSua pontuação final foi de: %d\n", jogador.nickname, jogador.pontuacao);
                fclose(game);
                exit(0);
            default:
                mensagemErro = "Opção inválida. Tente novamente!";
                printf("O usuario escolheu uma opcao invalida.\n");
                break;
        }
    }
}

// MAIN

int main() {
    primeiratela();
    menuLoop();
    return 0;
}