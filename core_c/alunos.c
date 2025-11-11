#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// declarações das funções/tipos do alunos.c (assuma mesmos nomes)
typedef struct {
  int id;
  int preenchido;
  char nome[128];
  char email[128];
  char matricula[32];
  char curso[64];
} Aluno;

typedef struct {
  Aluno linhas[500];
} BancoDados;

typedef struct {
  FILE *arquivo;
  BancoDados *bd;
} Conexao;

/* prototypes from alunos.c */
extern Conexao *abrir_banco(const char *nome_arquivo, char modo);
extern void fechar_banco(Conexao *con);

char *listar_alunos_json(const char *arquivo_db)
{
  Conexao *con = abrir_banco(arquivo_db, 'r');
  if(!con) return NULL;

  // buffer dinâmico
  size_t cap = 4096;
  size_t len = 0;
  char *buf = malloc(cap);
  if(!buf) { fechar_banco(con); return NULL; }
  buf[0] = '['; len = 1;

  for(int i = 0; i < 500; ++i) {
    Aluno *a = &con->bd->linhas[i];
    if(!a->preenchido) continue;
    // JSON-escape minimal (não cobre todos os casos; ok para dev)
    char item[1024];
    int n = snprintf(item, sizeof(item),
      "{\"id\":%d,\"nome\":\"%s\",\"email\":\"%s\",\"matricula\":\"%s\",\"curso\":\"%s\"},",
      a->id, a->nome, a->email, a->matricula, a->curso);
    if(n < 0) continue;
    // garantir espaço
    if(len + (size_t)n + 2 > cap) {
      cap = (cap + n + 4096) * 2;
      buf = realloc(buf, cap);
      if(!buf) { fechar_banco(con); return NULL; }
    }
    memcpy(buf + len, item, n);
    len += n;
  }

  // substituir vírgula final por ] ou fechar corretamente
  if(len > 1 && buf[len-1] == ',') {
    buf[len-1] = ']';
    len--;
    buf[len] = ']'; // overwritten to ensure
    buf[len+1] = '\0';
    // fix: we set ']' twice but then add null - normalize:
    buf[len] = ']';
    buf[len+1] = '\0';
  } else {
    if(len + 2 > cap) { buf = realloc(buf, len + 2); if(!buf){fechar_banco(con);return NULL;} }
    buf[len++] = ']';
    buf[len] = '\0';
  }

  fechar_banco(con);
  return buf; // caller deve free()
}

void free_str(char *p) { free(p); }