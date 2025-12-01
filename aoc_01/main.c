#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE_PATH "./input.txt"

static size_t list_size = 1000;
static size_t list_count = 0;

int peek(FILE* fp) {
    const int c = fgetc(fp);
    if (c != EOF) ungetc(c, fp);
    return c;
}

int advance(FILE* fp) {
    return fgetc(fp);
}

int number(FILE* fp) {
    size_t buffer_size = 16;
    char* buffer = malloc(buffer_size);
    if (!buffer) {
        fclose(fp);
        printf("Error allocating memory for buffer");
        exit(EXIT_FAILURE);
    }
    size_t buffer_index = 0;
    while (isdigit(peek(fp))) {
        if (buffer_index >= buffer_size - 1) {
            buffer_size *= 2;
            char* tmpBuffer = realloc(buffer, buffer_size);
            if (!tmpBuffer) {
                free(buffer);
                fclose(fp);
                printf("Error reallocating memory for buffer");
                exit(EXIT_FAILURE);
            }
            buffer = tmpBuffer;
        }
        const char c = (char) advance(fp);
        buffer[buffer_index] = c;
        ++buffer_index;
    }
    buffer[buffer_index] = '\0';
    const int num = atoi(buffer);
    free(buffer);
    return num;
}

void read_lists(int** list_1, int** list_2) {
    list_count = 0;
    FILE* fp = fopen(INPUT_FILE_PATH, "rb");
    if (!fp) {
        free(*list_1);
        free(*list_2);
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }
    int number_count = 0;
    while (true) {
        const int c = peek(fp);
        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                advance(fp);
                break;
            case EOF:
                if (feof(fp)) {
                    fclose(fp);
                    return;
                }
                if (ferror(fp)) {
                    free(*list_1);
                    free(*list_2);
                    fclose(fp);
                    printf("Error reading file");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                if (isdigit(c) && number_count < 2) {
                    if (list_count >= list_size) {
                        list_size *= 2;
                        int* tmp_list_1 = realloc(*list_1, list_size * sizeof(int));
                        if (!tmp_list_1) {
                            free(*list_1);
                            free(*list_2);
                            fclose(fp);
                            printf("Error reallocating memory for list 1");
                            exit(EXIT_FAILURE);
                        }
                        *list_1 = tmp_list_1;
                        int* tmp_list_2 = realloc(*list_2, list_size * sizeof(int));
                        if (!tmp_list_2) {
                            free(*list_1);
                            free(*list_2);
                            fclose(fp);
                            printf("Error reallocating memory for list 2");
                            exit(EXIT_FAILURE);
                        }
                        *list_2 = tmp_list_2;
                        for (size_t i = list_count; i < list_size; ++i) {
                            (*list_1)[i] = 0;
                            (*list_2)[i] = 0;
                        }
                    }
                    const int num = number(fp);
                    if (number_count == 0) {
                        (*list_1)[list_count] = num;
                        ++number_count;
                        continue;
                    }
                    if (number_count == 1) {
                        (*list_2)[list_count] = num;
                        ++number_count;
                    }
                    continue;
                }
                if (c == '\n' && number_count == 2) {
                    ++list_count;
                    number_count = 0;
                    advance(fp);
                    continue;
                }
                free(*list_1);
                free(*list_2);
                fclose(fp);
                printf("Invalid list format");
                exit(EXIT_FAILURE);
        }
    }
}

int compare(const void* a, const void* b) {
    return *((int*) a) - *((int*) b);
}

void sort_list_ascending(int* list) {
    qsort(list, list_count, sizeof(int), compare);
}

int get_list_distance(int* list_1, int* list_2) {
    sort_list_ascending(list_1);
    sort_list_ascending(list_2);
    int list_distance = 0;
    for (size_t i = 0; i < list_count; ++i) {
        list_distance += abs(list_1[i] - list_2[i]);
    }
    return list_distance;
}

int part_1(void) {
    int* list_1 = calloc(list_size, sizeof(int));
    if (!list_1) {
        printf("Error allocating memory for list 1");
        exit(EXIT_FAILURE);
    }
    int* list_2 = calloc(list_size, sizeof(int));
    if (!list_2) {
        free(list_1);
        printf("Error allocating memory for list 2");
        exit(EXIT_FAILURE);
    }
    read_lists(&list_1, &list_2);
    const int list_distance = get_list_distance(list_1, list_2);
    free(list_1);
    free(list_2);
    return list_distance;
}

int get_similarity_score(const int* list_1, const int* list_2) {
    int similarity_score = 0;
    for (size_t i = 0; i < list_count; ++i) {
        const int location_id = list_1[i];
        int similarity_count = 0;
        for (size_t j = 0; j < list_count; ++j) {
            if (list_2[j] == location_id) {
                ++similarity_count;
            }
        }
        similarity_score += location_id * similarity_count;
    }
    return similarity_score;
}

int part_2(void) {
    int* list_1 = calloc(list_size, sizeof(int));
    if (!list_1) {
        printf("Error allocating memory for list 1");
        exit(EXIT_FAILURE);
    }
    int* list_2 = calloc(list_size, sizeof(int));
    if (!list_2) {
        free(list_1);
        printf("Error allocating memory for list 2");
        exit(EXIT_FAILURE);
    }
    read_lists(&list_1, &list_2);
    const int similarity_score = get_similarity_score(list_1, list_2);
    free(list_1);
    free(list_2);
    return similarity_score;
}

int main(void) {
    printf("Part One: %d\n", part_1());
    printf("Part Two: %d\n", part_2());
    return 0;
}
