#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/*
 * This program generates a password between 8 and 32 characters, then
 * rate its "strength"
 *
 * "Strength" is calculated emulating http://www.passwordmeter.com/
 * "additions"
 *
 * "Deductions" are calculated substracting:
 *    - repetitions * 2
 *    - correlatives * 4
 *
 */

#define MAX_LEN 32
#define MIN_LEN 8

int max_strength(int lon);
int str_consecutive(char* str);
int str_no_repeat(char *str_result, char *str_origin);
int password_strength(char * p, int pswlon);

int main() {
    int length = 0;
    int rnum = 0;

    char password[MAX_LEN + 1] = {0};

    srand(time(NULL));

    scanf("%d", &length);

    if ((length < MIN_LEN) || (length > MAX_LEN)) {
        printf("The length must be between 8 and 32 characters!\n");
        return 1;
    }

    // only this piece of code generates password...
    for(int p = 0; p < length; p++) {
        rnum = rand() % 93 + 33;
        password[p] = rnum;
    }

    printf("Generated password: %s\n",password);
    printf("Password strentgth:\n");
    printf(" - %d / %d\n",
            password_strength(password, length),
            max_strength(length));
    printf(" - %.2f%%\n",
            password_strength(password, length)
            / (1.0 * max_strength(length)) * 100.0);

    return 0;
}

/*
 * Functions to calculate strength
 */

// returns number of consecutive digits or letters in a string
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

/*
 * Builds a string "str_result" with the contents of "str_origin"
 * without duplicated characters
 *
 * returns: length of str_result
 *
 * ej.:
 *
 *    char* alfa = "abaccd";
 *    char beta[7] = {0};
 *    int a = str_no_repeat(beta, alfa);
 *
 *    -> beta:  abcd
 *    -> a = 4
 *
 */
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

/*
 * Calculates password strength
 *
 * plus:
 *
 * +4 each character
 * +4 each number
 * +4 each symbol
 * +2 each symbol not in the beginning nor end
 * +(len - #uppercase) * 2 -if any
 * +(len - #lowercase) * 2 -if any
 * +2 each satisfied requirement:
 *    . len >= 8
 *    . uppercase letters
 *    . lowercase letters
 *    . numbers
 *    . symbols
 *
 * minus:
 *
 * -4 each pair of correlative symbols
 * -2 each duplicate symbol
 */
int password_strength(char * str, int pswlon){
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

// returns max password strength for a given password length
int max_strength(int lon) {
    return 8 + lon * 4
            + (lon - 1) * 2 * 2
            + (lon - 2) * 6
            + (lon - 2) * 2;
}
