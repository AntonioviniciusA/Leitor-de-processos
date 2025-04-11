#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"
#include "processo.c"


static void parseMultivalorado(char *str, int *array, int *count) {
    *count = 0;
    if (!str || strlen(str) < 2) {
        printf("Formato inválido: %s\n", str ? str : "NULL");
        return;
    }

    char temp[100];
    if (str[0] == '"') {
        size_t len = strlen(str);
        strncpy(temp, str + 1, len - 2);
        temp[len - 2] = '\0';        
    } else {
        strcpy(temp, str);
    }

    if (temp[0] != '{') {
        printf("Formato inválido (sem '{'): %s\n", temp);
        return;
    }
    char *end = strchr(temp, '}');
    if (!end) {
        printf("Chave de fechamento '}' não encontrada: %s\n", temp);
        return;
    }
    *end = '\0';

    char *start = temp + 1;
    char *token = strtok(start, ",");
    while (token && *count < MAX_ASSUNTOS) {
        array[(*count)++] = atoi(token);
        token = strtok(NULL, ",");
    }
}

static char *extractField(char **line) {
    char *start = *line;
    char *end;
    int in_quotes = 0;

    if (*start == '"') {
        in_quotes = 1;
        start++;
        end = start;
        while (*end && (*end != '"' || *(end - 1) == '\\')) end++;
    } else {
        end = strchr(start, ',');
        if (!end) end = start + strlen(start);
    }

    int len = end - start;
    char *field = malloc(len + 1);
    strncpy(field, start, len);
    field[len] = '\0';

    *line = in_quotes ? end + 2 : end + 1;
    if (**line == '\0') *line = end;

    return field;
}

int main() {

    char cwd[1024];
    _getcwd(cwd, sizeof(cwd));
    printf("Executando em: %s\n", cwd);
    
    FILE *fp = fopen("processo_043_202409032338.csv", "r");
    if (!fp) {
        printf("Erro ao abrir arquivo processo_043_202409032338.csv!\n");
        return 1;
    }

    char line[MAX_LINE];
    if (!fgets(line, MAX_LINE, fp)) {
        printf("Erro ao ler cabeçalho!\n");
        fclose(fp);
        return 1;
    }

    Processo processos[1000];
    int n = 0;

    while (fgets(line, MAX_LINE, fp) && n < 1000) {
        line[strcspn(line, "\n")] = 0;
        char *ptr = line;

        char *id_str = extractField(&ptr);
        processos[n].id = atol(id_str);
        free(id_str);

        char *numero = extractField(&ptr);
        strncpy(processos[n].numero, numero, 20);
        processos[n].numero[20] = '\0';
        free(numero);

        char *data = extractField(&ptr);
        strncpy(processos[n].data_ajuizamento, data, 24);
        processos[n].data_ajuizamento[24] = '\0';
        free(data);

        char *classe = extractField(&ptr);
        parseMultivalorado(classe, processos[n].id_classe, &processos[n].num_classes);
        free(classe);

        char *assunto = extractField(&ptr);
        parseMultivalorado(assunto, processos[n].id_assunto, &processos[n].num_assuntos);
        free(assunto);

        char *ano = extractField(&ptr);
        processos[n].ano_eleicao = atoi(ano);
        free(ano);

        n++;
    }
    fclose(fp);

    ordenarPorId(processos, n);
    printf("Dados ordenados por ID salvos em 'ordenado_id.csv'\n");

    ordenarPorDataDecrescente(processos, n);
    printf("Dados ordenados por data salvos em 'ordenado_data.csv'\n");

    int id_classe = 12553;
    int count_classe = contarProcessosPorClasse(processos, n, id_classe);
    printf("Processos com id_classe %d: %d\n", id_classe, count_classe);

    int total_assuntos = contarAssuntosUnicos(processos, n);
    printf("Total de assuntos únicos: %d\n", total_assuntos);

    listarProcessosMultiAssunto(processos, n);

    const char *data_atual = "2025-04-04 00:00:00.000";
    calcularDiasTramitacao(processos, n, data_atual);

    return 0;
}
