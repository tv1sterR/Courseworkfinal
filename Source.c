#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_RECORDS 100

typedef struct {
    char type[20];           // Тип памяти (DDR4)
    int volume; // Объём памяти в ГБ
    int frequency; // Тактовая частота в МГц
    int latency; // Тайминги
    char radiator[5];        // Наличие радиатора ("Yes" или "No")
} MemoryRecord;

int addRecord(MemoryRecord* records, int* recordCount);
void printRecords(const MemoryRecord* records, int recordCount);
int searchRecord(const MemoryRecord* records, int recordCount);
void sortRecords(MemoryRecord* records, int recordCount);
void saveToFile(const MemoryRecord* records, int recordCount);
int loadFromFile(MemoryRecord* records, int* recordCount, const char* filename);
int editRecord(MemoryRecord* records, int recordCount, int index);
int addMultipleRecords(MemoryRecord* records, int* recordCount, int n);
int deleteRecord(MemoryRecord* records, int* recordCount, int index);

int main() {
    setlocale(LC_CTYPE, "RUS");
    MemoryRecord records[MAX_RECORDS];
    int recordCount = 0;
    int choice;

    do {
        printf("\nМеню:\n");
        printf("1. Добавить запись\n");
        printf("2. Показать все записи\n");
        printf("3. Поиск записи\n");
        printf("4. Сортировать записи\n");
        printf("5. Сохранить данные в файл\n");
        printf("6. Загрузить данные из файла\n");
        printf("7. Изменить запись\n");
        printf("8. Добавить несколько записей\n");
        printf("9. Удалить запись\n");
        printf("0. Выход\n");
        printf("Выберите опцию: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addRecord(records, &recordCount);
            break;
        case 2:
            printRecords(records, recordCount);
            break;
        case 3:
            searchRecord(records, recordCount);
            break;
        case 4:
            sortRecords(records, recordCount);
            break;
        case 5:
            saveToFile(records, recordCount);
            break;
        case 6:
            loadFromFile(records, &recordCount, "RAM_records.txt");
            break;
        case 7: {
            printRecords(records, recordCount);
            printf("Введите номер записи для изменения: ");
            int index;
            scanf("%d", &index);
            editRecord(records, recordCount, index - 1);
            break;
        }
        case 8:
            addMultipleRecords(records, &recordCount, 0);
            break;
        case 9: {
            if (recordCount == 0) {
                printf("Нет записей для удаления.\n");
                break;
            }
            printRecords(records, recordCount);
            printf("Введите номер записи для удаления: ");
            int index;
            scanf("%d", &index);
            if (deleteRecord(records, &recordCount, index - 1) == 0) {
                printf("Запись успешно удалена.\n");
            }
            else {
                printf("Ошибка при удалении записи.\n");
            }
            break;
        }
        case 0:
            printf("Выход из программы.\n");
            break;
        default:
            printf("Неверный выбор, попробуйте снова.\n");
        }
    } while (choice != 0);

    return 0;
}

int addRecord(MemoryRecord* records, int* recordCount) {
    if (*recordCount >= MAX_RECORDS) {
        printf("Ошибка: база данных переполнена.\n");
        return -1;
    }

    printf("Введите тип памяти: ");
    scanf("%s", records[*recordCount].type);

    printf("Введите объем памяти (ГБ): ");
    scanf("%d", &records[*recordCount].volume);

    printf("Введите тактовую частоту (МГц): ");
    scanf("%d", &records[*recordCount].frequency);

    printf("Есть ли радиатор (Yes/No): ");
    scanf("%s", records[*recordCount].radiator);

    printf("Введите тайминги: ");
    scanf("%d", &records[*recordCount].latency);

    (*recordCount)++;
    printf("Запись успешно добавлена.\n");
    return 0;
}


void printRecords(const MemoryRecord* records, int recordCount) {
    if (recordCount == 0) {
        printf("База данных пуста.\n");
        return 0;
    }
    for (int i = 0; i < recordCount; i++) {
        printf("%d. Тип: %s, Объем: %d ГБ, Частота: %d МГц, Радиатор: %s, Тайминги: %d\n",
            i + 1, records[i].type, records[i].volume, records[i].frequency, records[i].radiator, records[i].latency);
    }
    return recordCount;
}

int searchRecord(const MemoryRecord* records, int recordCount) {
    char type[20] = "";          
    char minFreqStr[10];         
    char maxFreqStr[10];         
    int minFrequency = 0;        
    int maxFrequency = 0;        

    printf("Введите тип памяти (или нажмите Enter для пропуска): ");
    getchar(); 
    fgets(type, sizeof(type), stdin);
    if (type[strlen(type) - 1] == '\n') {
        type[strlen(type) - 1] = '\0'; 
    }

    printf("Введите минимальную тактовую частоту (или нажмите Enter для пропуска): ");
    fgets(minFreqStr, sizeof(minFreqStr), stdin);
    if (minFreqStr[0] != '\n') {
        minFrequency = atoi(minFreqStr); 
    }

    printf("Введите максимальную тактовую частоту (или нажмите Enter для пропуска): ");
    fgets(maxFreqStr, sizeof(maxFreqStr), stdin);
    if (maxFreqStr[0] != '\n') {
        maxFrequency = atoi(maxFreqStr); 
    }

    printf("\nРезультаты поиска:\n");
    int found = 0;
    for (int i = 0; i < recordCount; i++) {

        int typeMatch = (strlen(type) == 0 || strcmp(records[i].type, type) == 0);
        int minFreqMatch = (minFrequency == 0 || records[i].frequency >= minFrequency);
        int maxFreqMatch = (maxFrequency == 0 || records[i].frequency <= maxFrequency);

        if (typeMatch && minFreqMatch && maxFreqMatch) {
            printf("%d. Тип: %s, Объем: %d ГБ, Частота: %d МГц, Радиатор: %s, Тайминги: %d\n",
                i + 1, records[i].type, records[i].volume, records[i].frequency,
                records[i].radiator, records[i].latency);
            found = 1;
        }
    }

    if (!found) {
        printf("Записи не найдены.\n");
    } else {
        printf("Поиск завершен.\n");
    }
    return found;
}

void sortRecords(MemoryRecord* records, int recordCount) {
    if (recordCount == 0) {
        printf("База данных пуста, сортировать нечего.\n");
        return -1;
    }

    int choice;
    printf("Выберите параметр сортировки:\n");
    printf("1. По объему памяти\n");
    printf("2. По тактовой частоте\n");
    printf("3. По объему памяти и частоте\n");
    printf("Введите ваш выбор (1-3): ");
    scanf("%d", &choice);

    for (int i = 0; i < recordCount - 1; i++) {
        for (int j = 0; j < recordCount - i - 1; j++) {
            int needSwap = 0;
            switch (choice) {
            case 1: 
                if (records[j].volume > records[j + 1].volume) {
                    needSwap = 1;
                }
                break;
            case 2: 
                if (records[j].frequency > records[j + 1].frequency) {
                    needSwap = 1;
                }
                break;
            case 3: 
                if (records[j].volume > records[j + 1].volume ||
                    (records[j].volume == records[j + 1].volume && records[j].frequency > records[j + 1].frequency)) {
                    needSwap = 1;
                }
                break;
            default:
                printf("Неверный выбор. Сортировка не выполнена.\n");
                return;
            }

            if (needSwap) {
                MemoryRecord temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
    printf("Записи успешно отсортированы.\n");
    return 0;
}

void saveToFile(const MemoryRecord* records, int recordCount) {
    FILE* file = fopen("RAM_records.txt", "w");
    if (file == NULL) {
        printf("Ошибка при открытии файла для записи!\n");
        return -1;
    }

    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "%s %d %d %s %d\n",
            records[i].type,
            records[i].volume,
            records[i].frequency,
            records[i].radiator,
            records[i].latency);
    }

    fclose(file);
    printf("Данные успешно сохранены в файл 'RAM_records.txt'.\n");
    return 0;
}

int loadFromFile(MemoryRecord* records, int* recordCount, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка: Не удалось открыть файл %s для чтения.\n", filename);
        return -1;
    }

    int count = 0;
    while (fscanf(file, "%19s %d %d %4s %d",
        records[count].type,
        &records[count].volume,
        &records[count].frequency,
        records[count].radiator,
        &records[count].latency) == 5) {
        count++;
        if (count >= MAX_RECORDS) {
            printf("Предупреждение: Достигнут максимум записей (%d). Остальные записи будут проигнорированы.\n", MAX_RECORDS);
            break;
        }
    }

    fclose(file);
    *recordCount = count;

    if (count == 0) {
        printf("Файл %s пуст или имеет некорректный формат.\n", filename);
    }
    else {
        printf("Успешно загружено %d записей из файла %s.\n", count, filename);
        printf("Данные загружены успешно.\n");
    }
    return count;
}

int editRecord(MemoryRecord* records, int recordCount, int index) {
    if (index < 0 || index >= recordCount) {
        printf("Ошибка: Некорректный номер записи.\n");
        return -1;
    }

    printf("Редактирование записи #%d:\n", index + 1);

    printf("Введите новый тип памяти: ");
    scanf("%s", records[index].type);

    printf("Введите новый объем памяти (ГБ): ");
    scanf("%d", &records[index].volume);

    printf("Введите новую тактовую частоту (МГц): ");
    scanf("%d", &records[index].frequency);

    printf("Есть ли радиатор (Yes/No): ");
    scanf("%s", records[index].radiator);

    printf("Введите новые тайминги: ");
    scanf("%d", &records[index].latency);

    printf("Запись успешно изменена.\n");
    return 0;
}

int addMultipleRecords(MemoryRecord* records, int* recordCount, int n) {
    printf("Сколько записей вы хотите добавить? ");
    scanf("%d", &n);

    if (*recordCount + n > MAX_RECORDS) {
        printf("Ошибка: недостаточно места для добавления %d записей.\n", n);
        return -1;
    }

    for (int i = 0; i < n; i++) {
        printf("Добавление записи #%d из %d:\n", i + 1, n);
        if (addRecord(records, recordCount) != 0) {
            printf("Ошибка при добавлении записи #%d.\n", i + 1);
            return -1;
        }
    }

    printf("Записи успешно добавлены.\n");
    return 0;
}

int deleteRecord(MemoryRecord* records, int* recordCount, int index) {
    if (index < 0 || index >= *recordCount) return -1;  

    for (int i = index; i < *recordCount - 1; i++) {
        records[i] = records[i + 1];
    }

    (*recordCount)--;
    return 0; 
}
