# 🔠 Wordz - Jogo em C

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)

![Status](https://img.shields.io/badge/Status-Desenvolvimento_Completo-green?style=for-the-badge)

O **Wordz** é um jogo de terminal inspirado no *Wordle*, desenvolvido como projeto prático da matéria de **Algoritmos e Programação de Computadores na UnB**. O foco principal foi aplicar conceitos de manipulação de memória e lógica de strings.

## 🚀 Funcionalidades
- Fases fixas com diversas palavras para serem descobertas (Algumas palavras "secretas" concedem mais pontos!);
- Sistema de tentativas baseado na sua pontuação atual (Se chegar a pontos negativos você perde);
- Feedback visual no terminal para palavras corretas e quantidade restante de palavras para avançar de fase;
- Ranking local com nome e pontuação de cada jogador.

## 🛠️ Como Compilar e Rodar

*É necessário baixar o arquivo [fases.txt](./fases.txt) para que o jogo funcione corretamente.*

Certifique-se de ter o **GCC** instalado. No terminal, execute:

```bash
# Compilar
gcc main.c -o wordz

# Executar
./wordz
