from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas
from data_handler import ler_dados_txt

def gerar_relatorio_notas(aluno_id: int, filename: str):
    notas = ler_dados_txt('notas.txt')
    notas_filtradas = [n for n in notas if n['aluno_id'] == aluno_id]
    c = canvas.Canvas(filename, pagesize=letter)
    c.drawString(100, 750, f"Relatório de Notas - Aluno ID: {aluno_id}")
    y = 700
    for nota in notas_filtradas:
        c.drawString(100, y, f"Nota: {nota['nota']} em {nota['data']}")
        y -= 20
    c.save()
    print(f"Relatório salvo em {filename}")