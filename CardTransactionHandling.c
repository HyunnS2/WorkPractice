#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 카드 결제 내역에 대한 입력값 구조체 선언
typedef struct {
    char date[7];         
    char merchant[31];    
    char cardNumber[17];  
    char amount[10];      
    char fee[7];          
    char responseCode[4]; 
} Transaction;

typedef struct {
    Transaction *transactions;
    size_t size;
    size_t capacity;
} TransactionList;

void initTransactionList(TransactionList *list) {
    list->size = 0;
    list->capacity = 10; 
    list->transactions = malloc(list->capacity * sizeof(Transaction));
}

void freeTransactionList(TransactionList *list) {
    free(list->transactions);
    list->transactions = NULL;
    list->size = 0;
    list->capacity = 0;
}

// 카드 결제 내역을 입력받는 함수
void addTransaction(TransactionList *list, const char *date, const char *merchant, const char *cardNumber, const char *amount, const char *fee, const char *responseCode) {
    // 만약에 용량이 다 차면 두배로 늘림 
    if (list->size == list->capacity) {
        list->capacity *= 2; 
        list->transactions = realloc(list->transactions, list->capacity * sizeof(Transaction));
    }

    strncpy(list->transactions[list->size].date, date, sizeof(list->transactions[list->size].date) - 1);
    list->transactions[list->size].date[sizeof(list->transactions[list->size].date) - 1] = '\0';
    
    strncpy(list->transactions[list->size].merchant, merchant, sizeof(list->transactions[list->size].merchant) - 1);
    list->transactions[list->size].merchant[sizeof(list->transactions[list->size].merchant) - 1] = '\0';
    
    strncpy(list->transactions[list->size].cardNumber, cardNumber, sizeof(list->transactions[list->size].cardNumber) - 1);
    list->transactions[list->size].cardNumber[sizeof(list->transactions[list->size].cardNumber) - 1] = '\0';
    
    strncpy(list->transactions[list->size].amount, amount, sizeof(list->transactions[list->size].amount) - 1);
    list->transactions[list->size].amount[sizeof(list->transactions[list->size].amount) - 1] = '\0';
    
    strncpy(list->transactions[list->size].fee, fee, sizeof(list->transactions[list->size].fee) - 1);
    list->transactions[list->size].fee[sizeof(list->transactions[list->size].fee) - 1] = '\0';
    
    strncpy(list->transactions[list->size].responseCode, responseCode, sizeof(list->transactions[list->size].responseCode) - 1);
    list->transactions[list->size].responseCode[sizeof(list->transactions[list->size].responseCode) - 1] = '\0';
    
    list->size++;
}

// 입력 받은 값을 파일로 저장하는 함수 
void saveTransactionFile(const TransactionList *list, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return;
    }

    for (size_t i = 0; i < list->size; i++) {
        fprintf(file, "%s|%s|%s|%s|%s|%s\n", 
                list->transactions[i].date, 
                list->transactions[i].merchant, 
                list->transactions[i].cardNumber, 
                list->transactions[i].amount, 
                list->transactions[i].fee, 
                list->transactions[i].responseCode);
    }

    fclose(file);
}

// 저장 파일에서 값을 불러오는 함수
void loadTransactionFile(TransactionList *list, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        Transaction transaction;
        sscanf(buffer, "%6[^|]|%30[^|]|%16[^|]|%9[^|]|%6[^|]|%3s", 
               transaction.date, 
               transaction.merchant, 
               transaction.cardNumber, 
               transaction.amount, 
               transaction.fee, 
               transaction.responseCode);
        addTransaction(list, transaction.date, transaction.merchant, transaction.cardNumber, transaction.amount, transaction.fee, transaction.responseCode);
    }

    fclose(file);
}

// 입력 받은 결과를 출력하는 함수  reportTransaction
void reportTransaction(const TransactionList *list) {
    printf("=======================================<<결제 내역 레포트>>========================================\n");
    printf("날짜\t가맹점 상호명\t\t\t 카드 번호\t\t결제 금액\t수수료\t 응답 코드\n");
    printf("---------------------------------------------------------------------------------------------------\n");
    for (size_t i = 0; i < list->size; i++) {
        printf("%s\t%-30s\t%s\t%s\t%s\t%s\n", 
            list->transactions[i].date, 
            list->transactions[i].merchant, 
            list->transactions[i].cardNumber, 
            list->transactions[i].amount, 
            list->transactions[i].fee, 
            list->transactions[i].responseCode);
    }

    printf("===================================================================================================\n");
}

// 입력된 결제 정보에 대하여 영수증 출력되는 기능 -> 카드 번호 입력받기
void printReceiptCardNumber(const TransactionList *list, const char *cardNumber) {
    printf("=============================================<<영수증>>=============================================\n");
    printf("날짜\t가맹점 상호명\t\t\t 카드 번호\t\t결제 금액\t수수료\t 응답 코드\n");
    printf("---------------------------------------------------------------------------------------------------\n");
    int found = 0;
    for (size_t i = 0; i < list->size; i++) {
        if (strcmp(list->transactions[i].cardNumber, cardNumber) == 0) {
            printf("%s\t%-30s\t%s\t%s\t%s\t%s\n", 
                list->transactions[i].date, 
                list->transactions[i].merchant, 
                list->transactions[i].cardNumber, 
                list->transactions[i].amount, 
                list->transactions[i].fee, 
                list->transactions[i].responseCode);
            found = 1;
        }
    }
    printf("====================================================================================================\n");
    if (!found) {
        printf("해당 카드 번호로 결제 내역을 찾을 수 없습니다.\n");
    }
}

// 입력된 결제 정보에 대하여 영수증 출력되는 기능 -> 가맹점 상호명 입력받기
void printReceiptMerchant(const TransactionList *list, const char *merchant) {
    printf("=============================================<<영수증>>=============================================\n");
    printf("날짜\t가맹점 상호명\t\t\t 카드 번호\t\t결제 금액\t수수료\t 응답 코드\n");
    printf("---------------------------------------------------------------------------------------------------\n");
    int found = 0;
    for (size_t i = 0; i < list->size; i++) {
        if (strcmp(list->transactions[i].merchant, merchant) == 0) {
            printf("%s\t%-30s\t%s\t%s\t%s\t%s\n", 
                list->transactions[i].date, 
                list->transactions[i].merchant, 
                list->transactions[i].cardNumber, 
                list->transactions[i].amount, 
                list->transactions[i].fee, 
                list->transactions[i].responseCode);
            found = 1;
        }
    }
    printf("====================================================================================================\n");
    if (!found) {
        printf("해당 가맹점 상호명으로 결제 내역을 찾을 수 없습니다.\n");
    }
}

// 입력된 결제 정보에 대하여 일자별 건수 및 금액 집계 함수
void reportDailySum(const TransactionList *list){

  typedef struct{
    char date[7];
    int count;
    int totalAmount;
  } DailySum;

  // 결제 내역 100건 제한 
  DailySum sum[100];
  size_t sumCount = 0;

  for(size_t i = 0; i < list->size; i++){
    int amount = atoi(list->transactions[i].amount);
        int found = 0;
        for (size_t j = 0; j < sumCount; j++) {
            if (strcmp(sum[j].date, list->transactions[i].date) == 0) {
                sum[j].count++;
                sum[j].totalAmount += amount;
                found = 1;
                break;
            }
        }
        if (!found) {
            strncpy(sum[sumCount].date, list->transactions[i].date, sizeof(sum[sumCount].date) - 1);
            sum[sumCount].date[sizeof(sum[sumCount].date) - 1] = '\0';
            sum[sumCount].count = 1;
            sum[sumCount].totalAmount = amount;
            sumCount++;
        }

  }

  printf("=========<<총 일자별 결제 집계>>========\n");
  printf("날짜\t건수\t총 결제 금액\n");
  printf("---------------------------------------\n");

  for (size_t i = 0; i < sumCount; i++) {
        printf("%s\t%d\t%d\n", sum[i].date, sum[i].count, sum[i].totalAmount);
  }

  printf("=======================================\n");

}

// 입력된 결제 정보에 대하여 가맹점별 건수 및 금액 집계 함수 
void reportMerchantSum(const TransactionList *list) {
    typedef struct {
        char merchant[31];
        int count;
        int totalAmount;
    } MerchantSum;

    MerchantSum sum[100]; // 최대 100개의 가맹점 요약을 저장할 수 있다고 가정
    size_t sumCount = 0;

    for (size_t i = 0; i < list->size; i++) {
        int amount = atoi(list->transactions[i].amount);
        int found = 0;
        for (size_t j = 0; j < sumCount; j++) {
            if (strcmp(sum[j].merchant, list->transactions[i].merchant) == 0) {
                sum[j].count++;
                sum[j].totalAmount += amount;
                found = 1;
                break;
            }
        }
        if (!found) {
            strncpy(sum[sumCount].merchant, list->transactions[i].merchant, sizeof(sum[sumCount].merchant) - 1);
            sum[sumCount].merchant[sizeof(sum[sumCount].merchant) - 1] = '\0';
            sum[sumCount].count = 1;
            sum[sumCount].totalAmount = amount;
            sumCount++;
        }
    }

    printf("===================<<총 가맹점별 결제 집계>>==================\n");
    printf("가맹점 상호명\t\t\t건수\t총 결제 금액\n");
    printf("------------------------------------------------------------\n");
    for (size_t i = 0; i < sumCount; i++) {
        printf("%-30s\t%d\t%d\n", sum[i].merchant, sum[i].count, sum[i].totalAmount);
    }
}

// 입력된 결제 정보에 대하여 가맹점별 승인 여부 레포팅
void reportMerchantApproval(const TransactionList *list){

  typedef struct {
        char merchant[31];
        int normalCode;
        int abnormalCode;
    } ApprovalSum;

    ApprovalSum sum[100]; // 최대 100개의 가맹점 요약을 저장할 수 있다고 가정
    size_t sumCount = 0;

    for (size_t i = 0; i < list->size; i++) {
        int found = 0;
        for (size_t j = 0; j < sumCount; j++) {
            if (strcmp(sum[j].merchant, list->transactions[i].merchant) == 0) {
                if (strcmp(list->transactions[i].responseCode, "000") == 0) {
                    sum[j].normalCode++;
                } else {
                    sum[j].abnormalCode++;
                }
                found = 1;
                break;
            }
        }
        if (!found) {
            strncpy(sum[sumCount].merchant, list->transactions[i].merchant, sizeof(sum[sumCount].merchant) - 1);
            sum[sumCount].merchant[sizeof(sum[sumCount].merchant) - 1] = '\0';
            if (strcmp(list->transactions[i].responseCode, "000") == 0) {
                sum[sumCount].normalCode = 1;
                sum[sumCount].abnormalCode = 0;
            } else {
                sum[sumCount].normalCode = 0;
                sum[sumCount].abnormalCode = 1;
            }
            sumCount++;
        }
    }

    printf("====================<<가맹점별 승인 요약>>====================\n");
    printf("가맹점 상호명\t\t\t정상 승인\t비정상 승인\n");
    printf("-------------------------------------------------------------\n");
    for (size_t i = 0; i < sumCount; i++) {
        printf("%-30s\t%d\t\t%d\n", sum[i].merchant, sum[i].normalCode, sum[i].abnormalCode);
    }

    printf("=============================================================\n");
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

int main() {
    TransactionList list;
    initTransactionList(&list);

    printf("카드 결제 내역을 입력하세요. \n 입력 형식은 '날짜(YYMMDD) 가맹점상호명 카드번호 금액 수수료 응답코드' 순 입니다.\n 종료를 원하면 0 을 입력하세요. 다음으로 넘어갑니다.\n");

    // 사용자로부터 입력받아 데이터 추가
    while (1) {
        char input[256];
        char date[7];
        char merchant[31];
        char cardNumber[17];
        char amount[10];
        char fee[7];
        char responseCode[4];

        printf("결제 내역 입력 : ");
        fgets(input, sizeof(input), stdin);

        // 개행 문자 제거해줘야 종료 문자 0이 인식됨 
        input[strcspn(input, "\n")] = 0;

        if (sscanf(input, "%6s %30s %16s %9s %6s %3s", date, merchant, cardNumber, amount, fee, responseCode) == 6) {
            addTransaction(&list, date, merchant, cardNumber, amount, fee, responseCode);
        } else if(strcmp(input, "0") == 0) {
          break;
        } else {
            printf("잘못된 입력 형식입니다. 다시 입력해주세요.\n");
        }
    }

    // 데이터 파일로 저장
    saveTransactionFile(&list, "CardTransactions.txt");

    // 리스트 초기화 후 파일에서 데이터 불러오기
    freeTransactionList(&list);
    initTransactionList(&list);
    loadTransactionFile(&list, "CardTransactions.txt");

    // 불러온 데이터로 레포트 생성
    reportTransaction(&list);


    // 입력된 결제 정보에 대한 출력 기능 
    int choice;
    char cardNumber[17];
    char merchant[31];

    printf("~~~~~~~~~~~~~~~~~~<<원하는 기능의 숫자를 입력하세요>>~~~~~~~~~~~~~~~\n");
    printf("1. 카드 번호에 대한 영수증 출력\n");
    printf("2. 가맹점 상호명에 대한 영수증 출력\n");
    printf("3. 총 일자별 건수 및 금액 집계 레포트\n");
    printf("4. 총 가맹점별 건수 및 금액 집계 레포트\n");
    printf("5. 총 가맹점별 정상 승인 및 비정상 승인 집계 레포트\n");
    
    
    printf("선택: ");
    scanf("%d", &choice);
    getchar(); 

    printf("===================================================================\n");

    switch (choice) {
        case 1: {
            printf("카드 번호를 정확히 입력하세요: ");
            fgets(cardNumber, sizeof(cardNumber), stdin);
            cardNumber[strcspn(cardNumber, "\n")] = 0;
            printReceiptCardNumber(&list, cardNumber);
            break;
        }
        case 2: {
            printf("가맹점 상호명을 정확히 입력하세요: ");
            fgets(merchant, sizeof(merchant), stdin);
            merchant[strcspn(merchant, "\n")] = 0;
            printReceiptMerchant(&list, merchant);
            break;
        }
        case 3: {
          reportDailySum(&list);
          break;

        }
        case 4: {
          reportMerchantSum(&list);
          break;
        }
        case 5:{
          reportMerchantApproval(&list);
          break;
        }
        default:
            printf("잘못된 선택입니다.\n");
            break;
    }

    // 메모리 해제
    freeTransactionList(&list);
    
    return 0;
}

