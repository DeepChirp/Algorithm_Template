/*
 * name: 快速求模n下的a ^ b
 * description:
 *
 * 时间复杂度：$O(\log{b})$
 */

typedef long long number;

// a ^ b mod n
number modular_exponentitaion(number a, number b, number n) {
    number result = 1;
    number temp = a % n;

    while (b > 0) {
        number current_digit = b % 2;

        if (current_digit == 1) {
            result = (result * temp) % n;
        }

        temp = (temp * temp) % n;

        b /= 2;
    }

    return result;
}
