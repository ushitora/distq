#define HASH_SIZE 65536
#define Q 3

// q = 1
// #define HASH(x, j) x[j]

// q = 2
// #define HASH(x, j) (x[j - 1] << 2) + x[j]

// q = 3
#define HASH(x, j) (x[j - 2] << 4) + (x[j - 1] << 2) + x[j]

// q = 4
// #define HASH(x, j) (x[j - 3] << 6) + (x[j - 2] << 4) + (x[j - 1] << 2) + x[j]

// q = 5
// #define HASH(x, j) \
    // (x[j - 4] << 8) + (x[j - 3] << 6) + (x[j - 2] << 4) + (x[j - 1] << 2) + x[j]

// q = 6
// #define HASH(y, j)                                                           \
    // (y[j - 5] << 10) + (y[j - 4] << 8) + (y[j - 3] << 6) + (y[j - 2] << 4) + \
        // (y[j - 1] << 2) + y[j]

// q = 7
// #define HASH(y, j)                                                            \
    // (y[j - 6] << 12) + (y[j - 5] << 10) + (y[j - 4] << 8) + (y[j - 3] << 6) + \
        // (y[j - 2] << 4) + (y[j - 1] << 2) + y[j]

// q = 8
// #define HASH(y, j)                                                             \
    // (y[j - 7] << 14) + (y[j - 6] << 12) + (y[j - 5] << 10) + (y[j - 4] << 8) + \
        // (y[j - 3] << 6) + (y[j - 2] << 4) + (y[j - 1] << 2) + y[j]

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

    for (i = Q - 1; i < m; ++i) {
        h = HASH(p, i);
        shift[h] = m - i - 1;
    }

    // dist
    int prev[HASH_SIZE];
    int code;
    int d[m];
    for (i = 0; i < Q - 1; ++i) d[i] = 1;
    for (i = 0; i < HASH_SIZE; ++i) prev[i] = -1;
    for (i = Q - 1; i < m; ++i) {
        h = HASH(p, i);

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

    while (ip < n) {
        if (j <= 0) {
            while (1) {
                tHash = HASH(t, ip);
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

            if (j == m) {
                count++;
            }

            j = kmpNext[j];
        }

        ip = i + mMinus1 - j;
    }

    return count;
}
