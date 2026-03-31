
/*
Jogo: Wordz
Feito por: João Pedro Nunes Rodrigues
Matrícula: 252001181
*/

// BIBLIOTECAS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// MACROS
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define MAX_JOGOS 50
#define ENTRADA_FILE "fases.txt"
#define RANK_FILE "ranking.bin"
#define SAVE_FILE "save.bin"

typedef struct
{
    char nickname[21];
    int pontuacao;
} Jogador;

typedef struct
{
    char letras[20];
    int qtd;
    char palavras[50][50];
    int achada[50];

    int qtdSecretas;
    char secretas[20][50];
    int achadaSecreta[20];
} Fase;

// VARIAVEIS GLOBAIS
FILE *entrada;
FILE *rank;
FILE *s;
char arquivo[20];
int pontuacao = 0;
char linha[100];
Jogador jogador;

// FUNCOES

// SAVE DE FASES
int read_save()
{
    s = fopen(SAVE_FILE, "rb");
    int sp = 0;

    if (s)
    {
        if (fread(&sp, sizeof(int), 1, s) != 1)
        {
            sp = 0;
        }
        fclose(s);
    }
    return sp;
}

void write_save(int sp)
{
    s = fopen(SAVE_FILE, "wb+");
    if (!s)
    {
        s = fopen(SAVE_FILE, "wb");
    }
    if (s)
    {
        fwrite(&sp, sizeof(int), 1, s);
        fclose(s);
    }
}

// TELA INICIAL
void primeiratela()
{
    system(CLEAR);
    printf("=============================\n");
    printf("            WORDZ            \n");
    printf("      Feito por: joaopnr     \n");
    printf("=============================\n\n");
    printf("Bem vindo(a)!!!!\n\n");
    printf("Antes de comecar, informe seu nick (ate 20 caracteres): ");
    if (fgets(jogador.nickname, sizeof jogador.nickname, stdin) != NULL)
    {
        jogador.nickname[strcspn(jogador.nickname, "\n")] = '\0';
    }
    else
    {
        jogador.nickname[0] = '\0';
    }

    for (int i = 0; jogador.nickname[i] != '\0'; i++)
    {
        jogador.nickname[i] = toupper((unsigned char)jogador.nickname[i]);
    }
}

// PRINTAR MENU PRINCIPAL

void escreverMenu(const char *mensagemErro)
{
    printf("=============================\n");
    printf("        MENU PRINCIPAL       \n");
    printf("      Feito por: joaopnr     \n");
    printf("=============================\n\n");

    printf("1 - Jogar\n");
    printf("2 - Configuracoes\n");
    printf("3 - Instrucoes\n");
    printf("4 - Ranking\n");
    printf("5 - Sair\n\n");

    if (mensagemErro != NULL)
    {
        printf("%s\n\n", mensagemErro);
    }
    printf("Digite a opcao desejada: ");
}

// INSTRUCOES

void instrucoes()
{
    system(CLEAR);
    printf("=============================\n");
    printf("         INSTRUCOES         \n");
    printf("     Feito por: joaopnr     \n");
    printf("=============================\n\n");

    printf("-- JOGABILIDADE --\n\n");
    printf("1. O jogador deve tentar adivinhar as palavras que contenham as letras presentes naquela fase.\n");
    printf("2. A cada tentativa, o jogador recebe feedback sobre quais palavras estao corretas e quantas faltam para completar a fase.\n");
    printf("3. O jogo termina quando:\n    - Todas as fases forem completadas;\n    - O jogador atinge uma pontuacao negativa (chega a zero);\n    - O jogador decidir sair da fase.\n");
    printf("4. Seu progresso fica salvo ao concluir uma fase e escolher voltar ao menu principal.\n\n");

    printf("-- PONTUACAO --\n\n");
    printf("1. Cada palavra digitada corretamente, o jogador ganha 100 pontos.\n");
    printf("2. Cada palavra digitada incorretamente, o jogador perde 10 pontos.\n");
    printf("3. Ao terminar a fase, o jogador recebe 25 pontos extras.\n");
    printf("4. A quantidade de pontos debitados ou creditados dependem do nivel de dificuldade escolhido.\n\n");
    printf("5. Para que sua pontuacao fique salva, complete todas as fases ou 'SAIA' de uma fase possuindo uma pontuacao positiva.\n\n");

    printf("-- RANKING --\n\n");
    printf("1. Exibe os jogadores com as maiores pontuacoes.\n");
    printf("2. O ranking eh atualizado ao final de cada jogo.\n");
    printf("3. O jogador pode visualiza-lo a qualquer momento no menu principal.\n");
    printf("4. Nao sera exibido no ranking jogadores com pontuacao zero ou negativa.\n");

    printf("\nLembre-se de ler o README.md no github para mais detalhes sobre o jogo e como ele funciona!\n\n");

    printf("Pressione <ENTER> para voltar ao menu principal. ");

    getchar();
    getchar();
    system(CLEAR);
    return;
}

// RANKING

void salvarRanking(Jogador jogador)
{
    rank = fopen(RANK_FILE, "rb+");
    if (!rank)
    {
        rank = fopen(RANK_FILE, "wb+");
    }

    Jogador temp;
    int encontrado = 0;

    while (fread(&temp, sizeof(Jogador), 1, rank) == 1)
    {
        if (strcmp(temp.nickname, jogador.nickname) == 0)
        {
            fseek(rank, -(long)sizeof(Jogador), SEEK_CUR);
            fwrite(&jogador, sizeof(Jogador), 1, rank);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
    {
        fwrite(&jogador, sizeof(Jogador), 1, rank);
    }

    fclose(rank);
}

void removerDoRanking(const char *nickname)
{
    rank = fopen(RANK_FILE, "rb");
    if (!rank)
    {
        return;
    }

    Jogador lista[256];
    int n = 0;
    while (n < 256 && fread(&lista[n], sizeof(Jogador), 1, rank) == 1)
        n++;
    fclose(rank);

    int j = 0;
    int removido = 0;
    for (int i = 0; i < n; i++)
    {
        if (strcmp(lista[i].nickname, nickname) != 0)
            lista[j++] = lista[i];
        else
            removido++;
    }
    rank = fopen(RANK_FILE, "wb");
    if (!rank)
    {
        return;
    }
    fwrite(lista, sizeof(Jogador), j, rank);
    fclose(rank);
}

void ranking()
{
    system(CLEAR);

    rank = fopen(RANK_FILE, "rb");

    printf("=============================\n");
    printf("           RANKING           \n");
    printf("      Feito por: joaopnr     \n");
    printf("=============================\n\n");

    if (!rank)
    {
        printf("Nenhum jogador registrado ainda.\n\nPressione <ENTER> para voltar ao menu principal. ");
        rank = fopen(RANK_FILE, "wb");
        fclose(rank);
        getchar();
        getchar();
        return;
    }

    // LER RANKING
    Jogador lista_ranking[100];
    int n = 0;

    while (fread(&lista_ranking[n], sizeof(Jogador), 1, rank) == 1)
    {
        n++;
    }

    fclose(rank);

    if (n == 0)
    {
        printf("Nenhum jogador registrado ainda.\n");
        printf("\nPressione <ENTER> para voltar ao menu principal. ");

        getchar();
        getchar();
        return;
    }

    // ORDENAR
    for (int j = 0; j < n - 1; j++)
    {
        int trocou = 0;
        for (int i = 0; i < n - 1 - j; i++)
        {
            if (lista_ranking[i].pontuacao < lista_ranking[i + 1].pontuacao)
            {
                Jogador temp = lista_ranking[i + 1];
                lista_ranking[i + 1] = lista_ranking[i];
                lista_ranking[i] = temp;
                trocou = 1;
            }
        }
        if (!trocou)
            break;
    }

    // POSICAO ATUAL DO JOGADOR NO RANKING
    int posicaoatual = -1;
    for (int i = 0; i < n; i++)
    {
        if (strcmp(lista_ranking[i].nickname, jogador.nickname) == 0 && lista_ranking[i].pontuacao == jogador.pontuacao && jogador.pontuacao > 0)
        {
            posicaoatual = i + 1;
            break;
        }
    }

    int limite = (n < 5 ? n : 5);

    printf("---   Top %d jogadores   ---\n\n", limite);

    // PRINTAR O RANKING
    for (int i = 0; i < limite; i++)
    {
        if (lista_ranking[i].pontuacao > 0)
        {
            printf("%d. %s - %d pontos\n",
                   i + 1,
                   lista_ranking[i].nickname,
                   lista_ranking[i].pontuacao);
        }
    }

    printf("\n-----------------------------\n\n");

    if (posicaoatual == -1)
    {
        printf("\n%s, voce tem 0 pontos.\n", jogador.nickname);
    }
    else
    {
        printf("\nSua posicao no ranking:\n%d. %s - %d pontos\n", posicaoatual, jogador.nickname, jogador.pontuacao);
    }

    printf("\nPressione <ENTER> para voltar ao menu principal. ");

    getchar();
    getchar();
}

// CARREGAR AS FASES

int carregarFases(Fase fases[])
{
    entrada = fopen(ENTRADA_FILE, "r");
    if (!entrada)
    {
        printf("Erro ao abrir arquivo %s\n", ENTRADA_FILE);
        return 0;
    }

    char linhas[500][100];
    int total = 0;

    while (fgets(linhas[total], sizeof(linhas[total]), entrada) != NULL)
    {
        linhas[total][strcspn(linhas[total], "\n")] = '\0';

        for (int k = 0; linhas[total][k] != '\0'; k++)
        {
            linhas[total][k] = toupper((unsigned char)linhas[total][k]);
        }
        total++;
    }

    fclose(entrada);

    int f = 0;
    int i = 0;

    while (i < total)
    {
        if (strcmp(linhas[i], "-") == 0 || linhas[i][0] == '\0')
        {
            i++;
            continue;
        }

        if (i >= total)
            break;
        strcpy(fases[f].letras, linhas[i]);
        i++;

        if (i >= total)
            break;
        fases[f].qtd = atoi(linhas[i]);
        i++;

        for (int p = 0; p < fases[f].qtd; p++)
        {
            if (i >= total)
                break;
            strcpy(fases[f].palavras[p], linhas[i]);
            fases[f].achada[p] = 0;
            i++;
        }

        if (i >= total)
            break;
        fases[f].qtdSecretas = atoi(linhas[i]);
        i++;

        for (int s = 0; s < fases[f].qtdSecretas; s++)
        {
            if (i >= total)
                break;
            strcpy(fases[f].secretas[s], linhas[i]);
            fases[f].achadaSecreta[s] = 0;
            i++;
        }

        f++;
    }

    return f;
}

// COMPARAR SE JA ACHOU AS PALAVRAS
int palavraSecreta(Fase *f, const char *tentativa)
{
    for (int i = 0; i < f->qtdSecretas; i++)
    {
        if (strcmp(f->secretas[i], tentativa) == 0)
        {
            if (f->achadaSecreta[i] == 1)
                return -1;
            f->achadaSecreta[i] = 1;
            return 1;
        }
    }
    return 0;
}

int palavraExiste(Fase *f, const char *tentativa)
{
    for (int i = 0; i < f->qtd; i++)
    {
        if (strcmp(f->palavras[i], tentativa) == 0)
        {
            if (f->achada[i] == 1)
            {
                return -1;
            }
            else
            {
                f->achada[i] = 1;
                return 1;
            }
        }
    }
    return 0;
}

// LOOPING PRINCIPAL DO JOGO
void jogar()
{
    system(CLEAR);

    Fase fases[50];
    int total_fases = carregarFases(fases);

    if (total_fases == 0)
    {
        printf("Nenhuma fase existente.\n");
        printf("Aperte <ENTER> para voltar ao menu principal. ");
        getchar();
        getchar();
        return;
    }

    int pontos = 0;

    int save_fase = read_save();
    if (save_fase >= total_fases)
        save_fase = 0;
    if (save_fase < total_fases)

        for (int f = save_fase; f < total_fases; f++)
        {
            int acertos = 0;
            int acertosSecretas = 0;
            int erros = 0;
            char tentativa[50];

            for (int i = 0; i < fases[f].qtd; i++)
            {
                fases[f].achada[i] = 0;
            }

            while (acertos < fases[f].qtd)
            {
                // PONTUACAO NEGATIVA
                if (pontos < 0)
                {
                    system(CLEAR);
                    printf("===================\n");
                    printf("     GAME OVER     \n");
                    printf(" Feito por: joaopnr     \n");
                    printf("===================\n\n");

                    printf("Voce obteve mais erros do que acertos!\n");
                    printf("Pontuacao adquirida: %d\n", pontos);
                    if (jogador.pontuacao >= 0)
                    {
                        printf("Sua pontuacao total atual: %d\n\n", jogador.pontuacao);
                    }
                    else
                    {
                        jogador.pontuacao = 0;
                        printf("Sua pontuacao total atual: %d\n\n", jogador.pontuacao);
                    }

                    printf("Aperte <ENTER> para voltar ao menu principal. ");
                    salvarRanking(jogador);
                    getchar();
                    return;
                }

                // SISTEMA DAS FASES
                system(CLEAR);
                printf("=============================\n");
                printf("       WORDZ | FASE %02d       \n", f + 1);
                printf("      Feito por: joaopnr     \n");
                printf("=============================\n\n");

                printf("\n");
                int placar = 0;
                for (int i = 0; fases[f].letras[i] != '\0'; i++)
                {
                    printf("   %c   ", fases[f].letras[i]);

                    if (i % 2 == 0 && i > 0)
                    {
                        if (placar == 0)
                        {
                            printf("        PONTUACAO: %d\n\n", pontos);
                            placar = 1;
                        }

                        printf("\n");
                        printf("  ");
                    }
                }
                printf("\n\n");

                printf("Faltam %d palavras.\n\n", fases[f].qtd - acertos);

                if (acertos >= 1)
                {
                    printf("Palavras encontradas:\n");
                    for (int i = 0; i < fases[f].qtd; i++)
                    {
                        if (fases[f].achada[i] == 1)
                        {
                            printf("%s\n", fases[f].palavras[i]);
                        }
                    }
                }

                if (acertosSecretas >= 1)
                {
                    printf("\nPalavras secretas encontradas:\n");
                    for (int j = 0; j < fases[f].qtdSecretas; j++)
                    {

                        if (fases[f].achadaSecreta[j] == 1)
                        {
                            printf("%s\n", fases[f].secretas[j]);
                        }
                    }
                }

                printf("\n");

                printf("Informe uma palavra: ");
                scanf("%49s", tentativa);
                for (int i = 0; tentativa[i] != '\0'; i++)
                {
                    tentativa[i] = toupper((unsigned char)tentativa[i]);
                }

                int status = palavraExiste(&fases[f], tentativa);

                if (status == 1)
                {
                    printf("\nNova palavra encontrada!          *+100 pontos*\nAperte <ENTER> para continuar. ");
                    pontos += 100;
                    jogador.pontuacao += 100;
                    acertos++;
                    getchar();
                    getchar();
                }
                else if (status == -1)
                {
                    printf("\nEsta palavra ja foi encontrada!          *-10 pontos*\nAperte <ENTER> para continuar. ");
                    pontos -= 10;
                    jogador.pontuacao -= 10;
                    erros++;
                    getchar();
                    getchar();
                }
                else
                {
                    int sec = palavraSecreta(&fases[f], tentativa);

                    if (sec == 1)
                    {
                        printf("\nVOCE DESCOBRIU UMA PALAVRA SECRETA!!!!          *+150 pontos*\nAperte <ENTER> para continuar. ");
                        pontos += 150;
                        jogador.pontuacao += 150;
                        acertosSecretas++;
                    }
                    else if (sec == -1)
                    {
                        printf("\nPalavra secreta ja encontrada!\nAperte <ENTER> para continuar. ");
                    }
                    else
                    {

                        printf("\nPalavra inexistente!          *-10 pontos*\nAperte <ENTER> para continuar. ");
                        pontos -= 10;
                        jogador.pontuacao -= 10;
                        erros++;
                    }

                    getchar();
                    getchar();
                }
            }

            // FASE CONCLUÍDA
            system(CLEAR);
            printf("=============================\n");
            printf("       FASE %02d CONCLUIDA   \n", f + 1);
            printf("       Feito por: joaopnr     \n");
            printf("=============================\n\n");

            printf("Parabens! Voce acertou todas as palavras!!     *+25* pontos\n\n");
            jogador.pontuacao += 25;
            pontos += 25;

            printf("Acertos: %d | Erros: %d\n", acertos, erros);

            int qtdSecretas = fases[f].qtdSecretas;
            if (acertosSecretas > 0)
            {
                printf("Palavras secretas encontradas: %d/%d\n\n", acertosSecretas, qtdSecretas);
            }

            printf("Pontuacao atual: %d\n\n", pontos);

            if (f < total_fases - 1)
            {
                char continuar[5];
                printf("Digite 'SAIR' para voltar ao menu principal,\n");
                printf("ou qualquer outra coisa para ir para a proxima fase: ");
                scanf("%4s", continuar);

                for (int i = 0; continuar[i] != '\0'; i++)
                {
                    continuar[i] = toupper((unsigned char)continuar[i]);
                }

                if (strcmp(continuar, "SAIR") == 0)
                {
                    system(CLEAR);
                    // SALVAR FASE ATUAL
                    int next = f + 1;
                    if (next >= total_fases)
                        next = 0;
                    write_save(next);
                    printf("===================\n");
                    printf("    FIM DO JOGO    \n");
                    printf(" Feito por: joaopnr     \n");
                    printf("===================\n\n");

                    printf("Voce saiu antes de concluir todas as fases.\n");
                    printf("Pontuacao adquirida: +%d\n", pontos);
                    printf("Sua pontuacao total atual: %d\n\n", jogador.pontuacao);
                    printf("Aperte <ENTER> para voltar ao menu principal. ");

                    salvarRanking(jogador);

                    getchar();
                    getchar();
                    return;
                }
            }
        }

    // TODAS AS FASES FORAM CONCLUIDAS
    system(CLEAR);
    printf("=============================\n");
    printf("      FIM DO JOGO, %s!\n", jogador.nickname);
    printf("      Feito por: joaopnr     \n");
    printf("=============================\n\n");

    printf("Voce concluiu todas as fases!\n");
    printf("Pontuacao adquirida: +%d\n", pontos);
    printf("Sua pontuacao total atual: %d\n\n", jogador.pontuacao);
    printf("Aperte <ENTER> para voltar ao menu principal. ");

    salvarRanking(jogador);
    write_save(0);
    getchar();
}

// CONFIGURACOES
void configuracoes()
{
    int escolha;
    const char *mensagemErroConfig = NULL;

    while (1)
    {
        system(CLEAR);
        printf("=============================\n");
        printf("        CONFIGURACOES        \n");
        printf("      Feito por: joaopnr     \n");
        printf("=============================\n\n");

        printf("1 - Zerar ranking\n");
        printf("2 - Reiniciar fases\n");
        printf("3 - Voltar ao menu principal\n\n");

        if (mensagemErroConfig != NULL)
        {
            printf("%s\n\n", mensagemErroConfig);
        }

        printf("Digite a opcao desejada: ");
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
            system(CLEAR);
            printf("=============================\n");
            printf("        ZERAR RANKING        \n");
            printf("=============================\n\n");
            printf("Tem certeza que deseja resetar o ranking? [S/N] ");
            char escolharanking;
            scanf(" %1c", &escolharanking);
            escolharanking = toupper(escolharanking);
            if (escolharanking == 'S')
            {
                rank = fopen(RANK_FILE, "wb");
                fclose(rank);
                printf("\nRanking resetado com sucesso!\n");
            }
            else
            {
                printf("\nNada aconteceu.\n");
            }
            printf("Aperte <ENTER> para voltar as configuracoes. ");
            getchar();
            getchar();
            break;

        case 2:
            system(CLEAR);
            printf("=============================\n");
            printf("       REINICIAR FASES       \n");
            printf("=============================\n\n");
            printf("Essa opcao faz com que o seu progresso seja excluido (sua fase atual e pontuacao, alem de te remover do ranking).\n\n");
            printf("Tem certeza que deseja reiniciar as fases? [S/N] ");
            char escolhafases;
            scanf(" %1c", &escolhafases);
            escolhafases = toupper(escolhafases);
            if (escolhafases == 'S')
            {
                s = fopen(SAVE_FILE, "wb+");
                if (s)
                {
                    write_save(0);
                    fclose(s);
                }
                jogador.pontuacao = 0;
                if (jogador.nickname[0] != '\0')
                {
                    removerDoRanking(jogador.nickname);
                }
                printf("\nProgresso resetado com sucesso!\n");
            }
            else
            {
                printf("\nNada aconteceu.\n");
            }
            printf("Aperte <ENTER> para voltar as configuracoes. ");
            getchar();
            getchar();
            break;

        case 3:
            return;

        default:
            mensagemErroConfig = "Opcao invalida. Tente novamente!";
            break;
        }
    }
}

// LOOPING DO MENU PRINCIPAL

void menuLoop()
{
    int escolha;
    const char *mensagemErroMenu = NULL;

    while (1)
    {
        system(CLEAR);
        escreverMenu(mensagemErroMenu);
        mensagemErroMenu = NULL;
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
            jogar();
            break;
        case 2:
            configuracoes();
            break;
        case 3:
            instrucoes();
            break;
        case 4:
            ranking();
            break;
        case 5:
            write_save(0);
            system(CLEAR);
            printf("=============================\n            WORDZ            \n      Feito por: joaopnr     \n=============================\n\nObrigado por jogar %s!\n\nSua pontuacao final foi de: %d pontos\n\n", jogador.nickname, jogador.pontuacao);
            exit(0);
        default:
            mensagemErroMenu = "Opcao invalida. Tente novamente!";
            break;
        }
    }
}

int main()
{
    s = fopen(SAVE_FILE, "wb+");
    if (s)
    {
        write_save(0);
        fclose(s);
    }
    primeiratela();
    menuLoop();
    return 0;
}
