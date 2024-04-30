#include "stocks.h"

// 가상 주식 초기화
void initializeStocks(Stock* stocks) {
    char* stock_names[NUM_STOCKS] = { "애플", "구글", "아마존", "마이크로소프트", "페이스북",
                                      "테슬라", "넷플릭스", "알리바바", "엔비디아", "에이엠디" };

    // 각 주식에 대한 메모리 동적 할당
    for (int i = 0; i < NUM_STOCKS; i++) {
        if (strcpy_s(stocks[i].name, sizeof(stocks[i].name), stock_names[i]) != 0) {
            fprintf(stderr, "문자열 복사 실패\n");
            exit(1);
        }

        stocks[i].price = (double*)malloc(NUM_DAYS * sizeof(double));
        if (stocks[i].price == NULL) {
            fprintf(stderr, "메모리 할당 실패\n");
            exit(1);
        }

        // 가상 주식 가격 초기화
        for (int j = 0; j < NUM_DAYS; j++) {
            stocks[i].price[j] = 100.0 + (rand() % 100); // 가격을 100에서 200 사이의 무작위 값으로 초기화
        }
    }
}

// 몬테카를로 시뮬레이션을 통해 추천 주식 찾기
char** recommendStockMonteCarlo(Stock* stocks, int* num_recommendations) {
    double* total = (double*)malloc(NUM_STOCKS * sizeof(double));
    if (total == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    double* average = (double*)malloc(NUM_STOCKS * sizeof(double));
    if (average == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }

    // 각 주식의 총 가격 계산
    for (int i = 0; i < NUM_STOCKS; i++) {
        total[i] = 0.0;
        for (int j = 0; j < NUM_DAYS; j++) {
            total[i] += stocks[i].price[j];
        }
        average[i] = total[i] / NUM_DAYS; // 평균 가격 계산
    }

    // 평균 가격이 높은 상위 3개의 주식 추천
    char** recommendations = (char**)malloc(3 * sizeof(char*));
    if (recommendations == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }

    // 가격 순으로 주식 정렬
    for (int i = 0; i < NUM_STOCKS - 1; i++) {
        for (int j = 0; j < NUM_STOCKS - i - 1; j++) {
            if (average[j] < average[j + 1]) {
                double temp = average[j];
                average[j] = average[j + 1];
                average[j + 1] = temp;
                char temp_name[20];
                if (strcpy_s(temp_name, sizeof(temp_name), stocks[j].name) != 0) {
                    fprintf(stderr, "문자열 복사 실패\n");
                    exit(1);
                }
                if (strcpy_s(stocks[j].name, sizeof(stocks[j].name), stocks[j + 1].name) != 0) {
                    fprintf(stderr, "문자열 복사 실패\n");
                    exit(1);
                }
                if (strcpy_s(stocks[j + 1].name, sizeof(stocks[j + 1].name), temp_name) != 0) {
                    fprintf(stderr, "문자열 복사 실패\n");
                    exit(1);
                }
            }
        }
    }

    // 상위 3개 추천
    for (int i = 0; i < 3; i++) {
        recommendations[i] = _strdup(stocks[i].name);
        if (recommendations[i] == NULL) {
            fprintf(stderr, "메모리 할당 실패\n");
            exit(1);
        }
    }

    *num_recommendations = 3;
    free(total);
    free(average);
    return recommendations;
}

// 기술적 분석을 사용하여 주식 추천
char** recommendStockTech(Stock* stocks, int* num_recommendations) {
    char** recommendations = (char**)malloc(3 * sizeof(char*));
    if (recommendations == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }

    // 기술적 분석을 통해 상승 추세인 주식 추천
    int count = 0;
    for (int i = 0; i < NUM_STOCKS; i++) {
        double trend = 0.0; // 주식 가격 흐름

        // 최근 3일간의 주식 가격 변화를 확인하여 흐름 계산
        for (int j = NUM_DAYS - 1; j >= NUM_DAYS - 3; j--) {
            trend += stocks[i].price[j] - stocks[i].price[j - 1];
        }

        // 최근 3일간의 주식 가격이 상승 했을 경우 해당 주식을 추천
        if (trend > 0 && count < 3) {
            recommendations[count] = _strdup(stocks[i].name);
            if (recommendations[count] == NULL) {
                fprintf(stderr, "메모리 할당 실패\n");
                exit(1);
            }
            count++;
        }
    }

    *num_recommendations = count;
    return recommendations;
}

// 주식 가격이 주어진 일 수 동안 증가한 주식 추천
char** recommendStockPriceChange(Stock* stocks, int* num_recommendations, int num_days, int increase) {
    char** recommendations = (char**)malloc(NUM_STOCKS * sizeof(char*));
    if (recommendations == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }

    int count = 0;
    for (int i = 0; i < NUM_STOCKS; i++) {
        // 주식 가격이 주어진 일 수 동안 증가 또는 감소했는지 확인
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
                fprintf(stderr, "메모리 할당 실패\n");
                exit(1);
            }
            count++;
        }
    }

    *num_recommendations = count;
    return recommendations;
}
