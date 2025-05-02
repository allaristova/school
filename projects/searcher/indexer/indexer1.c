#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h> 
#include <stdbool.h>

#define MAX_WORDS 1000000
#define MAX_DOCS 100
#define W_LEN 80

bool
file_exists(char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

//compress
typedef struct Node {
    unsigned char symb;
    int freq;
    struct Node *next;
    struct Node *c0;
    struct Node *c1;
} Node;

typedef struct Table {
    unsigned char symb;
    unsigned long int bits;
    unsigned char nbits;
    struct Table *next;
} Table;

Table *table;

void count(Node** list, unsigned char c);
void sort(Node** list);
void ins_sort(Node** head, Node *e);
void del_tree(Node *n);
void create_tab(Node *n, int len, int code);
void insert_tab(unsigned char c, int len, int code);
Table *find(Table *table, unsigned char c);

typedef struct {
    char word[80];
    char *docs[MAX_DOCS];
    int doc_count;
} Word;

Word storage[MAX_WORDS];
int word_cnt = 0;
char *file_names[MAX_DOCS];

int
comp(const void *a, const void *b) {
    return strcmp(((Word *)a)->word, ((Word *)b)->word);
}

void
insert(char *word, int doc_id) {
    int i;
    for (i = 0; i < word_cnt; i++) {
        if (strcmp(storage[i].word, word) == 0) {
            for (int j = 0; j < storage[i].doc_count;j++) {
                if (strcmp(storage[i].docs[j], file_names[doc_id]) == 0) {
                    return;
                }
            }
            storage[i].docs[storage[i].doc_count++] = file_names[doc_id];
            return;
        }
    }
    strcpy(storage[word_cnt].word, word);
    storage[word_cnt].docs[0] = file_names[doc_id];
    storage[word_cnt].doc_count = 1;
    word_cnt++;
}


int
main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }
    int doc_id = 0;
    for (int i = 1; i < argc; i++) {
        if (!file_exists(argv[i])) {
            printf("No such file.\n");
            exit(1);
        }
        FILE *inp = fopen(argv[i], "r");
        fseek(inp,0,SEEK_END);
        long pos=ftell(inp);
        if (inp == NULL || !(pos > 0)) {
            printf("NULL\n");
            exit(1);
        } else {
        file_names[doc_id] = argv[i];
        FILE *fp = fopen(argv[i], "r");
        char buffer[100000];
        const char s[100] =  " \t\n!;:~'_@#${}|^=+&*(.><,?)]%%\\[";
        while (fgets(buffer, sizeof(buffer), fp)) {
//            char *word = strtok(buffer, " \t\n");
            char *word = strtok(buffer, s);
            while (word != NULL) {
                for (int j = 0; word[j]; j++) {
                    word[j] = tolower(word[j]);
                }
                insert(word, doc_id);
                word = strtok(NULL, s);
            }
        }
        fclose(fp);
        doc_id++;
        }
    }
    FILE *output = fopen("inv.txt", "w");
    for (int i = 0; i < word_cnt; i++) {
        fprintf(output, "%s:", storage[i].word);
        for (int j = 0; j < storage[i].doc_count; j++) {
            fprintf(output, " %s", storage[i].docs[j]);
        }
        fprintf(output, "\n");
    }
    fclose(output);
    // compress part
    Node *list, *tree, *new_node;
    FILE *outp;
    unsigned char c;
    Table *tb;
    int tb_el;
    long int inp_size = 0;
    unsigned long int d_word;
    int n_dbits;
    list = NULL;
    
    // Freq
    FILE *inp = fopen("inv.txt", "r");
    do {
        c = fgetc(inp);
        if (feof(inp)) {
            break;
        }
        inp_size++;
        count(&list, c);
    } while (1);

    fclose(inp);
    sort(&list);

    tree = list;
    while (tree && tree->next) {
        new_node = (Node*)calloc(1, sizeof(Node));
        new_node->symb = 0;
        new_node->c1 = tree;
        tree = tree->next;
        new_node->c0 = tree;
        tree = tree->next;
        new_node->freq = new_node->c1->freq + new_node->c0->freq;
        ins_sort(&tree, new_node);
    }

    table = NULL;
    create_tab(tree, 0, 0);
    outp = fopen("inv.txt.compress", "wb");
    fwrite(&inp_size, sizeof(long int), 1, outp);
    tb_el = 0;
    tb = table;
    while (tb) {
        tb_el++;
        tb = tb->next;
    }
    fwrite(&tb_el, sizeof(int), 1, outp);

    tb = table;
    while (tb) {
        fwrite(&tb->symb, sizeof(char), 1, outp);
        fwrite(&tb->bits, sizeof(unsigned long int), 1, outp);
        fwrite(&tb->nbits, sizeof(char), 1, outp);
        tb = tb->next;
    }

    // Encoding
    inp = fopen("inv.txt", "r");
    d_word = 0;
    n_dbits = 0;
    do {
        c = fgetc(inp);
        if (feof(inp)) {
            break;
        }
        tb = find(table, c);
        while (n_dbits + tb->nbits > 32) {
            c = d_word >> (n_dbits - 8);
            fwrite(&c, sizeof(char), 1, outp);
            n_dbits -= 8;
        }
        d_word <<= tb->nbits;
        d_word |= tb->bits;
        n_dbits += tb->nbits;
    } while (1);

    while (n_dbits > 0) {
        if (n_dbits >= 8) {
            c = d_word >> (n_dbits - 8);
        } else {
            c = d_word << (8 - n_dbits);
        }
        fwrite(&c, sizeof(char), 1, outp);
        n_dbits -= 8;
    }

    fclose(inp);
    fclose(outp);
    del_tree(tree);

    // del table
    while (table) {
        tb = table;
        table = tb->next;
        free(tb);
    }

    int fl;
    fl = remove("inv.txt");
    return 0;
}

void
del_tree(Node *n) {
    if (n->c0) {
        del_tree(n->c0);
    }
    if (n->c1) {
        del_tree(n->c1);
    }
    free(n);
}

Table
*find(Table *table, unsigned char c) {
    Table *el;
    el = table;
    while (el && el->symb != c) {
        el = el->next;
    }
    return el;
}

void
insert_tab(unsigned char c, int len, int code) {
    Table *el, *new_node, *cur;
    el = (Table*)calloc(1, sizeof(Table));
    el->symb = c;
    el->bits = code;
    el->nbits = len;
    if (!table) {
        table = el;
        table->next = NULL;
    } else {
        new_node = table;
        cur = NULL;
        while (new_node && new_node->symb < el->symb) {
            cur = new_node;
            new_node = new_node->next;
        }
        el->next = new_node;
        if (cur) {
            cur->next = el;
        } else {
            table = el;
        }
    }
}

void
create_tab(Node *root, int len, int code) {
    if (root->c1) {
        create_tab(root->c1, len + 1, (code << 1) | 1);
    }
    if (root->c0) {
        create_tab(root->c0, len + 1, code << 1);
    }
    if (!root->c1 && !root->c0) {
        insert_tab(root->symb, len, code);
    }
}

void
ins_sort(Node **head, Node *elem) {
    Node *new_node, *cur;
    if (!*head) {
        *head = elem;
        (*head)->next = NULL;
    } else {
        new_node = *head;
        cur = NULL;
        while (new_node && new_node->freq < elem->freq) {
            cur = new_node;
            new_node = new_node->next;
        }
        elem->next = new_node;
        if (cur) {
            cur->next = elem;
        } else {
            *head = elem;
        }
    }
}

void
sort(Node **list) {
    Node *list1, *prev;
    if (!*list) {
        return;
    }
    list1 = *list;
    *list = NULL;
    while (list1) {
        prev = list1;
        list1 = prev->next;
        ins_sort(list, prev);
    }
}

void
count(Node **list, unsigned char c) {
    Node *new_node, *prev, *cur;
    if (!*list) {
        *list = (Node *)calloc(1, sizeof(Node));
        (*list)->symb = c;
        (*list)->freq = 1;
        (*list)->next = (*list)->c0 = (*list)->c1 = NULL;
    } else {
        new_node = *list;
        prev = NULL;
        while (new_node && new_node->symb < c) {
            prev = new_node;
            new_node = new_node->next;
        }
        if (new_node && new_node->symb == c) {
            new_node->freq++;
        } else {
            cur = (Node *)calloc(1, sizeof(Node));
            cur->symb = c;
            cur->freq = 1;
            cur->c0 = cur->c1 = NULL;
            cur->next = new_node;
            if (prev) {
                prev->next = cur;
            } else {
                *list = cur;
            }
        }
    }
}
