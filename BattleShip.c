#include <stdio.h>
#include <stdbool.h>

// Definições de constantes
#define BOARD_SIZE 10
#define SHIP_SIZE 3
#define WATER 0
#define SHIP 3

// Função para inicializar o tabuleiro com água (0)
void initializeBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    
    // Percorre todas as posições do tabuleiro
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = WATER; // Inicializa com água
        }
    }
}

// Função para validar se as coordenadas estão dentro dos limites do tabuleiro
bool validateCoordinates(int row, int col, int size, bool horizontal) {
    // Verifica se a posição inicial está dentro dos limites
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return false;
    }
    
    // Verifica se o navio cabe no tabuleiro na orientação especificada
    if (horizontal) {
        // Navio horizontal: verifica se cabe na linha
        return (col + size <= BOARD_SIZE);
    } else {
        // Navio vertical: verifica se cabe na coluna
        return (row + size <= BOARD_SIZE);
    }
}

// Função para verificar se há sobreposição com outros navios
bool checkOverlap(int board[BOARD_SIZE][BOARD_SIZE], 
                  int row, int col, int size, bool horizontal) {
    int i;
    
    // Verifica cada posição que o navio ocupará
    for (i = 0; i < size; i++) {
        if (horizontal) {
            // Navio horizontal: verifica posições na mesma linha
            if (board[row][col + i] != WATER) {
                return true; // Há sobreposição
            }
        } else {
            // Navio vertical: verifica posições na mesma coluna
            if (board[row + i][col] != WATER) {
                return true; // Há sobreposição
            }
        }
    }
    
    return false; // Não há sobreposição
}

// Função para posicionar um navio no tabuleiro
bool placeShip(int board[BOARD_SIZE][BOARD_SIZE], 
               int ship[SHIP_SIZE], int row, int col, bool horizontal) {
    int i;
    
    // Valida as coordenadas
    if (!validateCoordinates(row, col, SHIP_SIZE, horizontal)) {
        printf("Erro: Coordenadas inválidas para o navio!\n");
        return false;
    }
    
    // Verifica sobreposição
    if (checkOverlap(board, row, col, SHIP_SIZE, horizontal)) {
        printf("Erro: Navio se sobrepõe a outro navio!\n");
        return false;
    }
    
    // Posiciona o navio no tabuleiro
    for (i = 0; i < SHIP_SIZE; i++) {
        if (horizontal) {
            // Navio horizontal: posiciona na mesma linha
            board[row][col + i] = ship[i];
        } else {
            // Navio vertical: posiciona na mesma coluna
            board[row + i][col] = ship[i];
        }
    }
    
    return true; // Navio posicionado com sucesso
}

// Função para exibir o tabuleiro no console
void displayBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    
    printf("\n=== TABULEIRO DE BATALHA NAVAL ===\n");
    printf("   ");
    
    // Exibe números das colunas (0-9)
    for (j = 0; j < BOARD_SIZE; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    
    // Exibe cada linha do tabuleiro
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%2d ", i); // Número da linha
        
        // Exibe cada coluna da linha atual
        for (j = 0; j < BOARD_SIZE; j++) {
            printf("%2d ", board[i][j]);
        }
        printf("\n");
    }
    
    printf("\nLegenda: 0 = Água, 3 = Navio\n\n");
}

int main() {
    // Declaração das variáveis
    int board[BOARD_SIZE][BOARD_SIZE];
    int ship1[SHIP_SIZE] = {SHIP, SHIP, SHIP}; // Navio horizontal
    int ship2[SHIP_SIZE] = {SHIP, SHIP, SHIP}; // Navio vertical
    
    // Coordenadas dos navios (definidas no código)
    int ship1_row = 2, ship1_col = 1; // Navio horizontal na linha 2, a partir da coluna 1
    int ship2_row = 5, ship2_col = 7; // Navio vertical na coluna 7, a partir da linha 5
    
    printf("=== JOGO DE BATALHA NAVAL ===\n");
    printf("Inicializando tabuleiro %dx%d...\n", BOARD_SIZE, BOARD_SIZE);
    
    // 1. Inicializar o tabuleiro
    initializeBoard(board);
    printf("Tabuleiro inicializado com sucesso!\n");
    
    // 2. Posicionar o primeiro navio (horizontal)
    printf("\nPosicionando navio 1 (horizontal) na linha %d, coluna %d...\n", 
           ship1_row, ship1_col);
    
    if (placeShip(board, ship1, ship1_row, ship1_col, true)) {
        printf("Navio 1 posicionado com sucesso!\n");
    } else {
        printf("Falha ao posicionar navio 1!\n");
        return 1;
    }
    
    // 3. Posicionar o segundo navio (vertical)
    printf("\nPosicionando navio 2 (vertical) na linha %d, coluna %d...\n", 
           ship2_row, ship2_col);
    
    if (placeShip(board, ship2, ship2_row, ship2_col, false)) {
        printf("Navio 2 posicionado com sucesso!\n");
    } else {
        printf("Falha ao posicionar navio 2!\n");
        return 1;
    }
    
    // 4. Exibir o tabuleiro final
    printf("\n=== POSICIONAMENTO CONCLUÍDO ===\n");
    displayBoard(board);
    
    printf("Programa executado com sucesso!\n");
    printf("Navios posicionados:\n");
    printf("- Navio 1: Horizontal, linha %d, colunas %d-%d\n", 
           ship1_row, ship1_col, ship1_col + SHIP_SIZE - 1);
    printf("- Navio 2: Vertical, coluna %d, linhas %d-%d\n", 
           ship2_col, ship2_row, ship2_row + SHIP_SIZE - 1);
    
    return 0;
}