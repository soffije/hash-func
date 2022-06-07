//MD5 algorithm on C

#include <stdio.h>

//пустая строка, дополненная 1 битом и нулями, пока она не станет 512 байтов длиной как 16 32-битных слов
unsigned int message[16] = { 0x00000080, 0x00000000, 0x00000000, 0x00000000,
                            0x00000000, 0x00000000, 0x00000000, 0x00000000,
                            0x00000000, 0x00000000, 0x00000000, 0x00000000,
                            0x00000000, 0x00000000, 0x00000000, 0x00000000 };

//величины сдвигов для каждой операции
unsigned int shiftAmounts[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                                 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
                                 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                                 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

//таблица констант 
unsigned int partsOfSines[64] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                                 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                                 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                                 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                                 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                                 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                                 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                                 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                                 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                                 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                                 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                                 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                                 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                                 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                                 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                                 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

//сдвиг влево х на n бит
unsigned leftRotate(unsigned x, int n) {
    return (x << n) | (x >> (32 - n)); //в х<<n последние n битов равны 0, потому, чтобы наконец поставить первые 3 бита x, выполняем побитовое или x<<n с x>>(32 - n)
}

//чтоб печатать с обратным порядком байтов, ибо входные байты представлены строкой из бит, где первый бит — старший (big-endian)
void printReverseEndian(unsigned n) {
    printf("%02x%02x%02x%02x", n & 0xff, (n >> 8) & 0xff, (n >> 16) & 0xff, n >> 24);
}

int main() {

    unsigned int a0, b0, c0, d0,
        A, B, C, D,
        i, F, g, bufD;

    //инициализация переменных
    a0 = 0x67452301;
    b0 = 0xefcdab89;
    c0 = 0x98badcfe;
    d0 = 0x10325476;

    //и помещаем их в цикл для каждого 512-битного фрагмента
    A = a0;
    B = b0;
    C = c0;
    D = d0;

    //основной цикл для каждого "кусочка"
    for (i = 0; i < 64; i++) {
        if (i < 16) {
            F = (B & C) | (~B & D);
            g = i;
        }
        else if (i < 32) {
            F = (D & B) | (~D & C);
            g = (5 * i + 1) % 16;
        }
        else if (i < 48) {
            F = B ^ C ^ D;
            g = (3 * i + 5) % 16;
        }
        else {
            F = C ^ (B | ~D);
            g = (7 * i) % 16;
        }
        bufD = D;
        D = C;
        C = B;
        B += leftRotate((A + F + partsOfSines[i] + message[g]), shiftAmounts[i]); //выполняем битовый сдвиг
        A = bufD;
    }

    //прибавляем результат текущего "кусочка" к общему результату
    a0 += A;
    b0 += B;
    c0 += C;
    d0 += D;

    //результат в формате little-endian
    printReverseEndian(a0);
    printReverseEndian(b0);
    printReverseEndian(c0);
    printReverseEndian(d0);

    return 0;
}
