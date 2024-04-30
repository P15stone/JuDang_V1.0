#ifndef STOCKS_H
#define STOCKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_STOCKS 10      // �ùķ��̼��� �ֽ��� ����
#define NUM_DAYS 10         // �ùķ��̼��� �� ��

// �ֽ� ����ü
typedef struct {
    char name[20];       // �ֽ� �̸�
    double* price;       // �Ϻ� �ֽ� ������ ����Ű�� ������
} Stock;

// ���� �ֽ� �ʱ�ȭ
void initializeStocks(Stock* stocks);

// ����ī���� �ùķ��̼��� ���� ��õ �ֽ� ã��
char** recommendStockMonteCarlo(Stock* stocks, int* num_recommendations);

// ����� �м��� ����Ͽ� �ֽ� ��õ
char** recommendStockTech(Stock* stocks, int* num_recommendations);

// �ֽ� ������ �־��� �� �� ���� ������ �ֽ� ��õ
char** recommendStockPriceChange(Stock* stocks, int* num_recommendations, int num_days, int increase);

#endif /* STOCKS_H */
