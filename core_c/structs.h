#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char nome[50];
    int id;
    char matricula[20];
    char senha[20];
} Aluno;

typedef struct {
    char nome[50];
    int id;
    char disciplina[30];
    char senha[20];
} Professor;

typedef struct {
    int id;
    char nome[50];
    int alunos_count;
    int aluno_ids[100];
} Turma;

typedef struct {
    int id;
    int aluno_id;
    float nota;
    char data[20];
} Nota;

typedef struct {
    int id;
    int aluno_id;
    char descricao[100];
    char data[20];
} Atividade;

typedef struct {
    int id;
    int aluno_id;
    char data[20];
} Falta;

// Protótipos das funções
int autenticar(const char* usuario, const char* senha, int* eh_professor);
void adicionar_nota(int id, int aluno_id, float nota, const char* data_str);
Aluno* ler_alunos(int* count);
void exportar_alunos_para_txt(const char* filename);

#endif