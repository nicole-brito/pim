# ...existing code...
import json, ctypes
from pathlib import Path
import ctypes
import csv
from typing import List, Dict

_so = None
def _load_lib():
    global _so
    if _so: return _so
    so_path = Path(__file__).resolve().parents[1] / "core_c" / "pim.so"
    try:
        _so = ctypes.CDLL(str(so_path))
        _so.listar_alunos_json.argtypes = [ctypes.c_char_p]
        _so.listar_alunos_json.restype = ctypes.c_char_p
        _so.free_str.argtypes = [ctypes.c_char_p]
        _so.free_str.restype = None
    except OSError:
        _so = None
    return _so

def ler_alunos_from_c(dbfile):
    so = _load_lib()
    if not so:
        # fallback existente que lê from data/*.txt
        return ler_alunos_from_txt()  # ajuste para função do projeto
    res = so.listar_alunos_json(str(dbfile).encode())
    if not res:
        return []
    try:
        s = ctypes.cast(res, ctypes.c_char_p).value.decode()
        return json.loads(s)
    finally:
        so.free_str(res)
# ...existing code...


_so = None
def _load_lib():
    global _so
    if _so: return _so
    so_path = Path(__file__).resolve().parents[1] / "core_c" / "pim.so"
    try:
        _so = ctypes.CDLL(str(so_path))
        _so.listar_alunos_json.argtypes = [ctypes.c_char_p]
        _so.listar_alunos_json.restype = ctypes.c_char_p
        _so.free_str.argtypes = [ctypes.c_char_p]
        _so.free_str.restype = None
    except OSError:
        _so = None
    return _so

def ler_alunos_from_c(dbfile):
    so = _load_lib()
    if not so:
        # fallback existente que lê from data/*.txt
        return ler_alunos_from_txt()  # ajuste para função do projeto
    res = so.listar_alunos_json(str(dbfile).encode())
    if not res:
        return []
    try:
        s = ctypes.cast(res, ctypes.c_char_p).value.decode()
        return json.loads(s)
    finally:
        so.free_str(res)
# ...existing code...


# Após try: lib = ctypes.CDLL...
try:
    lib = ctypes.CDLL('../core_c/pim.so')  # Ajuste caminho se projeto em ~/pim
    print("Lib C carregada com sucesso!")  # Debug: Adicione aqui
except OSError as e:
    print(f"Erro ao carregar libescola.so: {e}")  # Mostra erro específico
    lib = None

def autenticar_user(usuario: str, senha: str) -> tuple[int, bool]:
    if lib is None:
        print("Lib C não carregada – autenticação falha!")  # Debug
        return 0, False
    eh_prof = ctypes.c_int()
    sucesso = lib.autenticar(usuario.encode('utf-8'), senha.encode('utf-8'), ctypes.byref(eh_prof))
    print(f"Chamada C: Sucesso={bool(sucesso)}, EhProf={eh_prof.value}")  # Debug
    return eh_prof.value, bool(sucesso)

# Carrega biblioteca C (ajuste caminho se necessário)
try:
    lib = ctypes.CDLL('../core_c/pim.so')  # Linux/WSL; use .dll no Windows
except OSError:
    print("Erro ao carregar pim.so – compile C primeiro!")
    lib = None

# Tipos para ctypes (exemplo para Aluno)
class Aluno(ctypes.Structure):
    _fields_ = [
        ("nome", ctypes.c_char * 50),
        ("id", ctypes.c_int),
        ("matricula", ctypes.c_char * 20),
        ("senha", ctypes.c_char * 20)
    ]

def autenticar_user(usuario: str, senha: str) -> tuple[int, bool]:
    if lib is None:
        return 0, False
    eh_prof = ctypes.c_int()
    sucesso = lib.autenticar(usuario.encode('utf-8'), senha.encode('utf-8'), ctypes.byref(eh_prof))
    return eh_prof.value, bool(sucesso)

def ler_dados_txt(arquivo: str) -> List[Dict]:
    dados = []
    try:
        with open(f"../data/{arquivo}", 'r') as f:
            reader = csv.reader(f, delimiter='|')
            for row in reader:
                if len(row) >= 3 and row[0] != '#':  # Ignora comentários
                    if arquivo == 'alunos.txt':
                        dados.append({'id': int(row[0]), 'nome': row[1], 'matricula': row[2]})
                    elif arquivo == 'notas.txt':
                        dados.append({'id': int(row[0]), 'aluno_id': int(row[1]), 'nota': float(row[2]), 'data': row[3]})
                    # Expanda para outros arquivos
    except FileNotFoundError:
        pass
    return dados

def ler_alunos_from_c() -> List[Dict]:
    if lib is None:
        return ler_dados_txt('alunos.txt')
    count = ctypes.c_int()
    ptr = lib.ler_alunos(ctypes.byref(count))
    if count.value == 0:
        return []
    # Simples: Retorna via arquivo exportado (para simplicidade)
    lib.exportar_alunos_para_txt(b"../data/alunos_export.txt")
    return ler_dados_txt('alunos_export.txt')