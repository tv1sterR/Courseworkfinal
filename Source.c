#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_RECORDS 100

typedef struct {
    char type[20];           // ��� ������ (DDR4)
    int volume; // ����� ������ � ��
    int frequency; // �������� ������� � ���
    int latency; // ��������
    char radiator[5];        // ������� ��������� ("Yes" ��� "No")
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
        printf("\n����:\n");
        printf("1. �������� ������\n");
        printf("2. �������� ��� ������\n");
        printf("3. ����� ������\n");
        printf("4. ����������� ������\n");
        printf("5. ��������� ������ � ����\n");
        printf("6. ��������� ������ �� �����\n");
        printf("7. �������� ������\n");
        printf("8. �������� ��������� �������\n");
        printf("9. ������� ������\n");
        printf("0. �����\n");
        printf("�������� �����: ");
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
            printf("������� ����� ������ ��� ���������: ");
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
                printf("��� ������� ��� ��������.\n");
                break;
            }
            printRecords(records, recordCount);
            printf("������� ����� ������ ��� ��������: ");
            int index;
            scanf("%d", &index);
            if (deleteRecord(records, &recordCount, index - 1) == 0) {
                printf("������ ������� �������.\n");
            }
            else {
                printf("������ ��� �������� ������.\n");
            }
            break;
        }
        case 0:
            printf("����� �� ���������.\n");
            break;
        default:
            printf("�������� �����, ���������� �����.\n");
        }
    } while (choice != 0);

    return 0;
}

int addRecord(MemoryRecord* records, int* recordCount) {
    if (*recordCount >= MAX_RECORDS) {
        printf("������: ���� ������ �����������.\n");
        return -1;
    }

    printf("������� ��� ������: ");
    scanf("%s", records[*recordCount].type);

    printf("������� ����� ������ (��): ");
    scanf("%d", &records[*recordCount].volume);

    printf("������� �������� ������� (���): ");
    scanf("%d", &records[*recordCount].frequency);

    printf("���� �� �������� (Yes/No): ");
    scanf("%s", records[*recordCount].radiator);

    printf("������� ��������: ");
    scanf("%d", &records[*recordCount].latency);

    (*recordCount)++;
    printf("������ ������� ���������.\n");
    return 0;
}


void printRecords(const MemoryRecord* records, int recordCount) {
    if (recordCount == 0) {
        printf("���� ������ �����.\n");
        return 0;
    }
    for (int i = 0; i < recordCount; i++) {
        printf("%d. ���: %s, �����: %d ��, �������: %d ���, ��������: %s, ��������: %d\n",
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

    printf("������� ��� ������ (��� ������� Enter ��� ��������): ");
    getchar(); 
    fgets(type, sizeof(type), stdin);
    if (type[strlen(type) - 1] == '\n') {
        type[strlen(type) - 1] = '\0'; 
    }

    printf("������� ����������� �������� ������� (��� ������� Enter ��� ��������): ");
    fgets(minFreqStr, sizeof(minFreqStr), stdin);
    if (minFreqStr[0] != '\n') {
        minFrequency = atoi(minFreqStr); 
    }

    printf("������� ������������ �������� ������� (��� ������� Enter ��� ��������): ");
    fgets(maxFreqStr, sizeof(maxFreqStr), stdin);
    if (maxFreqStr[0] != '\n') {
        maxFrequency = atoi(maxFreqStr); 
    }

    printf("\n���������� ������:\n");
    int found = 0;
    for (int i = 0; i < recordCount; i++) {

        int typeMatch = (strlen(type) == 0 || strcmp(records[i].type, type) == 0);
        int minFreqMatch = (minFrequency == 0 || records[i].frequency >= minFrequency);
        int maxFreqMatch = (maxFrequency == 0 || records[i].frequency <= maxFrequency);

        if (typeMatch && minFreqMatch && maxFreqMatch) {
            printf("%d. ���: %s, �����: %d ��, �������: %d ���, ��������: %s, ��������: %d\n",
                i + 1, records[i].type, records[i].volume, records[i].frequency,
                records[i].radiator, records[i].latency);
            found = 1;
        }
    }

    if (!found) {
        printf("������ �� �������.\n");
    } else {
        printf("����� ��������.\n");
    }
    return found;
}

void sortRecords(MemoryRecord* records, int recordCount) {
    if (recordCount == 0) {
        printf("���� ������ �����, ����������� ������.\n");
        return -1;
    }

    int choice;
    printf("�������� �������� ����������:\n");
    printf("1. �� ������ ������\n");
    printf("2. �� �������� �������\n");
    printf("3. �� ������ ������ � �������\n");
    printf("������� ��� ����� (1-3): ");
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
                printf("�������� �����. ���������� �� ���������.\n");
                return;
            }

            if (needSwap) {
                MemoryRecord temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
    printf("������ ������� �������������.\n");
    return 0;
}

void saveToFile(const MemoryRecord* records, int recordCount) {
    FILE* file = fopen("RAM_records.txt", "w");
    if (file == NULL) {
        printf("������ ��� �������� ����� ��� ������!\n");
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
    printf("������ ������� ��������� � ���� 'RAM_records.txt'.\n");
    return 0;
}

int loadFromFile(MemoryRecord* records, int* recordCount, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("������: �� ������� ������� ���� %s ��� ������.\n", filename);
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
            printf("��������������: ��������� �������� ������� (%d). ��������� ������ ����� ���������������.\n", MAX_RECORDS);
            break;
        }
    }

    fclose(file);
    *recordCount = count;

    if (count == 0) {
        printf("���� %s ���� ��� ����� ������������ ������.\n", filename);
    }
    else {
        printf("������� ��������� %d ������� �� ����� %s.\n", count, filename);
        printf("������ ��������� �������.\n");
    }
    return count;
}

int editRecord(MemoryRecord* records, int recordCount, int index) {
    if (index < 0 || index >= recordCount) {
        printf("������: ������������ ����� ������.\n");
        return -1;
    }

    printf("�������������� ������ #%d:\n", index + 1);

    printf("������� ����� ��� ������: ");
    scanf("%s", records[index].type);

    printf("������� ����� ����� ������ (��): ");
    scanf("%d", &records[index].volume);

    printf("������� ����� �������� ������� (���): ");
    scanf("%d", &records[index].frequency);

    printf("���� �� �������� (Yes/No): ");
    scanf("%s", records[index].radiator);

    printf("������� ����� ��������: ");
    scanf("%d", &records[index].latency);

    printf("������ ������� ��������.\n");
    return 0;
}

int addMultipleRecords(MemoryRecord* records, int* recordCount, int n) {
    printf("������� ������� �� ������ ��������? ");
    scanf("%d", &n);

    if (*recordCount + n > MAX_RECORDS) {
        printf("������: ������������ ����� ��� ���������� %d �������.\n", n);
        return -1;
    }

    for (int i = 0; i < n; i++) {
        printf("���������� ������ #%d �� %d:\n", i + 1, n);
        if (addRecord(records, recordCount) != 0) {
            printf("������ ��� ���������� ������ #%d.\n", i + 1);
            return -1;
        }
    }

    printf("������ ������� ���������.\n");
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
