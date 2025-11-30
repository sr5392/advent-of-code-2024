#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST_SIZE 1000
#define INPUT_FILE_PATH "./input.txt"

int peek(FILE* fp) {
    const int c = fgetc(fp);
    if (c != EOF) ungetc(c, fp);
    return c;
}

int advance(FILE* fp) {
    return fgetc(fp);
}

void read_lists(FILE* fp, int* list_1, int* list_2) {
    size_t list_index = 0;
    char buffer[8] = {0};
    size_t buffer_index = 0;

    while (true) {
        const int c = advance(fp);
        if (c == EOF) return;

        if (list_index >= LIST_SIZE) {
            printf("List size exceeded");
            exit(EXIT_FAILURE);
        }

        switch (c) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
                buffer[buffer_index] = (char) c;
                ++buffer_index;

                if (peek(fp) == '\t' || peek(fp) == ' ')
                    list_1[list_index] = atoi(buffer);

                if (peek(fp) == '\r')
                    list_2[list_index] = atoi(buffer);
                break;
            case ' ':
            case '\t':
                memset(buffer, 0, sizeof(buffer));
                buffer_index = 0;
                break;
            case '\r':
                break;
            case '\n':
                memset(buffer, 0, sizeof(buffer));
                buffer_index = 0;
                ++list_index;
                break;
            default:
                printf("%d: ", c);
                printf("Invalid list format");
                exit(EXIT_FAILURE);
        }
    }
}

int cmp(const void* a, const void* b) {
    return *((int*) a) - *((int*) b);
}

void sort_list_ascending(int* list) {
    qsort(list, LIST_SIZE, sizeof(int), cmp);
}

int get_list_distance(int* list_1, int* list_2) {
    sort_list_ascending(list_1);
    sort_list_ascending(list_2);
    int list_distance = 0;
    for (int i = 0; i < LIST_SIZE; ++i) {
        list_distance += abs(list_1[i] - list_2[i]);
    }
    return list_distance;
}

int part_1(void) {
    int list_1[LIST_SIZE] = {0};
    int list_2[LIST_SIZE] = {0};
    FILE* fp = fopen(INPUT_FILE_PATH, "rb");
    if (!fp) {
        perror("Unable to open file");
    }
    read_lists(fp, list_1, list_2);
    fclose(fp);
    return get_list_distance(list_1, list_2);
}

int get_similarity_score(const int* list_1, const int* list_2) {
    int similarity_score = 0;
    for (int i = 0; i < LIST_SIZE; ++i) {
        const int location_id = list_1[i];
        int similarity_count = 0;
        for (int j = 0; j < LIST_SIZE; ++j) {
            if (list_2[j] == location_id) {
                ++similarity_count;
            }
        }
        similarity_score += location_id * similarity_count;
    }
    return similarity_score;
}

int part_2(void) {
    int list_1[LIST_SIZE] = {0};
    int list_2[LIST_SIZE] = {0};
    FILE* fp = fopen(INPUT_FILE_PATH, "rb");
    if (!fp) {
        perror("Unable to open file");
    }
    read_lists(fp, list_1, list_2);
    fclose(fp);
    return get_similarity_score(list_1, list_2);
}

int main(void) {
    printf("Part One: %d\n", part_1());
    printf("Part Two: %d\n", part_2());
    return 0;
}