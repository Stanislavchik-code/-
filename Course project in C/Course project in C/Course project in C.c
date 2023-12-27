#include <locale.h>
#include <stdio.h>
#include <time.h>

#define N 10
#define LIGHT_FIELD_COUNT 12
#define AVERAGE_FIELD_COUNT 20
#define HARD_FIELD_COUNT 28

int RulesGame();
int GameMenu();
int FieldGeneration(int(*pGameArray)[N], int fieldCount);
int OpenField(int(*pGameArray)[N], int ControlField);
void PlayingField(int(*pS)[N]);
int Game(int(*pGameArray)[N]);
_Bool DFSZeroGroups(int(*pGameArray)[N], _Bool visited[N][N], int row, int col);
_Bool CheckZeroGroups(int(*pGameArray)[N]);
_Bool CheckNegativePairs(int(*pGameArray)[N]);
_Bool CheckWin(int(*pGameArray)[N]);
_Bool CheckGameOver(int(*pGameArray)[N]);
void DisplayGameOver(int(*pGameArray)[N]);
void SaveEnd(int(*pGameArray)[N]);

// Главное меню
int main() {
    system("cls");

    setlocale(LC_ALL, "RUS");
    srand(time(NULL));

    int menu;

    while (1) {
        printf("\n+---------------------------|Курсовой Проект|----------------------------+\n");
        printf("|Тема: Разработка игры-головоломки Ход                                   |\n");
        printf("|Выполнил: Лынов Станислав Михайлович                                    |\n");
        printf("|Факультет: ФИТКБ                                                        |\n");
        printf("|Группа: бИСТ-231                                                        |\n");
        printf("|Преподаватель: Минакова Ольга Владимировна                              |\n");
        printf("+------------------------------------------------------------------------+\n\n");

        printf("+-----------------------------|Главное меню|-----------------------------+\n");
        printf("|Выберете действие:                                                      |\n");
        printf("|1. Ознакомится с правилами игры-головоломки                             |\n");
        printf("|2. Приступить к игре-головоломке                                        |\n");
        printf("|3. Выйти из программы                                                   |\n");
        printf("+------------------------------------------------------------------------+\n\n");

        printf("Введите номер действия: ");
        if (scanf("%d", &menu) != 1) {
            continue;
        }

        switch (menu) {
        case 1:
            RulesGame();
            break;
        case 2:
            GameMenu();
            break;
        case 3:
            printf("Выход из программы\n");
            return 0;
        default:
            printf("Неверный выбор");
            break;
        }
    }

    return 0;
}

// Правила игры
int RulesGame() {
    system("cls");

    FILE* Rules = fopen("RulesGame.txt", "r");

    if (Rules == NULL) {
        system("cls");
        perror("Ошибка открытия файла RulesGame.txt");
        return 1;
    }

    int c = 0;

    while ((c = fgetc(Rules)) != EOF) {
        char symbol = (char)c;
        printf("%c", symbol);
    }

    fclose(Rules);

    char xxx;
    scanf("%c", &xxx);
    getchar();

    system("cls");
}

// Игровое меню
int GameMenu() {
    system("cls");

    int option;
    int GameArray[N][N];

    printf("+---------------------------------|Меню|---------------------------------+\n");
    printf("|Выберите действие:                                                      |\n");
    printf("|1. Загрузить поле из файла                                              |\n");
    printf("|2. Начать с легким уровнем сложности                                    |\n");
    printf("|3. Начать со средним уровнем сложности                                  |\n");
    printf("|4. Начать со сложным уровнем сложности                                  |\n");
    printf("|5. Открыть контрольный пример                                           |\n");
    printf("|6. Вернуться в главное меню                                             |\n");
    printf("+------------------------------------------------------------------------+\n\n");

    while (1) {
        printf("Введите номер действия: ");
        if (scanf("%d", &option) != 1) {
            printf("Неверный ввод. Пожалуйста, введите число.\n");
            return 1;
        }

        switch (option) {
        case 1:
            OpenField(GameArray, 0);
            break;
        case 2:
            FieldGeneration(GameArray, LIGHT_FIELD_COUNT);
            break;
        case 3:
            FieldGeneration(GameArray, AVERAGE_FIELD_COUNT);
            break;
        case 4:
            FieldGeneration(GameArray, HARD_FIELD_COUNT);
            break;
        case 5:
            OpenField(GameArray, 1);
            break;
        case 6:
            printf("Возвращение в главное меню...\n");
            system("cls");
            return 0;
        default:
            printf("Неверный выбор\n");
            break;
        }
    }

    return 0;
}

// Открытие игрового поля из файла
int OpenField(int(*pGameArray)[N], int ControlField) {
    char filename[100];

    if (ControlField) {
        strcpy(filename, "ConstantField.txt");
    }
    else {
        printf("Введите имя файла для загрузки игрового поля (например Game.txt): ");
        scanf("%s", filename);
    }

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Ошибка открытия файла %s для чтения.\n", filename);
        return 1;
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (fscanf(file, "%d", &pGameArray[i][j]) != 1) {
                perror("Ошибка чтения из файла. Проверьте формат файла.\n");
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);

    Game(*pGameArray);

    return 0;
}

// Генерация игрового поля от уровня сложности
int FieldGeneration(int(*pGameArray)[N], int fieldCount) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            pGameArray[i][j] = 0;
        }
    }

    int count = 0;
    while (count < fieldCount) {
        int x = rand() % N;
        int y = rand() % N;
        if (pGameArray[x][y] == 0) {
            pGameArray[x][y] = rand() % 7 + 1;
            count++;
        }
    }

    Game(*pGameArray);

    return 0;
}

// Контур игрового поля
void PlayingField(int(*pS)[N]) {
    printf("\n    ");
    for (int i = 0; i < N; i++) {
        printf("%d   ", i);
    }
    printf("\n");
    printf("  +");
    for (int i = 0; i < N; i++) {
        printf("---+");
    }
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%d |", i);
        for (int j = 0; j < N; j++) {
            if (*(pS[i] + j) == -1) {
                printf(" X |");
            }
            else if (*(pS[i] + j) == 0) {
                printf("   |");
            }
            else {
                printf("%- 2d |", *(pS[i] + j));
            }
        }
        printf("\n  +");
        for (int j = 0; j < N; j++) {
            printf("---+");
        }
        printf("\n");
    }
}

// Работа с игровым полем (игра)
int Game(int(*pGameArray)[N]) {
    system("cls");

    PlayingField(*pGameArray);

    int x, y, moveX, moveY, choice;

    while (1) {
        printf("Введите координаты ячейки, которую нужно передвинуть (x y): ");
        if (scanf("%d %d", &x, &y) != 2 || x < 0 || x >= N || y < 0 || y >= N) {
            printf("Неверные координаты. Пожалуйста, введите два числа от 0 до %d.\n", N - 1);
            while (getchar() != '\n');
            continue;
        }

        if (pGameArray[x][y] == 0) {
            printf("Нельзя передвигать пустую ячейку.\n");
            continue;
        }

        if (pGameArray[x][y] == -1) {
            printf("Нельзя перемещать символ 'X'.\n");
            continue;
        }

        printf("Введите направление для перемещения (1 - вверх, 2 - вниз, 3 - влево, 4 - вправо): ");
        int direction;
        if (scanf("%d", &direction) != 1 || direction < 1 || direction > 4) {
            printf("Неверное направление. Пожалуйста, введите число от 1 до 4.\n");
            while (getchar() != '\n');
            continue;
        }

        moveX = x;
        moveY = y;

        if (direction == 1 && x > 0 && (*(pGameArray + x - 1))[y] == 0 || (*(pGameArray + x - 1))[y] == -1) {
            moveX = x - 1;
        }
        else if (direction == 2 && x < N - 1 && (*(pGameArray + x + 1))[y] == 0 || (*(pGameArray + x + 1))[y] == -1) {
            moveX = x + 1;
        }
        else if (direction == 3 && y > 0 && (*(pGameArray + x))[y - 1] == 0 || (*(pGameArray + x))[y - 1] == -1) {
            moveY = y - 1;
        }
        else if (direction == 4 && y < N - 1 && (*(pGameArray + x))[y + 1] == 0 || (*(pGameArray + x))[y + 1] == -1) {
            moveY = y + 1;
        }
        else {
            printf("Неверное направление или нельзя передвигать в эту ячейку.\n");
            continue;
        }

        if (pGameArray[moveX][moveY] == 0) {
            int newValue = pGameArray[x][y] - 1;
            pGameArray[moveX][moveY] = (newValue == 0) ? -1 : newValue;
            pGameArray[x][y] = 0;
        }
        else {
            printf("Нельзя перемещать числа в ячейку с другим числом.\n");
            continue;
        }

        system("cls");
        PlayingField(*pGameArray);

        if (CheckGameOver(*pGameArray)) {
            DisplayGameOver(*pGameArray);
            break;
        }

        do {
            printf("Желаете продолжить? (1 - да, 0 - нет): ");
            if (scanf("%d", &choice) != 1 || choice < 0 || choice > 1) {
                printf("Неверный выбор. Пожалуйста, введите 0 или 1.\n");
                while (getchar() != '\n');
                choice = -1;
            }
        } while (choice < 0 || choice > 1);

        if (choice == 0) {
            SaveEnd(*pGameArray);
            return 0;
        }
    }

    return 0;
}

// Проверка на соприкосновение всех нулей (облпстей нулей) true = 1, false = 0
_Bool CheckZeroGroups(int(*pGameArray)[N]) {
    _Bool visited[N][N] = {0};

    int groupCount = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if ((*(pGameArray + i))[j] == 0 && !visited[i][j]) {
                groupCount++;

                if (!DFSZeroGroups(pGameArray, visited, i, j)) {
                    return 0;
                }
            }
        }
    }

    return groupCount == 1;
}

// Рекурсивный поиск областей соеденённых 0 между собой
_Bool DFSZeroGroups(int(*pGameArray)[N], _Bool visited[N][N], int row, int col) {

    if (row < 0 || col < 0 || row >= N || col >= N) {
        return 0;
    }

    if (visited[row][col]) {
        return 1;
    }

    visited[row][col] = 1;

    if (row - 1 >= 0 && (*(pGameArray + row - 1))[col] == 0 && !DFSZeroGroups(pGameArray, visited, row - 1, col)) {
        return 0;
    }
    if (row + 1 < N && (*(pGameArray + row + 1))[col] == 0 && !DFSZeroGroups(pGameArray, visited, row + 1, col)) {
        return 0;
    }
    if (col - 1 >= 0 && (*(pGameArray + row))[col - 1] == 0 && !DFSZeroGroups(pGameArray, visited, row, col - 1)) {
        return 0;
    }
    if (col + 1 < N && (*(pGameArray + row))[col + 1] == 0 && !DFSZeroGroups(pGameArray, visited, row, col + 1)) {
        return 0;
    }

    return 1;
}

// Проверка на соприкосновение X
_Bool CheckNegativePairs(int(*pGameArray)[N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if ((*(pGameArray + i))[j] == -1) {
                if (i > 0 && (*(pGameArray + i - 1))[j] == -1) {
                    return 0;
                }
                if (i < N - 1 && (*(pGameArray + i + 1))[j] == -1) {
                    return 0;
                }
                if (j > 0 && (*(pGameArray + i))[j - 1] == -1) {
                    return 0;
                }
                if (j < N - 1 && (*(pGameArray + i))[j + 1] == -1) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// Проверка завершения
_Bool CheckGameOver(int(*pGameArray)[N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if ((*(pGameArray + i))[j] != 0 && (*(pGameArray + i))[j] != -1) {
                return 0;
            }
        }
    }
    return 1;
}

// Проверка победы
_Bool CheckWin(int(*pGameArray)[N]) {
    return CheckGameOver(*pGameArray) && CheckZeroGroups(*pGameArray) && CheckNegativePairs(*pGameArray);
}

// Блок вывода победы или поражения
void DisplayGameOver(int(*pGameArray)[N]) {
    int exitChoice;
    if (CheckWin(*pGameArray)) {
        printf("Поздравляем! Вы выиграли!\n");
    }
    else if (CheckGameOver(*pGameArray)) {
        printf("Игра окончена. Вы проиграли, повезёт в следующий раз!\n");
    }
    
    printf("Для завершения нажмите 1: ");
    scanf("%d", &exitChoice);
    if (exitChoice == 1) {
        SaveEnd(*pGameArray);
    }
}

// Сохранение результата по желанию и выход в главное меню
void SaveEnd(int(*GameArray)[N]) {
    int choice;
    int exitChoice;
    printf("Желаете сохранить игру перед выходом? (1 - да, 0 - нет): ");
    if (scanf("%d", &choice) != 1) {
        printf("Неверный ввод. Возвращение в главное меню...\n");
        return;
    }

    if (choice == 1) {
        char filename[100];
        printf("Введите имя файла для сохранения игрового поля (например, SaveGame.txt): ");
        scanf("%s", filename);

        FILE* file = fopen(filename, "w");

        if (file == NULL) {
            perror("Ошибка открытия файла %s для записи.\n", filename);
            return;
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                fprintf(file, "%d ", (*(GameArray + i))[j]);
            }
            fprintf(file, "\n");
        }

        fclose(file);
        printf("Игровое поле успешно сохранено в файле %s\n", filename);

        printf("Для завершения нажмите 1: ");
        scanf("%d", &exitChoice);
    }

    main();
}