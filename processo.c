#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"


static int compararId(const void *a, const void *b) {
    return ((Processo*)a)->id - ((Processo*)b)->id;
}

static int compararData(const void *a, const void *b) {
    return strcmp(((Processo*)b)->data_ajuizamento, ((Processo*)a)->data_ajuizamento);
}

void ordenarPorId(Processo *processos, int n) {
    if (!processos || n <= 0) return;
    qsort(processos, n, sizeof(Processo), compararId);
    FILE *fp = fopen("ordenado_id.csv", "w");
    if (!fp) {
        printf("Erro ao criar ordenado_id.csv\n");
        
    }
    fprintf(fp, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%ld,%s,%s,{", processos[i].id, processos[i].numero, processos[i].data_ajuizamento);
        for (int j = 0; j < processos[i].num_classes; j++) {
            fprintf(fp, "%d%s", processos[i].id_classe[j], j < processos[i].num_classes - 1 ? "," : "");
        }
        fprintf(fp, "},{");
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            fprintf(fp, "%d%s", processos[i].id_assunto[j], j < processos[i].num_assuntos - 1 ? "," : "");
        }
        fprintf(fp, "},%d\n", processos[i].ano_eleicao);
    }
    fclose(fp);
}

void ordenarPorDataDecrescente(Processo *processos, int n) {
    if (!processos || n <= 0) return;
    qsort(processos, n, sizeof(Processo), compararData);
    FILE *fp = fopen("ordenado_data.csv", "w");
    if (!fp) {
        printf("Erro ao criar ordenado_data.csv\n");
        return;
    }
    fprintf(fp, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%ld,%s,%s,{", processos[i].id, processos[i].numero, processos[i].data_ajuizamento);
        for (int j = 0; j < processos[i].num_classes; j++) {
            fprintf(fp, "%d%s", processos[i].id_classe[j], j < processos[i].num_classes - 1 ? "," : "");
        }
        fprintf(fp, "},{");
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            fprintf(fp, "%d%s", processos[i].id_assunto[j], j < processos[i].num_assuntos - 1 ? "," : "");
        }
        fprintf(fp, "},%d\n", processos[i].ano_eleicao);
    }
    fclose(fp);
}

int contarProcessosPorClasse(Processo *processos, int n, int id_classe) {
    if (!processos || n <= 0) return 0;
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].num_classes; j++) {
            if (processos[i].id_classe[j] == id_classe) {
                count++;
                break;
            }
        }
    }
    return count;
}

int contarAssuntosUnicos(Processo *processos, int n) {
    if (!processos || n <= 0) return 0;
    int assuntos[10000] = {0};
    int total = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            if (!assuntos[processos[i].id_assunto[j]]) {
                assuntos[processos[i].id_assunto[j]] = 1;
                total++;
            }
        }
    }
    return total;
}

void listarProcessosMultiAssunto(Processo *processos, int n) {
    if (!processos || n <= 0) return;
    FILE *fp = fopen("multi_assuntos.csv", "w");
    if (!fp) {
        printf("Erro ao criar multi_assuntos.csv\n");
        return;
    }
    fprintf(fp, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (processos[i].num_assuntos > 1) {
            fprintf(fp, "%ld,%s,%s,{", processos[i].id, processos[i].numero, processos[i].data_ajuizamento);
            for (int j = 0; j < processos[i].num_classes; j++) {
                fprintf(fp, "%d%s", processos[i].id_classe[j], j < processos[i].num_classes - 1 ? "," : "");
            }
            fprintf(fp, "},{");
            for (int j = 0; j < processos[i].num_assuntos; j++) {
                fprintf(fp, "%d%s", processos[i].id_assunto[j], j < processos[i].num_assuntos - 1 ? "," : "");
            }
            fprintf(fp, "},%d\n", processos[i].ano_eleicao);
            count++;
        }
    }
    fclose(fp);
    if (count == 0) {
        printf("Nenhum processo com mais de um assunto encontrado. Arquivo 'multi_assuntos.csv' está vazio.\n");
    } else {
        printf("Processos com mais de um assunto: %d (salvos em 'multi_assuntos.csv')\n", count);
    }
}

static long calcularDiferencaDias(const char *data_inicio, const char *data_fim) {
    if (!data_inicio || !data_fim) return -1;
    struct tm inicio = {0}, fim = {0};
    if (sscanf(data_inicio, "%d-%d-%d %d:%d:%d", 
               &inicio.tm_year, &inicio.tm_mon, &inicio.tm_mday,
               &inicio.tm_hour, &inicio.tm_min, &inicio.tm_sec) != 6) {
        return -1;
    }
    if (sscanf(data_fim, "%d-%d-%d %d:%d:%d", 
               &fim.tm_year, &fim.tm_mon, &fim.tm_mday,
               &fim.tm_hour, &fim.tm_min, &fim.tm_sec) != 6) {
        return -1;
    }
    inicio.tm_year -= 1900;
    inicio.tm_mon -= 1;
    fim.tm_year -= 1900;
    fim.tm_mon -= 1;
    
    time_t t_inicio = mktime(&inicio);
    time_t t_fim = mktime(&fim);
    if (t_inicio == -1 || t_fim == -1) return -1;
    
    return (t_fim - t_inicio) / (24 * 3600);
}

void calcularDiasTramitacao(Processo *processos, int n, const char *data_atual) {
    if (!processos || n <= 0 || !data_atual) return;
    long total_dias = 0;
    int count = 0;
    long exemplo_dias = -1;
    long exemplo_id = -1;

    for (int i = 0; i < n; i++) {
        long dias = calcularDiferencaDias(processos[i].data_ajuizamento, data_atual);
        if (dias >= 0) {
            total_dias += dias;
            count++;
            if (exemplo_dias == -1) { // Pega o primeiro processo válido como exemplo
                exemplo_dias = dias;
                exemplo_id = processos[i].id;
            }
        }
    }
    if (count > 0) {
        printf("Média de dias em tramitação: %.2f\n", (double)total_dias / count);
        printf("Exemplo: Processo ID %ld tem %ld dias em tramitação\n", exemplo_id, exemplo_dias);
    } else {
        printf("Nenhum processo com dias calculáveis.\n");
    }
}
