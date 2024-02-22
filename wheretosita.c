/* 
COP 3502C Assignment 2
This program is written by: Izaac Plambeck 
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PEOPLE 10

int n, p;
char names[MAX_PEOPLE][20];
int has_popcorn[MAX_PEOPLE];
bool not_adjacent[MAX_PEOPLE][MAX_PEOPLE] = {false};
bool used[MAX_PEOPLE] = {false};
int perm[MAX_PEOPLE];
int valid_count = 0;

bool can_sit_together(int idx1, int idx2) {
    return !not_adjacent[idx1][idx2];
}

bool can_have_popcorn(int idx) {
    if (idx == 0) {
        return has_popcorn[perm[idx]] || has_popcorn[perm[idx + 1]];
    } else if (idx == n - 1) {
        return has_popcorn[perm[idx]] || has_popcorn[perm[idx - 1]];
    } else {
        return has_popcorn[perm[idx]] || has_popcorn[perm[idx + 1]] || has_popcorn[perm[idx - 1]];
    }
}

bool is_valid_permutation() {
    for (int i = 0; i < n; i++) {
        if (!can_have_popcorn(i)) {
            return false;
        }
        if (i > 0 && !can_sit_together(perm[i], perm[i - 1])) {
            return false;
        }
    }
    return true;
}

void permute(int level) {
    if (level == n) {
        if (is_valid_permutation()) {
            valid_count++;
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            perm[level] = i;
            used[i] = true;
            permute(level + 1);
            used[i] = false;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <inputfile>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    fscanf(fp, "%d %d", &n, &p);
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%s %d", names[i], &has_popcorn[i]);
    }

    for (int i = 0; i < p; i++) {
        char name1[20], name2[20];
        int idx1 = -1, idx2 = -1;
        fscanf(fp, "%s %s", name1, name2);
        for (int j = 0; j < n; j++) {
            if (strcmp(name1, names[j]) == 0) idx1 = j;
            if (strcmp(name2, names[j]) == 0) idx2 = j;
        }
        not_adjacent[idx1][idx2] = true;
        not_adjacent[idx2][idx1] = true;
    }

    fclose(fp);

    permute(0);

    printf("%d\n", valid_count);

    return 0;
}
