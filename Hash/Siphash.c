/*
 * name: SipHash 哈希算法（用于Rust标准库中的相关容器）
 * description:
 *  该算法需要输入一个128位的密钥（由k_0，k_1两个u64组成），对任意长度的消息生成64位的摘要值。
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define SIPHASH_COMPRESSION_ROUND 2
#define SIPHASH_FINALIZATION_ROUND 4

#define ROTL(x, b) (uint64_t)(((x) << (b)) | ((x) >> (64 - (b))))

uint64_t siphash(uint64_t k_0, uint64_t k_1, void *msg, size_t msg_len);

uint64_t siphash(uint64_t k_0, uint64_t k_1, void *msg, size_t msg_len) {
    uint64_t v_0 = k_0 ^ 0x736f6d6570736575;
    uint64_t v_1 = k_1 ^ 0x646f72616e646f6d;
    uint64_t v_2 = k_0 ^ 0x6c7967656e657261;
    uint64_t v_3 = k_1 ^ 0x7465646279746573;

    size_t normal_count = msg_len / 8;

    uint64_t *current_word_ptr = (uint64_t *)msg;

    for (size_t i = 0; i < normal_count; i++) {
        uint64_t current_word = *current_word_ptr;

        v_3 ^= current_word;

        for (int j = 0; j < SIPHASH_COMPRESSION_ROUND; j++) {
            // SipRound
            {
                v_0 += v_1;
                v_2 += v_3;

                v_1 = ROTL(v_1, 13);
                v_3 = ROTL(v_3, 16);

                v_1 ^= v_0;
                v_3 ^= v_2;

                v_0 = ROTL(v_0, 32);

                v_2 += v_1;
                v_0 += v_3;

                v_1 = ROTL(v_1, 17);
                v_3 = ROTL(v_3, 21);

                v_1 ^= v_2;
                v_3 ^= v_0;

                v_2 = ROTL(v_2, 32);
            }
        }

        v_0 ^= current_word;

        current_word_ptr++;
    }

    uint64_t final_word = 0;

    memcpy(&final_word, current_word_ptr, msg_len - normal_count * 8);

    final_word |= (msg_len % 256) << 56;

    v_3 ^= final_word;

    for (int j = 0; j < SIPHASH_COMPRESSION_ROUND; j++) {
        // SipRound
        {
            v_0 += v_1;
            v_2 += v_3;

            v_1 = ROTL(v_1, 13);
            v_3 = ROTL(v_3, 16);

            v_1 ^= v_0;
            v_3 ^= v_2;

            v_0 = ROTL(v_0, 32);

            v_2 += v_1;
            v_0 += v_3;

            v_1 = ROTL(v_1, 17);
            v_3 = ROTL(v_3, 21);

            v_1 ^= v_2;
            v_3 ^= v_0;

            v_2 = ROTL(v_2, 32);
        }
    }

    v_0 ^= final_word;

    v_2 ^= 0xff;

    for (int j = 0; j < SIPHASH_FINALIZATION_ROUND; j++) {
        // SipRound
        {
            v_0 += v_1;
            v_2 += v_3;

            v_1 = ROTL(v_1, 13);
            v_3 = ROTL(v_3, 16);

            v_1 ^= v_0;
            v_3 ^= v_2;

            v_0 = ROTL(v_0, 32);

            v_2 += v_1;
            v_0 += v_3;

            v_1 = ROTL(v_1, 17);
            v_3 = ROTL(v_3, 21);

            v_1 ^= v_2;
            v_3 ^= v_0;

            v_2 = ROTL(v_2, 32);
        }
    }

    return v_0 ^ v_1 ^ v_2 ^ v_3;
}

int main(void) {

    uint64_t k_0 = 0x0706050403020100;
    uint64_t k_1 = 0x0f0e0d0c0b0a0908;

    uint8_t buf[15] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                       0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e};

    uint64_t result = siphash(k_0, k_1, buf, 15);
    uint64_t expected = 0xa129ca6149be45e5;

    return 0;
}
