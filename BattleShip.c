#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Definições de constantes
#define BOARD_SIZE 10
#define SHIP_SIZE 3
#define WATER 0
#define SHIP 3
#define HIT 1
#define MISS 2
#define CONE_EFFECT 4
#define CROSS_EFFECT 5
#define OCTAHEDRON_EFFECT 6

// Estrutura para representar uma coordenada
typedef struct {
    int row;
    int col;
} Coordinate;

// Estrutura para habilidades especiais
typedef struct {
    char name[30];
    int effectType;
    int range;
    char symbol;
} SpecialAbility;

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

// Função para aplicar área de efeito em CONE
void applyConeEffect(int effectBoard[BOARD_SIZE][BOARD_SIZE], int originRow, int originCol, int range, char direction) {
    int i, j;
    
    printf("Aplicando efeito CONE com alcance %d na direção %c...\n", range, direction);
    
    // Aplica o efeito baseado na direção
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            bool inCone = false;
            int distance = 0;
            
            switch (direction) {
                case 'N': // Norte (para cima)
                    if (i <= originRow) {
                        distance = originRow - i;
                        if (distance <= range && abs(j - originCol) <= distance) {
                            inCone = true;
                        }
                    }
                    break;
                case 'S': // Sul (para baixo)
                    if (i >= originRow) {
                        distance = i - originRow;
                        if (distance <= range && abs(j - originCol) <= distance) {
                            inCone = true;
                        }
                    }
                    break;
                case 'E': // Leste (para direita)
                    if (j >= originCol) {
                        distance = j - originCol;
                        if (distance <= range && abs(i - originRow) <= distance) {
                            inCone = true;
                        }
                    }
                    break;
                case 'W': // Oeste (para esquerda)
                    if (j <= originCol) {
                        distance = originCol - j;
                        if (distance <= range && abs(i - originRow) <= distance) {
                            inCone = true;
                        }
                    }
                    break;
            }
            
            if (inCone) {
                effectBoard[i][j] = CONE_EFFECT;
            }
        }
    }
}

// Função para aplicar área de efeito em CRUZ
void applyCrossEffect(int effectBoard[BOARD_SIZE][BOARD_SIZE], int originRow, int originCol, int range) {
    int i;
    
    printf("Aplicando efeito CRUZ com alcance %d...\n", range);
    
    // Aplica efeito na linha horizontal
    for (i = 0; i < BOARD_SIZE; i++) {
        if (abs(i - originCol) <= range) {
            effectBoard[originRow][i] = CROSS_EFFECT;
        }
    }
    
    // Aplica efeito na linha vertical
    for (i = 0; i < BOARD_SIZE; i++) {
        if (abs(i - originRow) <= range) {
            effectBoard[i][originCol] = CROSS_EFFECT;
        }
    }
}

// Função para aplicar área de efeito em OCTAEDRO (losango 3D)
void applyOctahedronEffect(int effectBoard[BOARD_SIZE][BOARD_SIZE], int originRow, int originCol, int range) {
    int i, j;
    
    printf("Aplicando efeito OCTAEDRO com alcance %d...\n", range);
    
    // Aplica efeito em formato de losango (distância manhattan)
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            int manhattanDistance = abs(i - originRow) + abs(j - originCol);
            if (manhattanDistance <= range) {
                effectBoard[i][j] = OCTAHEDRON_EFFECT;
            }
        }
    }
}

// Função para combinar tabuleiro principal com efeitos
void combineBoards(int mainBoard[BOARD_SIZE][BOARD_SIZE], int effectBoard[BOARD_SIZE][BOARD_SIZE], int resultBoard[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    
    // Copia o tabuleiro principal
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            resultBoard[i][j] = mainBoard[i][j];
        }
    }
    
    // Aplica os efeitos onde não há navios
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (effectBoard[i][j] != WATER && mainBoard[i][j] == WATER) {
                resultBoard[i][j] = effectBoard[i][j];
            } else if (effectBoard[i][j] != WATER && mainBoard[i][j] == SHIP) {
                // Navio atingido por habilidade especial
                resultBoard[i][j] = HIT;
            }
        }
    }
}

// Função para exibir o tabuleiro com símbolos especiais
void displayBoardWithEffects(int board[BOARD_SIZE][BOARD_SIZE]) {
    int i, j;
    
    printf("\n=== TABULEIRO COM EFEITOS ESPECIAIS ===\n");
    printf("   ");
    
    // Exibe números das colunas (0-9)
    for (j = 0; j < BOARD_SIZE; j++) {
        printf("%2d ", j);
    }
    printf("\n");
    
    // Exibe cada linha do tabuleiro
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%2d ", i); // Número da linha
        
        // Exibe cada coluna da linha atual com símbolos
        for (j = 0; j < BOARD_SIZE; j++) {
            char symbol;
            switch (board[i][j]) {
                case WATER: symbol = '~'; break;           // Água
                case SHIP: symbol = '#'; break;            // Navio
                case HIT: symbol = 'X'; break;             // Navio atingido
                case MISS: symbol = 'O'; break;            // Tiro na água
                case CONE_EFFECT: symbol = '^'; break;     // Efeito cone
                case CROSS_EFFECT: symbol = '+'; break;    // Efeito cruz
                case OCTAHEDRON_EFFECT: symbol = '*'; break; // Efeito octaedro
                default: symbol = '?'; break;
            }
            printf("%2c ", symbol);
        }
        printf("\n");
    }
    
    printf("\nLegenda:\n");
    printf("~ = Água        # = Navio       X = Navio Atingido\n");
    printf("^ = Cone        + = Cruz        * = Octaedro\n\n");
}

// Função para exibir o tabuleiro básico
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

// Função para demonstrar habilidades especiais
void demonstrateSpecialAbilities(int mainBoard[BOARD_SIZE][BOARD_SIZE]) {
    int effectBoard[BOARD_SIZE][BOARD_SIZE];
    int resultBoard[BOARD_SIZE][BOARD_SIZE];
    
    printf("\n=== DEMONSTRAÇÃO DE HABILIDADES ESPECIAIS ===\n\n");
    
    // 1. Habilidade CONE
    printf("1. HABILIDADE: CONE DE FOGO\n");
    printf("Origem: (5, 3) - Direção: Norte - Alcance: 3\n");
    initializeBoard(effectBoard);
    applyConeEffect(effectBoard, 5, 3, 3, 'N');
    combineBoards(mainBoard, effectBoard, resultBoard);
    displayBoardWithEffects(resultBoard);
    
    printf("Pressione Enter para continuar...\n");
    getchar();
    
    // 2. Habilidade CRUZ
    printf("2. HABILIDADE: BOMBARDEIO CRUZADO\n");
    printf("Origem: (4, 6) - Alcance: 2\n");
    initializeBoard(effectBoard);
    applyCrossEffect(effectBoard, 4, 6, 2);
    combineBoards(mainBoard, effectBoard, resultBoard);
    displayBoardWithEffects(resultBoard);
    
    printf("Pressione Enter para continuar...\n");
    getchar();
    
    // 3. Habilidade OCTAEDRO
    printf("3. HABILIDADE: EXPLOSÃO SÔNICA\n");
    printf("Origem: (7, 7) - Alcance: 2\n");
    initializeBoard(effectBoard);
    applyOctahedronEffect(effectBoard, 7, 7, 2);
    combineBoards(mainBoard, effectBoard, resultBoard);
    displayBoardWithEffects(resultBoard);
    
    printf("Pressione Enter para continuar...\n");
    getchar();
    
    // 4. Combinação de múltiplas habilidades
    printf("4. COMBINAÇÃO: MÚLTIPLAS HABILIDADES\n");
    printf("Cone (2,8,S,2) + Cruz (6,2,3) + Octaedro(8,5,1)\n");
    initializeBoard(effectBoard);
    
    // Aplica cone
    applyConeEffect(effectBoard, 2, 8, 2, 'S');
    // Aplica cruz (sobrepõe onde necessário)
    applyCrossEffect(effectBoard, 6, 2, 3);
    // Aplica octaedro (sobrepõe onde necessário)  
    applyOctahedronEffect(effectBoard, 8, 5, 1);
    
    combineBoards(mainBoard, effectBoard, resultBoard);
    displayBoardWithEffects(resultBoard);
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
    
    printf("=== BATALHA NAVAL: HABILIDADES ESPECIAIS COM ÁREAS DE EFEITO ===\n");
    printf("Inicializando tabuleiro %dx%d...\n", BOARD_SIZE, BOARD_SIZE);
    printf("Posicionando 4 navios (2 ortogonais + 2 diagonais)...\n\n");
    
    // 1. Inicializar o tabuleiro
    initializeBoard(board);
    printf("✓ Tabuleiro inicializado com sucesso!\n");
    
    // 2. Posicionar navios
    printf("\nPosicionando navios...\n");
    
    if (placeShip(board, ship1, ship1Row, ship1Col, ship1Orientation) &&
        placeShip(board, ship2, ship2Row, ship2Col, ship2Orientation) &&
        placeShip(board, ship3, ship3Row, ship3Col, ship3Orientation) &&
        placeShip(board, ship4, ship4Row, ship4Col, ship4Orientation)) {
        
        printf("✓ Todos os navios posicionados com sucesso!\n");
        
        // Exibe tabuleiro inicial
        displayBoard(board);
        
        printf("Pressione Enter para iniciar demonstração das habilidades...\n");
        getchar();
        
        // Demonstra as habilidades especiais
        demonstrateSpecialAbilities(board);
        
    } else {
        printf("✗ Falha ao posicionar os navios!\n");
        return 1;
    }
    
    printf("\n🎯 Demonstração das habilidades especiais concluída!\n");
    printf("\n📍 Habilidades implementadas:\n");
    printf("• CONE: Área triangular em uma direção específica\n");
    printf("• CRUZ: Linhas horizontais e verticais cruzadas\n");
    printf("• OCTAEDRO: Área em formato de losango (distância Manhattan)\n");
    printf("• COMBINAÇÃO: Múltiplas habilidades aplicadas simultaneamente\n");
    
    return 0;
}