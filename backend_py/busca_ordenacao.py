import pandas as pd
from typing import List, Dict
from data_handler import ler_dados_txt, ler_alunos_from_c

def buscar_aluno(nome: str) -> List[Dict]:
    alunos = ler_alunos_from_c()
    return [a for a in alunos if nome.lower() in a.get('nome', '').lower()]

def ordenar_notas(por: str = 'nota') -> List[Dict]:
    notas = ler_dados_txt('notas.txt')
    if not notas:
        return []
    df = pd.DataFrame(notas)
    return df.sort_values(by=por, ascending=False).to_dict('records')