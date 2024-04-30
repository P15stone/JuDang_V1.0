#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stocks.h"

// ����ڰ� ���� Ű�� ���� ������ ���
void waitForEnter() {
    printf("����Ϸ��� ���� Ű�� ��������...");
    while (getchar() != '\n'); // Enter Ű�� �Էµ� ������ ���
}

int main() {
    srand((unsigned int)time(NULL));  // ���� �߻��� �ʱ�ȭ

    printf("====================================\n");
    printf("�ֽ� ��õ ���α׷� 'JuDang'�� ���� ���� ȯ���մϴ� ^^\n");
    printf("====================================\n\n");

    waitForEnter(); // ����ڰ� ���͸� ���� ������ ���

    Stock* stocks = (Stock*)malloc(NUM_STOCKS * sizeof(Stock)); // �ֽ� �迭 ���� �Ҵ�
    if (stocks == NULL) {
        fprintf(stderr, "�޸� �Ҵ� ����\n");
        return 1;
    }

    initializeStocks(stocks);  // �ֽ� �ʱ�ȭ

    // ���� �ֽ� ���� ���
    printf("%d�ϰ��� ���� �ֽ� ����:\n", NUM_DAYS);
    for (int i = 0; i < NUM_STOCKS; i++) {
        printf("%s: ", stocks[i].name);
        for (int j = 0; j < NUM_DAYS; j++) {
            printf("%.2f ", stocks[i].price[j]);
        }
        printf("\n");
    }

    // ����ī���� �ùķ��̼��� ���� ��õ �ֽ� ���
    printf("\n����ī���� ������� ��õ�� �ֽ�:\n");
    int num_recommendations;
    char** monte_carlo_recommendations = recommendStockMonteCarlo(stocks, &num_recommendations);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", monte_carlo_recommendations[i]);
            free(monte_carlo_recommendations[i]);
        }
    }
    else {
        printf("����\n");
    }
    free(monte_carlo_recommendations); // �޸� ����

    // ����� �м��� ���� ��õ �ֽ� ���
    printf("\n����� �м����� ��õ�� �ֽ�:\n");
    char** tech_recommendations = recommendStockTech(stocks, &num_recommendations);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", tech_recommendations[i]);
            free(tech_recommendations[i]);
        }
    }
    else {
        printf("����\n");
    }
    free(tech_recommendations); // �޸� ����

    // �ֽ� ������ 3�� ���� ������ �ֽ� ���
    printf("\n3�� ���� �ֽ� ������ ������ �ֽ�:\n");
    char** increase_recommendations = recommendStockPriceChange(stocks, &num_recommendations, 3, 1);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", increase_recommendations[i]);
            free(increase_recommendations[i]);
        }
    }
    else {
        printf("����\n");
    }
    free(increase_recommendations); // �޸� ����

    // �ֽ� ������ 3�� ���� ������ �ֽ� ���
    printf("\n3�� ���� �ֽ� ������ ������ �ֽ�:\n");
    char** decrease_recommendations = recommendStockPriceChange(stocks, &num_recommendations, 3, 0);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", decrease_recommendations[i]);
            free(decrease_recommendations[i]);
        }
    }
    else {
        printf("����\n");
    }
    free(decrease_recommendations); // �޸� ����

    // �޸� ����
    for (int i = 0; i < NUM_STOCKS; i++) {
        free(stocks[i].price);
    }
    free(stocks);

    return 0;
}
