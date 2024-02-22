/* 
COP 3502C Assignment 2 
This program is written by: Izaac Plambeck 
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Global variables to hold data for attendees, popcorn status, and seating restrictions
int n, p;
char names[10][20];
int has_popcorn[10];
bool not_adjacent[10][10] = {false};
bool used[10] = {false};
int perm[10];
bool found_solution = false;

// Function to check if two attendees can sit together
bool can_sit_together(int idx1, int idx2) {
    return !not_adjacent[idx1][idx2];
}

// Function to check if an attendee has access to popcorn
bool can_have_popcorn(int idx) {
    if (idx == 0) {
        return has_popcorn[perm[idx]] || has_popcorn[perm[idx + 1]];
    } else if (idx == n - 1) {
        return has_popcorn[perm[idx]] || has_popcorn[perm[idx - 1]];
    } else {
        return has_popcorn[perm[idx]] || has_popcorn[perm[idx + 1]] || has_popcorn[perm[idx - 1]];
    }
}

// Function to verify if a seating permutation is valid based on the provided constraints
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

// Recursive function to generate and verify seating permutations
void permute(int level) {
    if (found_solution) {
        return;
    }

    if (level == n) {
        if (is_valid_permutation()) {
            for (int i = 0; i < n; i++) {
                printf("%s\n", names[perm[i]]);
            }
            found_solution = true;
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
        fscanf(fp, "%s %s", name1, name2);
        for (int idx1 = 0; idx1 < n; idx1++) {
            for (int idx2 = 0; idx2 < n; idx2++) {
                if (strcmp(name1, names[idx1]) == 0 && strcmp(name2, names[idx2]) == 0) {
                    not_adjacent[idx1][idx2] = true;
                    not_adjacent[idx2][idx1] = true;
                }
            }
        }
    }

    fclose(fp);

    permute(0);

    if (!found_solution) {
        printf("No valid seating arrangement found.\n");
    }

    return 0;
}
