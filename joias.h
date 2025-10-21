#ifndef JOIAS_H
#define JOIAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_CATEGORIA 50
#define TAM_GENERO 10
#define TAM_COR 20
#define TAM_MATERIAL 20
#define TAM_GEMSTONE 20
#define TAM_TIMESTAMP 30

// estrutura para joias
typedef struct{
    long long id_produto;
    long long id_categoria;
    char alias_categoria[TAM_CATEGORIA];
    long long id_marca;
    double preco;
    char genero[TAM_GENERO];
    char cor[TAM_COR];
    char material[TAM_MATERIAL];
    char pedra_preciosa[TAM_GEMSTONE];
    char quebra_linha;
} Joia;

// estrutura para compras
typedef struct{
    long long id_pedido;
    long long id_produto;
    int quantidade;
    long long id_usuario;
    char data_hora[TAM_TIMESTAMP];
    char quebra_linha;
} Compra;

// estrutura para indice parcial
typedef struct{
    long long chave;
    long posicao;
} IndiceParcial;

// funcoes para joias
int inserir_joias_do_csv(const char* arquivo_csv, const char* arquivo_dados);
int mostrar_joias(const char* arquivo_dados);
int buscar_joia_binaria(const char* arquivo_dados, long long id_produto, Joia* joia);
int consultar_joia(const char* arquivo_dados, long long id_produto);
int criar_indice_joias(const char* arquivo_dados, const char* arquivo_indice);

// funcoes para compras
int inserir_compras_do_csv(const char* arquivo_csv, const char* arquivo_dados);
int mostrar_compras(const char* arquivo_dados);
int buscar_compra_binaria(const char* arquivo_dados, long long id_pedido, Compra* compra);
int consultar_compra(const char* arquivo_dados, long long id_pedido);
int criar_indice_compras(const char* arquivo_dados, const char* arquivo_indice);

// funcoes utilitarias
void preencher_string(char* str, int tamanho);
void limpar_string(char* str, int tamanho);
int contar_registros(const char* arquivo_dados, size_t tamanho_registro);
void ordenar_joias_por_id_produto(Joia* joias, int quantidade);
void ordenar_compras_por_id_pedido(Compra* compras, int quantidade);
int comparar_joias(const void* a, const void* b);
int comparar_compras(const void* a, const void* b);
int comparar_indices(const void* a, const void* b);

// funcoes para visualizar indices
void mostrar_indice_joias(const char* arquivo_indice);
void mostrar_indice_compras(const char* arquivo_indice);

#endif
