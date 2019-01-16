#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 32
#define MIN_LEN 8

int max_strength(int lon);
int str_consecutive(char* str);
int str_no_repeat(char *str_result, char *str_origin);
int password_strength(char * p, int symqty, int pswlon);

int main() {
    int length = 0;
    int rnum = 0;

    char password[MAX_LEN + 1] = {0};

    srand(time(NULL));
//length = 10;
    scanf("%d", &length);

    if ((length < MIN_LEN) || (length > MAX_LEN)) {
        printf("The length must be between 8 and 32 characters!\n");
        return 1;
    }

    for(int p = 0; p < length; p++) {
        rnum = rand() % 93 + 33;
        password[p] = rnum;
    }

//    password[0] = '&';
//    password[1] = '-';
//    password[2] = 'e';
//    password[3] = 's';
//    password[4] = '!';
//    password[5] = '=';
//    password[6] = 'k';
//    password[7] = '2';
//    password[8] = '}';
//    password[9] = 'b';

    printf("%d\n", max_strength(length));

    printf("%s\n",password);
    printf("%d\n", password_strength(password, 93, length ));
    printf("%.2f%%\n", password_strength(password, 93, length ) / (1.0 * max_strength(length)) * 100.0);

    return 0;
}

int str_consecutive(char* str) {
    int consec = 0;
    if (strlen(str) > 1) {
        str++;
        while(*str){
            if (abs(*str - *(str -1)) == 1)
                consec++;
            str++;
        }
    }
    return consec;
}

int str_no_repeat(char *str_result, char *str_origin) {
    char* aux = 0;
    char* aux_ini = str_result;
    while(*str_origin) {
        aux = aux_ini;
        while(*aux) {
            if (*aux == *str_origin)
                break;
            aux++;
        }
        if (*aux == '\0') {
            *str_result = *str_origin;
            str_result++;
        }
        str_origin++;
    }

    return (str_result - aux_ini);
}

int password_strength(char * str, int symqty, int pswlon){
    char* p = str;
    int strength = 0;
    char dos[MAX_LEN + 1] = {0};

    int upper_qty = 0, lower_qty = 0, number_qty = 0, symbol_qty = 0;
    int midd_symnum_qty = 0;

    (void)str_no_repeat(dos, str);

    int correlative = str_consecutive(str);
    int diff_len = strlen(str) - strlen(dos);

    while(*str) {
        if ((*str >= 'A') && (*str <= 'Z')){
            upper_qty++;
        }
        else
        if ((*str >= 'a') && (*str <= 'z')){
            lower_qty++;
        }
        else
        if ((*str >= '0') && (*str <= '9')){
            number_qty++;
            if ((str - p > 0) && (*(str + 1) != '\0')) {
                midd_symnum_qty++;
            }
        }
        else
        {
            symbol_qty++;
            if ((str - p > 0) && (*(str + 1) != '\0')) {
                midd_symnum_qty++;
            }
        }

        str++;
    }

/*    printf("%d\n", pswlon * 4);
    printf("%d\n", upper_qty ? ((pswlon - upper_qty) * 2) : 0);
    printf("%d\n", lower_qty ? ((pswlon - lower_qty) * 2) : 0);
    printf("%d\n", (number_qty * 4));
    printf("%d\n", (symbol_qty * 6));
    printf("%d\n", midd_symnum_qty * 2);
    printf("%d\n", (
                           (upper_qty > 0) + (lower_qty > 0) +
                           (number_qty > 0) + (symbol_qty > 0) +
                           (pswlon >= 8)   ) * 2);*/

    strength += (pswlon * 4);
    strength += (upper_qty ? ((pswlon - upper_qty) * 2) : 0);
    strength += (lower_qty ? ((pswlon - lower_qty) * 2) : 0);
    strength += (number_qty * 4);
    strength += (symbol_qty * 6);
    strength += midd_symnum_qty * 2;
    strength += ( (upper_qty > 0) + (lower_qty > 0) +
                  (number_qty > 0) + (symbol_qty > 0) +
                  (pswlon >= 8)
                  ) * 2;

    strength -= correlative * 4;
    strength -= diff_len * 2;

    return strength;
}

int max_strength(int lon) {
    return 8 + lon * 4 + (lon - 1) * 2 * 2 + (lon - 2) * 6 + (lon - 2) * 2;
}
