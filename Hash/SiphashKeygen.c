/*
 * name: SipHash 哈希算法密钥生成
 * description:
 *  由输入的字符串生成密钥（只考虑前128位），虽然方便，但随机性可能没有随机生成的好
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

char *s_gets(char *dst, int n);

int main(void) {
    char buffer[255] = {0};
    s_gets(buffer, 255);

    uint64_t k_0 = 0;
    uint64_t k_1 = 0;

    memcpy(&k_0, buffer, 8);
    memcpy(&k_1, buffer + 8, 8);

    printf("k_0 = 0x%016lx\n", k_0);
    printf("k_1 = 0x%016lx\n", k_1);
}

char *s_gets(char *dst, int n) {
    char *ret = fgets(dst, n, stdin);
    int i = 0;
    if (ret != NULL) {
        while (dst[i] != '\n' && dst[i] != '\0') {
            i++;
        }
        if (dst[i] == '\n') {
            dst[i] = '\0';
        } else {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
        }
    }

    return ret;
}
