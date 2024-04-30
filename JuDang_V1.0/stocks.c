#include "stocks.h"

// ���� �ֽ� �ʱ�ȭ
void initializeStocks(Stock* stocks) {
    char* stock_names[NUM_STOCKS] = { "����", "����", "�Ƹ���", "����ũ�μ���Ʈ", "���̽���",
                                      "�׽���", "���ø���", "�˸��ٹ�", "������", "���̿���" };

    // �� �ֽĿ� ���� �޸� ���� �Ҵ�
    for (int i = 0; i < NUM_STOCKS; i++) {
        if (strcpy_s(stocks[i].name, sizeof(stocks[i].name), stock_names[i]) != 0) {
            fprintf(stderr, "���ڿ� ���� ����\n");
            exit(1);
        }

        stocks[i].price = (double*)malloc(NUM_DAYS * sizeof(double));
        if (stocks[i].price == NULL) {
            fprintf(stderr, "�޸� �Ҵ� ����\n");
            exit(1);
        }

        // ���� �ֽ� ���� �ʱ�ȭ
        for (int j = 0; j < NUM_DAYS; j++) {
            stocks[i].price[j] = 100.0 + (rand() % 100); // ������ 100���� 200 ������ ������ ������ �ʱ�ȭ
        }
    }
}

// ����ī���� �ùķ��̼��� ���� ��õ �ֽ� ã��
char** recommendStockMonteCarlo(Stock* stocks, int* num_recommendations) {
    double* total = (double*)malloc(NUM_STOCKS * sizeof(double));
    if (total == NULL) {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        exit(1);
    }
    double* average = (double*)malloc(NUM_STOCKS * sizeof(double));
    if (average == NULL) {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        exit(1);
    }

    // �� �ֽ��� �� ���� ���
    for (int i = 0; i < NUM_STOCKS; i++) {
        total[i] = 0.0;
        for (int j = 0; j < NUM_DAYS; j++) {
            total[i] += stocks[i].price[j];
        }
        average[i] = total[i] / NUM_DAYS; // ��� ���� ���
    }

    // ��� ������ ���� ���� 3���� �ֽ� ��õ
    char** recommendations = (char**)malloc(3 * sizeof(char*));
    if (recommendations == NULL) {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        exit(1);
    }

    // ���� ������ �ֽ� ����
    for (int i = 0; i < NUM_STOCKS - 1; i++) {
        for (int j = 0; j < NUM_STOCKS - i - 1; j++) {
            if (average[j] < average[j + 1]) {
                double temp = average[j];
                average[j] = average[j + 1];
                average[j + 1] = temp;
                char temp_name[20];
                if (strcpy_s(temp_name, sizeof(temp_name), stocks[j].name) != 0) {
                    fprintf(stderr, "���ڿ� ���� ����\n");
                    exit(1);
                }
                if (strcpy_s(stocks[j].name, sizeof(stocks[j].name), stocks[j + 1].name) != 0) {
                    fprintf(stderr, "���ڿ� ���� ����\n");
                    exit(1);
                }
                if (strcpy_s(stocks[j + 1].name, sizeof(stocks[j + 1].name), temp_name) != 0) {
                    fprintf(stderr, "���ڿ� ���� ����\n");
                    exit(1);
                }
            }
        }
    }

    // ���� 3�� ��õ
    for (int i = 0; i < 3; i++) {
        recommendations[i] = _strdup(stocks[i].name);
        if (recommendations[i] == NULL) {
            fprintf(stderr, "�޸� �Ҵ� ����\n");
            exit(1);
        }
    }

    *num_recommendations = 3;
    free(total);
    free(average);
    return recommendations;
}

// ����� �м��� ����Ͽ� �ֽ� ��õ
char** recommendStockTech(Stock* stocks, int* num_recommendations) {
    char** recommendations = (char**)malloc(3 * sizeof(char*));
    if (recommendations == NULL) {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        exit(1);
    }

    // ����� �м��� ���� ��� �߼��� �ֽ� ��õ
    int count = 0;
    for (int i = 0; i < NUM_STOCKS; i++) {
        double trend = 0.0; // �ֽ� ���� �帧

        // �ֱ� 3�ϰ��� �ֽ� ���� ��ȭ�� Ȯ���Ͽ� �帧 ���
        for (int j = NUM_DAYS - 1; j >= NUM_DAYS - 3; j--) {
            trend += stocks[i].price[j] - stocks[i].price[j - 1];
        }

        // �ֱ� 3�ϰ��� �ֽ� ������ ��� ���� ��� �ش� �ֽ��� ��õ
        if (trend > 0 && count < 3) {
            recommendations[count] = _strdup(stocks[i].name);
            if (recommendations[count] == NULL) {
                fprintf(stderr, "�޸� �Ҵ� ����\n");
                exit(1);
            }
            count++;
        }
    }

    *num_recommendations = count;
    return recommendations;
}

// �ֽ� ������ �־��� �� �� ���� ������ �ֽ� ��õ
char** recommendStockPriceChange(Stock* stocks, int* num_recommendations, int num_days, int increase) {
    char** recommendations = (char**)malloc(NUM_STOCKS * sizeof(char*));
    if (recommendations == NULL) {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        exit(1);
    }

    int count = 0;
    for (int i = 0; i < NUM_STOCKS; i++) {
        // �ֽ� ������ �־��� �� �� ���� ���� �Ǵ� �����ߴ��� Ȯ��
        int price_change = 1;
        for (int j = NUM_DAYS - 1; j >= NUM_DAYS - num_days; j--) {
            if ((increase && stocks[i].price[j] <= stocks[i].price[j - 1]) ||
                (!increase && stocks[i].price[j] >= stocks[i].price[j - 1])) {
                price_change = 0;
                break;
            }
        }
        if (price_change) {
            recommendations[count] = _strdup(stocks[i].name);
            if (recommendations[count] == NULL) {
                fprintf(stderr, "�޸� �Ҵ� ����\n");
                exit(1);
            }
            count++;
        }
    }

    *num_recommendations = count;
    return recommendations;
}
