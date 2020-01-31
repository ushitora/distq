#define HASH_SIZE 65536
#define Q 3

// q = 2
// #define HASH(x, j) (x[j - 1] << 2) + x[j]
// #define REHASH(a, b, h) ((((h) - (a << 2)) << 2) + (b))

// q = 3
#define HASH(x, j) (x[j - 2] << 4) + (x[j - 1] << 2) + x[j]
#define REHASH(a, b, h) ((((h) - (a << 4)) << 2) + (b))

// q = 4
// #define HASH(x, j) (x[j - 3] << 6) + (x[j - 2] << 4) + (x[j - 1] << 2) + x[j]
// #define REHASH(a, b, h) ((((h) - (a << 6)) << 2) + (b))

// q = 5
// #define HASH(x, j) \
    // (x[j - 4] << 8) + (x[j - 3] << 6) + (x[j - 2] << 4) + (x[j - 1] << 2) + x[j]
// #define REHASH(a, b, h) ((((h) - (a << 8)) << 2) + (b))

// q = 6
// #define HASH(y, j)                                                           \
    // (y[j - 5] << 10) + (y[j - 4] << 8) + (y[j - 3] << 6) + (y[j - 2] << 4) + \
        // (y[j - 1] << 2) + y[j]
// #define REHASH(a, b, h) ((((h) - (a << 10)) << 2) + (b))

// q = 7
// #define HASH(y, j)                                                            \
    // (y[j - 6] << 12) + (y[j - 5] << 10) + (y[j - 4] << 8) + (y[j - 3] << 6) + \
        // (y[j - 2] << 4) + (y[j - 1] << 2) + y[j]
// #define REHASH(a, b, h) ((((h) - (a << 12)) << 2) + (b))

// q = 8
// #define HASH(y, j)                                                             \
    // (y[j - 7] << 14) + (y[j - 6] << 12) + (y[j - 5] << 10) + (y[j - 4] << 8) + \
        // (y[j - 3] << 6) + (y[j - 2] << 4) + (y[j - 1] << 2) + y[j]
// #define REHASH(a, b, h) ((((h) - (a << 14)) << 2) + (b))

int search(unsigned char *p, int m, unsigned char *t, int n) {
    if (m < Q) return -1;

    const int mMinus1 = m - 1;
    const int mMinus2 = m - 2;

    // KMP_Shift
    int i = 0, j = -1;
    int kmpNext[m + 1];
    kmpNext[0] = -1;
    while (i < m) {
        while ((j > -1) && (p[i] != p[j])) j = kmpNext[j];
        if (i < m && p[++i] == p[++j])
            kmpNext[i] = kmpNext[j];
        else
            kmpNext[i] = j;
    }

    int kmpShift[m + 1];
    for (j = 0; j < m + 1; ++j) {
        kmpShift[j] = j - kmpNext[j];
    }

    // HQ_Shift
    int shift[HASH_SIZE];
    unsigned short h;
    for (i = 0; i < HASH_SIZE; ++i) {
        shift[i] = m - Q + 1;
    }

    h = HASH(p, Q - 1);
    shift[h] = m - Q;

    for (i = Q; i < m; ++i) {
        h = REHASH(p[i - Q], p[i], h);
        shift[h] = m - i - 1;
    }

    // dist
    int prev[HASH_SIZE];
    int code;
    int d[m];
    for (i = 0; i < Q - 1; ++i) d[i] = 1;
    for (i = 0; i < HASH_SIZE; ++i) prev[i] = -1;

    h = HASH(p, Q - 1);
    d[Q - 1] = 1;
    prev[h] = Q - 1;

    for (i = Q; i < m; ++i) {
        h = REHASH(p[i - Q], p[i], h);

        if (prev[h] == -1) {
            code = i - Q + 2;
        } else {
            code = i - prev[h];
        }
        d[i] = code;
        prev[h] = i;
    }

    for (i = 0; i < m; i++) t[n + i] = p[i];

    i = 0;
    j = 0;

    int ip = mMinus1;
    int count = 0;
    unsigned short tHash;
    const int mq = m - Q + 1;
    int pos = 0;
    int sh;
    int lastHashPos = -1;

    while (ip < n) {
        if (j <= 0) {
            while (1) {
                if (ip - lastHashPos >= Q) {
                    tHash = HASH(t, ip);
                } else {
                    for (int l = 0; l < ip - lastHashPos; ++l) {
                        tHash = REHASH(t[lastHashPos - Q + 1 + l],
                                       t[lastHashPos + 1 + l], tHash);
                    }
                }
                lastHashPos = ip;

                sh = shift[tHash];

                if (sh == mq) {
                    ip += mq;
                } else {
                    ip += sh;
                    pos = mMinus1 - sh;

                    if (p[0] == t[ip - mMinus1]) break;

                    ip += d[pos];
                }
            }

            j = 1;
            i = ip - mMinus2;

            while ((j < m) && (t[i] == p[j])) {
                ++i;
                ++j;
            }

            if (j == m && ip < n) {
                count++;
            }

            if (d[pos] > kmpShift[j] && d[pos] >= j) {
                j -= d[pos];
            } else {
                j = kmpNext[j];
            }
        } else {
            while ((j < m) && (t[i] == p[j])) {
                ++i;
                ++j;
            }

            if (j == m && ip < n) {
                count++;
            }

            j = kmpNext[j];
        }

        ip = i + mMinus1 - j;
    }

    return count;
}
