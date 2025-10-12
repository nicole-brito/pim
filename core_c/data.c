#include "structs.h"

// Adicionar nota (escreve em ../data/notas.txt: id|aluno_id|nota|data)
void adicionar_nota(int id, int aluno_id, float nota, const char* data_str) {
    FILE* file = fopen("../data/notas.txt", "a");
    if (file) {
        fprintf(file, "%d|%d|%.2f|%s\n", id, aluno_id, nota, data_str);
        fclose(file);
    }
}

// Ler alunos (de ../data/alunos.txt: id|nome|matricula)
Aluno* ler_alunos(int* count) {
    FILE* file = fopen("../data/alunos.txt", "r");
    if (!file) {
        *count = 0;
        return NULL;
    }

    Aluno* alunos = malloc(100 * sizeof(Aluno));  // Buffer fixo
    *count = 0;
    char line[100];
    while (fgets(line, sizeof(line), file) && *count < 100) {
        line[strcspn(line, "\n")] = 0;
        sscanf(line, "%d|%[^|]|%[^|]", &alunos[*count].id, alunos[*count].nome, alunos[*count].matricula);
        (*count)++;
    }
    fclose(file);
    return alunos;
}

// Função exportar para Python (escreve alunos em arquivo)
void exportar_alunos_para_txt(const char* filename) {
    int count;
    Aluno* alunos = ler_alunos(&count);
    FILE* file = fopen(filename, "w");
    if (file && alunos) {
        for (int i = 0; i < count; i++) {
            fprintf(file, "%d|%s|%s\n", alunos[i].id, alunos[i].nome, alunos[i].matricula);
        }
        fclose(file);
    }
    if (alunos) free(alunos);
}