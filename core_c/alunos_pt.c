#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 128
#define MAX_ROWS 500

typedef struct {
  int id;
  int set;
  char nome[MAX_DATA];
  char email[MAX_DATA];
  char matricula[32];
  char curso[64];
} Student;

typedef struct {
  Student rows[MAX_ROWS];
} Database;

typedef struct {
  FILE *file;
  Database *db;
} Connection;

void die(const char *message)
{
  if(errno) perror(message);
  else fprintf(stderr, "ERROR: %s\n", message);
  exit(1);
}

void Student_print(const Student *s)
{
  printf("id=%d nome=\"%s\" email=\"%s\" matricula=\"%s\" curso=\"%s\"\n",
         s->id, s->nome, s->email, s->matricula, s->curso);
}

void Database_load(Connection *conn)
{
  if(fread(conn->db, sizeof(Database), 1, conn->file) != 1)
    die("Failed to load database");
}

Connection *Database_open(const char *filename, char mode)
{
  Connection *conn = malloc(sizeof(Connection));
  if(!conn) die("Memory error");
  conn->db = malloc(sizeof(Database));
  if(!conn->db) die("Memory error");

  if(mode == 'c') {
    conn->file = fopen(filename, "w+b"); // binary create
  } else {
    conn->file = fopen(filename, "r+b");
    if(conn->file) Database_load(conn);
  }
  if(!conn->file) die("Failed to open file");
  return conn;
}

void Database_close(Connection *conn)
{
  if(!conn) return;
  if(conn->file) fclose(conn->file);
  if(conn->db) free(conn->db);
  free(conn);
}

void Database_write(Connection *conn)
{
  rewind(conn->file);
  if(fwrite(conn->db, sizeof(Database), 1, conn->file) != 1) die("Failed to write database");
  if(fflush(conn->file) == -1) die("Cannot flush database");
}

void Database_create(Connection *conn)
{
  for(int i = 0; i < MAX_ROWS; i++){
    Student s = {.id = i, .set = 0};
    conn->db->rows[i] = s;
  }
}

static void safe_copy(char *dst, const char *src, size_t n)
{
  if(!src) { dst[0] = '\0'; return; }
  strncpy(dst, src, n-1);
  dst[n-1] = '\0';
}

void Database_set(Connection *conn, int id, const char *nome, const char *email, const char *matricula, const char *curso)
{
  if(id < 0 || id >= MAX_ROWS) die("Invalid id");
  Student *s = &conn->db->rows[id];
  if(s->set) die("Already set, delete it first");

  s->set = 1;
  s->id = id;
  safe_copy(s->nome, nome, sizeof(s->nome));
  safe_copy(s->email, email, sizeof(s->email));
  safe_copy(s->matricula, matricula, sizeof(s->matricula));
  safe_copy(s->curso, curso, sizeof(s->curso));
}

void Database_get(Connection *conn, int id)
{
  if(id < 0 || id >= MAX_ROWS) die("Invalid id");
  Student *s = &conn->db->rows[id];
  if(s->set) Student_print(s);
  else die("ID is not set");
}

void Database_delete(Connection *conn, int id)
{
  if(id < 0 || id >= MAX_ROWS) die("Invalid id");
  Student s = {.id = id, .set = 0};
  conn->db->rows[id] = s;
}

void Database_list(Connection *conn)
{
  for(int i = 0; i < MAX_ROWS; i++){
    Student *cur = &conn->db->rows[i];
    if(cur->set) Student_print(cur);
  }
}

int main(int argc, char *argv[])
{
  if(argc < 3) die("USAGE: alunos <dbfile> <action> [params]");
  char *filename = argv[1];
  char action = argv[2][0];
  Connection *conn = Database_open(filename, action);
  int id = 0;
  if(argc > 3) id = atoi(argv[3]);
  if(id >= MAX_ROWS) die("There is not that many records.");

  switch(action) {
    case 'c':
      Database_create(conn);
      Database_write(conn);
      break;
    case 'g':
      if(argc != 4) die("Need an id to get");
      Database_get(conn, id);
      break;
    case 's':
      if(argc != 8) die("Need id, nome, email, matricula, curso to set");
      Database_set(conn, id, argv[4], argv[5], argv[6], argv[7]);
      Database_write(conn);
      break;
    case 'd':
      if(argc != 4) die("Need an id to delete");
      Database_delete(conn, id);
      Database_write(conn);
      break;
    case 'l':
      Database_list(conn);
      break;
    default:
      die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");
  }

  Database_close(conn);
  return 0;
}