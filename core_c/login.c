#include "structs.h"

// Função de autenticação (lê de ../data/usuarios.txt: usuario|senha|tipo)
int autenticar(const char* usuario, const char* senha, int* eh_professor) {
    FILE* file = fopen("../data/usuarios.txt", "r");
    if (!file) return 0;

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char u[50], s[20], tipo[10];
        // Remove \n do final
        line[strcspn(line, "\n")] = 0;
        sscanf(line, "%[^|]|%[^|]|%s", u, s, tipo);
        if (strcmp(u, usuario) == 0 && strcmp(s, senha) == 0) {
            *eh_professor = (strcmp(tipo, "prof") == 0 ? 1 : 0);
            fclose(file);
            return 1;  // Sucesso
        }
    }
    fclose(file);
    return 0;  // Falha
}