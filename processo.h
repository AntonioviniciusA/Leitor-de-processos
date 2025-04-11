#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_ASSUNTOS 10
#define MAX_LINE 256

typedef struct {
    long id;
    char numero[21];
    char data_ajuizamento[25];
    int id_classe[MAX_ASSUNTOS];
    int id_assunto[MAX_ASSUNTOS];
    int num_classes;
    int num_assuntos;
    int ano_eleicao;
} Processo;

void ordenarPorId(Processo *processos, int n);
void ordenarPorDataDecrescente(Processo *processos, int n);
int contarProcessosPorClasse(Processo *processos, int n, int id_classe);
int contarAssuntosUnicos(Processo *processos, int n);
void listarProcessosMultiAssunto(Processo *processos, int n); 
void calcularDiasTramitacao(Processo *processos, int n, const char *data_atual);

#endif
