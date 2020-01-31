#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./distq.h"
// #include "./ldistq.h"

unsigned char *mygets(unsigned char *s, int n) {
    if (fgets(s, n, stdin) == NULL) return NULL;
    char *ln = strchr(s, '\n');
    if (ln) *ln = '\0';
    else
        while (1) {
            unsigned char c = getchar();
            if (c == '\n' || c == EOF) break;
        }
    return s;
}

int main() {
    int X, M, N;
    scanf("%d %d %d", &X, &M, &N);
    getchar();

    char *P, *T;
    P = (char *) malloc(sizeof(char) * (M + 1));
    T = (char *) malloc(sizeof(char) * (N + 1 + M));
    T[N + M] = '\0';

    for (int x = 0; x < X; x++) {
        mygets(P, M + 1);
        mygets(T, N + 1);
        printf("%d\n", search((unsigned char *) P, M, (unsigned char *) T, N));
    }
    return 0;
}
