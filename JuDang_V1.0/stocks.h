#ifndef STOCKS_H
#define STOCKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_STOCKS 10      // 시뮬레이션할 주식의 개수
#define NUM_DAYS 10         // 시뮬레이션할 일 수

// 주식 구조체
typedef struct {
    char name[20];       // 주식 이름
    double* price;       // 일별 주식 가격을 가리키는 포인터
} Stock;

// 가상 주식 초기화
void initializeStocks(Stock* stocks);

// 몬테카를로 시뮬레이션을 통해 추천 주식 찾기
char** recommendStockMonteCarlo(Stock* stocks, int* num_recommendations);

// 기술적 분석을 사용하여 주식 추천
char** recommendStockTech(Stock* stocks, int* num_recommendations);

// 주식 가격이 주어진 일 수 동안 증가한 주식 추천
char** recommendStockPriceChange(Stock* stocks, int* num_recommendations, int num_days, int increase);

#endif /* STOCKS_H */
