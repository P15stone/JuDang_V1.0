#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stocks.h"

// 사용자가 엔터 키를 누를 때까지 대기
void waitForEnter() {
    printf("계속하려면 엔터 키를 누르세요...");
    while (getchar() != '\n'); // Enter 키가 입력될 때까지 대기
}

int main() {
    srand((unsigned int)time(NULL));  // 난수 발생기 초기화

    printf("====================================\n");
    printf("주식 추천 프로그램 'JuDang'에 오신 것을 환영합니다 ^^\n");
    printf("====================================\n\n");

    waitForEnter(); // 사용자가 엔터를 누를 때까지 대기

    Stock* stocks = (Stock*)malloc(NUM_STOCKS * sizeof(Stock)); // 주식 배열 동적 할당
    if (stocks == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        return 1;
    }

    initializeStocks(stocks);  // 주식 초기화

    // 현재 주식 가격 출력
    printf("%d일간의 현재 주식 가격:\n", NUM_DAYS);
    for (int i = 0; i < NUM_STOCKS; i++) {
        printf("%s: ", stocks[i].name);
        for (int j = 0; j < NUM_DAYS; j++) {
            printf("%.2f ", stocks[i].price[j]);
        }
        printf("\n");
    }

    // 몬테카를로 시뮬레이션을 통한 추천 주식 출력
    printf("\n몬테카를로 방법으로 추천된 주식:\n");
    int num_recommendations;
    char** monte_carlo_recommendations = recommendStockMonteCarlo(stocks, &num_recommendations);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", monte_carlo_recommendations[i]);
            free(monte_carlo_recommendations[i]);
        }
    }
    else {
        printf("없음\n");
    }
    free(monte_carlo_recommendations); // 메모리 해제

    // 기술적 분석을 통한 추천 주식 출력
    printf("\n기술적 분석으로 추천된 주식:\n");
    char** tech_recommendations = recommendStockTech(stocks, &num_recommendations);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", tech_recommendations[i]);
            free(tech_recommendations[i]);
        }
    }
    else {
        printf("없음\n");
    }
    free(tech_recommendations); // 메모리 해제

    // 주식 가격이 3일 동안 증가한 주식 출력
    printf("\n3일 동안 주식 가격이 증가한 주식:\n");
    char** increase_recommendations = recommendStockPriceChange(stocks, &num_recommendations, 3, 1);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", increase_recommendations[i]);
            free(increase_recommendations[i]);
        }
    }
    else {
        printf("없음\n");
    }
    free(increase_recommendations); // 메모리 해제

    // 주식 가격이 3일 동안 감소한 주식 출력
    printf("\n3일 동안 주식 가격이 감소한 주식:\n");
    char** decrease_recommendations = recommendStockPriceChange(stocks, &num_recommendations, 3, 0);
    if (num_recommendations > 0) {
        for (int i = 0; i < num_recommendations; i++) {
            printf("%s\n", decrease_recommendations[i]);
            free(decrease_recommendations[i]);
        }
    }
    else {
        printf("없음\n");
    }
    free(decrease_recommendations); // 메모리 해제

    // 메모리 해제
    for (int i = 0; i < NUM_STOCKS; i++) {
        free(stocks[i].price);
    }
    free(stocks);

    return 0;
}
