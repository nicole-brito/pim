## Visão rápida do repositório

Projeto simples de gestão escolar com três camadas principais:
- frontend/: SPA/HTML estático servido pelo Flask (JS em `frontend/js`, CSS em `frontend/css`). Ex.: `frontend/index.html`, `frontend/dashboard.html`.
- backend_py/: API em Python/Flask que serve o front e expõe rotas em `/api` — ponto central: `backend_py/app.py`, `backend_py/routes.py`.
- core_c/: lógica/núcleo em C compilada como biblioteca compartilhada (`pim.so`) e consumida por `backend_py/data_handler.py` via `ctypes`.

## O que um agente de codificação precisa saber (prioritário)

1. Como rodar e testar localmente

   - A API é iniciada executando `backend_py/app.py`. Ela espera encontrar a pasta `frontend/` em ../frontend (caminho relativo a `backend_py`).

   ```bash
   # a partir da raíz do repo
   python3 backend_py/app.py
   # o serviço escuta por padrão em http://0.0.0.0:5000
   ```

   - A camada C deve ser compilada para produzir `core_c/pim.so` antes de usar funcionalidades que dependem de `ctypes`.

   ```bash
   # dentro de core_c/
   make
   # ou: gcc -shared -fPIC -o pim.so *.c
   ```

2. Fluxo de dados e integração Python <-> C

   - `backend_py/data_handler.py` tenta carregar `../core_c/pim.so`. Se a biblioteca não existir, o código cai em fallback que lê arquivos de texto em `data/` (ex.: `data/alunos.txt`, `data/notas.txt`).
   - Principais funções C expostas esperadas: `autenticar`, `ler_alunos`, `exportar_alunos_para_txt`. Veja `core_c/main.c` e `structs.h` para definição.
   - Ao trabalhar em autenticação ou leitura de alunos, atualize ambos os lados (C e Python). `data_handler.autenticar_user()` retorna `(eh_prof, sucesso)` e contém prints de debug úteis.

3. Convenções e pontos específicos do projeto

   - Arquivos de dados ficam em `data/` e usam `|` como separador. `data_handler.ler_dados_txt()` ignora linhas que começam com `#`.
   - Rotas registradas em `backend_py/routes.py` usam blueprint `api` no prefixo `/api`. Exemplos:
     - Login: POST /api/login  (JSON {usuario, senha})
     - Busca:  GET /api/busca?nome=fulano
     - Notas:  GET /api/notas?aluno_id=1
     - Relatório: POST /api/relatorio  (apenas professores)

   - Controle de sessão: `session['usuario']` e `session['eh_prof']` são as chaves usadas. Algumas checagens usam lógica simplificada (ex.: hardcode `aluno_id != 1` em `routes.py` — revise antes de alterar regras de autorização).

4. Dependências importantes (instalar antes de rodar)

   - Python: flask, flask_cors, pandas

   ```bash
   pip install flask flask_cors pandas
   ```

5. Padrões de código e decisões observadas

   - Serviço web leve: não há uso de frameworks grandes além do Flask. Frontend é estático e servido pelo backend.
   - Integração com C feita por `ctypes` e com fallback para arquivos texto — os agentes devem sempre considerar o caminho relativo (`../core_c/pim.so`) e mensagens de fallback em `data_handler.py`.
   - Relação entre arquivos:
     - `backend_py/app.py` configura app + serve arquivos estáticos
     - `backend_py/routes.py` contém endpoints e usa `data_handler`, `busca_ordenacao` e `relatorios`
     - `backend_py/busca_ordenacao.py` usa `pandas` para ordenação e `data_handler.ler_alunos_from_c()` para busca de alunos

## Exemplos rápidos para agentes

- Para adicionar uma rota que retorna estatísticas de notas, atualize `routes.py` e chame `busca_ordenacao.ordenar_notas()` como exemplo de uso de pandas.
- Ao modificar autenticação C/Python: compile `core_c` (`make`) e rode `python3 backend_py/app.py`; verifique mensagens de `print` em `data_handler.py` para confirmar carregamento da lib.

## Coisas a evitar / observar com cautela

- Não assuma que `pim.so` existe — sempre verifique `data_handler.py` comportamento de fallback.
- Não exponha a chave `app.secret_key` em commits públicos. Atualmente está hardcoded em `backend_py/app.py`.
- Há checagens de autorização simplificadas; revise antes de escalar para produção.

## Onde olhar primeiro ao começar a editar

- `backend_py/routes.py` — ponto de integração para novas APIs.
- `backend_py/data_handler.py` — como o sistema lê/obtém dados (text/C).
- `core_c/` — se mexer na autenticação/estrutura de dados nativa.
- `data/` — exemplos reais de conteúdo e formato (seeder simples).

Se quiser, eu atualizo este arquivo com mais exemplos de chamadas HTTP ou um mini `requirements.txt`/script de build. Tem algo que ficou vago ou quer exemplos de resposta JSON para as rotas? 
