#include <stdio.h>
#include <stdbool.h>

// Definições de constantes
#define BOARD_SIZE 10
#define SHIP_SIZE 3
#define WATER 0
#define SHIP 3

// Função para retornar o nome da orientação
const char* getOrientationName(char orientation) {
    switch (orientation) {
        case 'H': return "Horizontal";
        case 'V': return "Vertical";
        case 'D': return "Diagonal Principal (↘)";
        case 'A': return "Diagonal Anti-Principal (↙)";
        default: return "Desconhecida";
    }
}

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
bool validateCoordinates(int row, int col, int size, char orientation) {
    // Verifica se a posição inicial está dentro dos limites
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return false;
    }
    
    // Verifica se o navio cabe no tabuleiro na orientação especificada
    switch (orientation) {
        case 'H': // Horizontal
            return (col + size <= BOARD_SIZE);
        case 'V': // Vertical
            return (row + size <= BOARD_SIZE);
        case 'D': // Diagonal principal (↘)
            return (row + size <= BOARD_SIZE && col + size <= BOARD_SIZE);
        case 'A': // Diagonal anti-principal (↙)
            return (row + size <= BOARD_SIZE && col - size + 1 >= 0);
        default:
            return false;
    }
}

// Função para verificar se há sobreposição com outros navios
bool checkOverlap(int board[BOARD_SIZE][BOARD_SIZE], 
                  int row, int col, int size, char orientation) {
    int i;
    
    // Verifica cada posição que o navio ocupará
    for (i = 0; i < size; i++) {
        int checkRow, checkCol;
        
        switch (orientation) {
            case 'H': // Horizontal
                checkRow = row;
                checkCol = col + i;
                break;
            case 'V': // Vertical
                checkRow = row + i;
                checkCol = col;
                break;
            case 'D': // Diagonal principal (↘)
                checkRow = row + i;
                checkCol = col + i;
                break;
            case 'A': // Diagonal anti-principal (↙)
                checkRow = row + i;
                checkCol = col - i;
                break;
            default:
                return true; // Orientação inválida
        }
        
        // Verifica se a posição está ocupada
        if (board[checkRow][checkCol] != WATER) {
            return true; // Há sobreposição
        }
    }
    
    return false; // Não há sobreposição
}

// Função para posicionar um navio no tabuleiro
bool placeShip(int board[BOARD_SIZE][BOARD_SIZE], 
               int ship[SHIP_SIZE], int row, int col, char orientation) {
    int i;
    
    // Valida as coordenadas
    if (!validateCoordinates(row, col, SHIP_SIZE, orientation)) {
        printf("Erro: Coordenadas inválidas para o navio!\n");
        return false;
    }
    
    // Verifica sobreposição
    if (checkOverlap(board, row, col, SHIP_SIZE, orientation)) {
        printf("Erro: Navio se sobrepõe a outro navio!\n");
        return false;
    }
    
    // Posiciona o navio no tabuleiro
    for (i = 0; i < SHIP_SIZE; i++) {
        int placeRow, placeCol;
        
        switch (orientation) {
            case 'H': // Horizontal
                placeRow = row;
                placeCol = col + i;
                break;
            case 'V': // Vertical
                placeRow = row + i;
                placeCol = col;
                break;
            case 'D': // Diagonal principal (↘)
                placeRow = row + i;
                placeCol = col + i;
                break;
            case 'A': // Diagonal anti-principal (↙)
                placeRow = row + i;
                placeCol = col - i;
                break;
            default:
                printf("Erro: Orientação inválida!\n");
                return false;
        }
        
        board[placeRow][placeCol] = ship[i];
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
    int ship1[SHIP_SIZE] = {SHIP, SHIP, SHIP}; // Navio 1
    int ship2[SHIP_SIZE] = {SHIP, SHIP, SHIP}; // Navio 2
    int ship3[SHIP_SIZE] = {SHIP, SHIP, SHIP}; // Navio 3
    int ship4[SHIP_SIZE] = {SHIP, SHIP, SHIP}; // Navio 4
    
    // Definição dos navios: {linha, coluna, orientação}
    int ship1Row = 1, ship1Col = 2; 
    char ship1Orientation = 'H'; // Horizontal
    int ship2Row = 4, ship2Col = 8; 
    char ship2Orientation = 'V'; // Vertical
    int ship3Row = 6, ship3Col = 1; 
    char ship3Orientation = 'D'; // Diagonal principal
    int ship4Row = 2, ship4Col = 7; 
    char ship4Orientation = 'A'; // Diagonal anti-principal
    
    printf("=== JOGO DE BATALHA NAVAL - NÍVEL INTERMEDIÁRIO ===\n");
    printf("Inicializando tabuleiro %dx%d...\n", BOARD_SIZE, BOARD_SIZE);
    printf("Posicionando 4 navios (2 ortogonais + 2 diagonais)...\n\n");
    
    // 1. Inicializar o tabuleiro
    initializeBoard(board);
    printf("✓ Tabuleiro inicializado com sucesso!\n");
    
    // 2. Posicionar o primeiro navio (horizontal)
    printf("\nPosicionando navio 1 (%s) na linha %d, coluna %d...\n", 
           getOrientationName(ship1Orientation), ship1Row, ship1Col);
    
    if (placeShip(board, ship1, ship1Row, ship1Col, ship1Orientation)) {
        printf("✓ Navio 1 posicionado com sucesso!\n");
    } else {
        printf("✗ Falha ao posicionar navio 1!\n");
        return 1;
    }
    
    // 3. Posicionar o segundo navio (vertical)
    printf("\nPosicionando navio 2 (%s) na linha %d, coluna %d...\n", 
           getOrientationName(ship2Orientation), ship2Row, ship2Col);
    
    if (placeShip(board, ship2, ship2Row, ship2Col, ship2Orientation)) {
        printf("✓ Navio 2 posicionado com sucesso!\n");
    } else {
        printf("✗ Falha ao posicionar navio 2!\n");
        return 1;
    }
    
    // 4. Posicionar o terceiro navio (diagonal principal)
    printf("\nPosicionando navio 3 (%s) na linha %d, coluna %d...\n", 
           getOrientationName(ship3Orientation), ship3Row, ship3Col);
    
    if (placeShip(board, ship3, ship3Row, ship3Col, ship3Orientation)) {
        printf("✓ Navio 3 posicionado com sucesso!\n");
    } else {
        printf("✗ Falha ao posicionar navio 3!\n");
        return 1;
    }
    
    // 5. Posicionar o quarto navio (diagonal anti-principal)
    printf("\nPosicionando navio 4 (%s) na linha %d, coluna %d...\n", 
           getOrientationName(ship4Orientation), ship4Row, ship4Col);
    
    if (placeShip(board, ship4, ship4Row, ship4Col, ship4Orientation)) {
        printf("✓ Navio 4 posicionado com sucesso!\n");
    } else {
        printf("✗ Falha ao posicionar navio 4!\n");
        return 1;
    }
    
    // 6. Exibir o tabuleiro final
    printf("\n=== POSICIONAMENTO CONCLUÍDO ===\n");
    displayBoard(board);
    
    printf("🎯 Programa executado com sucesso!\n");
    printf("\n📍 Resumo dos navios posicionados:\n");
    printf("• Navio 1: %s, linha %d, colunas %d-%d\n", 
           getOrientationName(ship1Orientation), ship1Row, ship1Col, ship1Col + SHIP_SIZE - 1);
    printf("• Navio 2: %s, coluna %d, linhas %d-%d\n", 
           getOrientationName(ship2Orientation), ship2Col, ship2Row, ship2Row + SHIP_SIZE - 1);
    printf("• Navio 3: %s, posições (%d,%d) a (%d,%d)\n", 
           getOrientationName(ship3Orientation), ship3Row, ship3Col, 
           ship3Row + SHIP_SIZE - 1, ship3Col + SHIP_SIZE - 1);
    printf("• Navio 4: %s, posições (%d,%d) a (%d,%d)\n", 
           getOrientationName(ship4Orientation), ship4Row, ship4Col, 
           ship4Row + SHIP_SIZE - 1, ship4Col - SHIP_SIZE + 1);
    
    return 0;
}