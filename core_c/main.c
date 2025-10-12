#include "structs.h"

// Declarações externas (protótipos das funções em .c)
extern int autenticar(const char* usuario, const char* senha, int* eh_professor);
extern void adicionar_nota(int id, int aluno_id, float nota, const char* data_str);
extern Aluno* ler_alunos(int* count);
extern void exportar_alunos_para_txt(const char* filename);

int main() {
    int eh_prof;
    if (autenticar("prof1", "123", &eh_prof)) {
        printf("Login OK! É professor: %d\n", eh_prof);
        adicionar_nota(1, 1, 8.5, "2023-10-01");  // Exemplo
        int count;
        Aluno* alunos = ler_alunos(&count);
        printf("Alunos carregados: %d\n", count);
        if (alunos) free(alunos);
    } else {
        printf("Login falhou!\n");
    }
    return 0;
}