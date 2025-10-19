#include "joias.h"

// funcoes utilitarias
void preencher_string(char* str, int tamanho) {
    int len = strlen(str);
    for (int i = len; i < tamanho - 1; i++) {
        str[i] = ' ';
    }
    str[tamanho - 1] = '\0';
}

void limpar_string(char* str, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (str[i] == '\n' || str[i] == '\r') {
            str[i] = '\0';
            break;
        }
    }
}

int contar_registros(const char* arquivo_dados, size_t tamanho_registro) {
    FILE* arquivo = fopen(arquivo_dados, "rb");
    if (!arquivo) return 0;
    
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fclose(arquivo);
    
    return (int)(tamanho / tamanho_registro);
}

// funcoes de ordenacao
int comparar_joias(const void* a, const void* b) {
    const Joia* joia_a = (const Joia*)a;
    const Joia* joia_b = (const Joia*)b;
    return (joia_a->id_produto > joia_b->id_produto) - (joia_a->id_produto < joia_b->id_produto);
}

int comparar_compras(const void* a, const void* b) {
    const Compra* compra_a = (const Compra*)a;
    const Compra* compra_b = (const Compra*)b;
    return (compra_a->id_pedido > compra_b->id_pedido) - (compra_a->id_pedido < compra_b->id_pedido);
}

void ordenar_joias_por_id_produto(Joia* joias, int quantidade) {
    qsort(joias, quantidade, sizeof(Joia), comparar_joias);
}

void ordenar_compras_por_id_pedido(Compra* compras, int quantidade) {
    qsort(compras, quantidade, sizeof(Compra), comparar_compras);
}

int comparar_indices(const void* a, const void* b) {
    const IndiceParcial* indice_a = (const IndiceParcial*)a;
    const IndiceParcial* indice_b = (const IndiceParcial*)b;
    return (indice_a->chave > indice_b->chave) - (indice_a->chave < indice_b->chave);
}

// funcoes para joias
int inserir_joias_do_csv(const char* arquivo_csv, const char* arquivo_dados) {
    FILE* csv = fopen(arquivo_csv, "r");
    if (!csv) {
        printf("Erro ao abrir arquivo CSV: %s\n", arquivo_csv);
        return -1;
    }
    
    FILE* dados = fopen(arquivo_dados, "wb");
    if (!dados) {
        printf("Erro ao criar arquivo de dados: %s\n", arquivo_dados);
        fclose(csv);
        return -1;
    }
    
    char linha[1000];
    Joia* joias = NULL;
    int capacidade = 1000;
    int quantidade = 0;
    
    joias = (Joia*)malloc(capacidade * sizeof(Joia));
    if (!joias) {
        printf("Erro de memoria\n");
        fclose(csv);
        fclose(dados);
        return -1;
    }
    
    // O CSV nao tem cabecalho, comeca diretamente com os dados
    // Voltar para o inicio do arquivo
    fseek(csv, 0, SEEK_SET);
    
    while (fgets(linha, sizeof(linha), csv)) {
        if (quantidade >= capacidade) {
            capacidade *= 2;
            joias = (Joia*)realloc(joias, capacidade * sizeof(Joia));
            if (!joias) {
                printf("Erro de memoria\n");
                fclose(csv);
                fclose(dados);
                return -1;
            }
        }
        
        // Parse da linha CSV - ordem correta:
        // Order datetime, Order ID, Product ID, Quantity, Category ID, Category alias, Brand ID, Price, User ID, Gender, Color, Material, Gemstone
        char* token = strtok(linha, ",");
        if (!token) continue;
        
        // Pular Order datetime (já lido acima)
        token = strtok(NULL, ","); // Order ID
        if (!token) continue;
        
        token = strtok(NULL, ","); // Product ID
        if (!token) continue;
        joias[quantidade].id_produto = atoll(token);
        
        token = strtok(NULL, ","); // Quantity
        if (!token) continue;
        
        token = strtok(NULL, ","); // Category ID
        if (!token) continue;
        joias[quantidade].id_categoria = atoll(token);
        
        token = strtok(NULL, ","); // category_alias
        if (!token) continue;
        strncpy(joias[quantidade].alias_categoria, token, TAM_CATEGORIA - 1);
        joias[quantidade].alias_categoria[TAM_CATEGORIA - 1] = '\0';
        preencher_string(joias[quantidade].alias_categoria, TAM_CATEGORIA);
        
        token = strtok(NULL, ","); // brand_id
        if (!token) continue;
        joias[quantidade].id_marca = atoll(token);
        
        token = strtok(NULL, ","); // price
        if (!token) continue;
        joias[quantidade].preco = atof(token);
        
        token = strtok(NULL, ","); // user_id
        token = strtok(NULL, ","); // gender
        if (!token) continue;
        strncpy(joias[quantidade].genero, token, TAM_GENERO - 1);
        joias[quantidade].genero[TAM_GENERO - 1] = '\0';
        preencher_string(joias[quantidade].genero, TAM_GENERO);
        
        token = strtok(NULL, ","); // color
        if (!token) continue;
        strncpy(joias[quantidade].cor, token, TAM_COR - 1);
        joias[quantidade].cor[TAM_COR - 1] = '\0';
        preencher_string(joias[quantidade].cor, TAM_COR);
        
        token = strtok(NULL, ","); // material
        if (!token) continue;
        strncpy(joias[quantidade].material, token, TAM_MATERIAL - 1);
        joias[quantidade].material[TAM_MATERIAL - 1] = '\0';
        preencher_string(joias[quantidade].material, TAM_MATERIAL);
        
        token = strtok(NULL, ","); // gemstone
        if (token) {
            // Remover quebra de linha se existir
            char* newline = strchr(token, '\n');
            if (newline) *newline = '\0';
            strncpy(joias[quantidade].pedra_preciosa, token, TAM_GEMSTONE - 1);
            joias[quantidade].pedra_preciosa[TAM_GEMSTONE - 1] = '\0';
        } else {
            limpar_string(joias[quantidade].pedra_preciosa, TAM_GEMSTONE);
        }
        preencher_string(joias[quantidade].pedra_preciosa, TAM_GEMSTONE);
        
        joias[quantidade].quebra_linha = '\n';
        quantidade++;
    }
    
    // Ordenar por id_produto
    ordenar_joias_por_id_produto(joias, quantidade);
    
    // Escrever no arquivo binario
    for (int i = 0; i < quantidade; i++) {
        fwrite(&joias[i], sizeof(Joia), 1, dados);
    }
    
    printf("Inseridas %d joias no arquivo %s\n", quantidade, arquivo_dados);
    
    free(joias);
    fclose(csv);
    fclose(dados);
    return quantidade;
}

int mostrar_joias(const char* arquivo_dados) {
    FILE* arquivo = fopen(arquivo_dados, "rb");
    if (!arquivo) {
        printf("Erro ao abrir arquivo: %s\n", arquivo_dados);
        return -1;
    }
    
    Joia joia;
    int contador = 0;
    
    printf("\n=== JOIAS ===\n");
    printf("%-15s %-15s %-20s %-10s %-10s %-10s %-15s %-15s %-15s\n",
           "ID Produto", "ID Categoria", "Categoria", "ID Marca", "Preco", "Genero", 
           "Cor", "Material", "Pedra Preciosa");
    printf("--------------------------------------------------------------------------------------------------------\n");
    
    while (fread(&joia, sizeof(Joia), 1, arquivo) == 1) {
        printf("%-15lld %-15lld %-20s %-10lld %-10.2f %-10s %-15s %-15s %-15s\n",
                joia.id_produto, joia.id_categoria, joia.alias_categoria, joia.id_marca,
                joia.preco, joia.genero, joia.cor, joia.material, joia.pedra_preciosa);
        contador++;
        if (contador >= 20) { // Limitar exibicao
            printf("... (mostrando apenas os primeiros 20 registros)\n");
            break;
        }
    }
    
    fclose(arquivo);
    return contador;
}

int buscar_joia_binaria(const char* arquivo_dados, long long id_produto, Joia* joia) {
    FILE* arquivo = fopen(arquivo_dados, "rb");
    if (!arquivo) {
        printf("Erro ao abrir arquivo: %s\n", arquivo_dados);
        return -1;
    }
    
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    int total_registros = (int)(tamanho / sizeof(Joia));
    
    int esquerda = 0;
    int direita = total_registros - 1;
    
    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        fseek(arquivo, meio * sizeof(Joia), SEEK_SET);
        
        if (fread(joia, sizeof(Joia), 1, arquivo) != 1) {
            fclose(arquivo);
            return -1;
        }
        
        if (joia->id_produto == id_produto) {
            fclose(arquivo);
            return meio;
        } else if (joia->id_produto < id_produto) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    
    fclose(arquivo);
    return -1; // Nao encontrado
}

int consultar_joia(const char* arquivo_dados, long long id_produto) {
    Joia joia;
    int posicao = buscar_joia_binaria(arquivo_dados, id_produto, &joia);
    
    if (posicao >= 0) {
        printf("\n=== JOIA ENCONTRADA ===\n");
        printf("ID Produto: %lld\n", joia.id_produto);
        printf("ID Categoria: %lld\n", joia.id_categoria);
        printf("Categoria: %s\n", joia.alias_categoria);
        printf("ID Marca: %lld\n", joia.id_marca);
        printf("Preco: $%.2f\n", joia.preco);
        printf("Genero: %s\n", joia.genero);
        printf("Cor: %s\n", joia.cor);
        printf("Material: %s\n", joia.material);
        printf("Pedra Preciosa: %s\n", joia.pedra_preciosa);
        printf("Posicao no arquivo: %d\n", posicao);
        return 1;
    } else {
        printf("Joia com ID Produto %lld nao encontrada.\n", id_produto);
        return 0;
    }
}

int criar_indice_joias(const char* arquivo_dados, const char* arquivo_indice) {
    FILE* dados = fopen(arquivo_dados, "rb");
    if (!dados) {
        printf("Erro ao abrir arquivo de dados: %s\n", arquivo_dados);
        return -1;
    }
    
    FILE* indice = fopen(arquivo_indice, "wb");
    if (!indice) {
        printf("Erro ao criar arquivo de indice: %s\n", arquivo_indice);
        fclose(dados);
        return -1;
    }
    
    Joia joia;
    IndiceParcial entrada_indice;
    int posicao = 0;
    
    while (fread(&joia, sizeof(Joia), 1, dados) == 1) {
        entrada_indice.chave = joia.id_produto;
        entrada_indice.posicao = posicao;
        fwrite(&entrada_indice, sizeof(IndiceParcial), 1, indice);
        posicao++;
    }
    
    fclose(dados);
    fclose(indice);
    
    // Ordenar o indice por chave para busca binaria
    FILE* indice_read = fopen(arquivo_indice, "rb");
    if (!indice_read) {
        printf("Erro ao abrir arquivo de indice para ordenacao: %s\n", arquivo_indice);
        return -1;
    }
    
    IndiceParcial* indices = (IndiceParcial*)malloc(posicao * sizeof(IndiceParcial));
    if (!indices) {
        printf("Erro de memoria\n");
        fclose(indice_read);
        return -1;
    }
    
    fread(indices, sizeof(IndiceParcial), posicao, indice_read);
    fclose(indice_read);
    
    // Ordenar por chave
    qsort(indices, posicao, sizeof(IndiceParcial), comparar_indices);
    
    // Reescrever o indice ordenado
    FILE* indice_write = fopen(arquivo_indice, "wb");
    if (!indice_write) {
        printf("Erro ao reescrever arquivo de indice ordenado: %s\n", arquivo_indice);
        free(indices);
        return -1;
    }
    
    fwrite(indices, sizeof(IndiceParcial), posicao, indice_write);
    fclose(indice_write);
    free(indices);
    
    printf("Indice criado e ordenado com %d entradas para %s\n", posicao, arquivo_dados);
    return posicao;
}

// funcoes para compras
int inserir_compras_do_csv(const char* arquivo_csv, const char* arquivo_dados) {
    FILE* csv = fopen(arquivo_csv, "r");
    if (!csv) {
        printf("Erro ao abrir arquivo CSV: %s\n", arquivo_csv);
        return -1;
    }
    
    FILE* dados = fopen(arquivo_dados, "wb");
    if (!dados) {
        printf("Erro ao criar arquivo de dados: %s\n", arquivo_dados);
        fclose(csv);
        return -1;
    }
    
    char linha[1000];
    Compra* compras = NULL;
    int capacidade = 1000;
    int quantidade = 0;
    
    compras = (Compra*)malloc(capacidade * sizeof(Compra));
    if (!compras) {
        printf("Erro de memoria\n");
        fclose(csv);
        fclose(dados);
        return -1;
    }
    
    // Voltar para o inicio do arquivo
    fseek(csv, 0, SEEK_SET);
    
    while (fgets(linha, sizeof(linha), csv)) {
        if (quantidade >= capacidade) {
            capacidade *= 2;
            compras = (Compra*)realloc(compras, capacidade * sizeof(Compra));
            if (!compras) {
                printf("Erro de memoria\n");
                fclose(csv);
                fclose(dados);
                return -1;
            }
        }
        
        // Parse da linha CSV - ordem correta:
        // Order datetime, Order ID, Product ID, Quantity, Category ID, Category alias, Brand ID, Price, User ID, Gender, Color, Material, Gemstone
        char* token = strtok(linha, ",");
        if (!token) continue;
        
        // Order datetime (já lido acima)
        strncpy(compras[quantidade].data_hora, token, TAM_TIMESTAMP - 1);
        compras[quantidade].data_hora[TAM_TIMESTAMP - 1] = '\0';
        preencher_string(compras[quantidade].data_hora, TAM_TIMESTAMP);
        
        token = strtok(NULL, ","); // Order ID
        if (!token) continue;
        compras[quantidade].id_pedido = atoll(token);
        
        token = strtok(NULL, ","); // Product ID
        if (!token) continue;
        compras[quantidade].id_produto = atoll(token);
        
        token = strtok(NULL, ","); // Quantity
        if (!token) continue;
        compras[quantidade].quantidade = atoi(token);
        
        // Pular category_id, category_alias, brand_id, price
        for (int i = 0; i < 4; i++) {
            token = strtok(NULL, ",");
            if (!token) break;
        }
        
        token = strtok(NULL, ","); // User ID
        if (!token) continue;
        compras[quantidade].id_usuario = atoll(token);
        
        compras[quantidade].quebra_linha = '\n';
        quantidade++;
    }
    
    // Ordenar por id_pedido
    ordenar_compras_por_id_pedido(compras, quantidade);
    
    // Escrever no arquivo binario
    for (int i = 0; i < quantidade; i++) {
        fwrite(&compras[i], sizeof(Compra), 1, dados);
    }
    
    printf("Inseridas %d compras no arquivo %s\n", quantidade, arquivo_dados);
    
    free(compras);
    fclose(csv);
    fclose(dados);
    return quantidade;
}

int mostrar_compras(const char* arquivo_dados) {
    FILE* arquivo = fopen(arquivo_dados, "rb");
    if (!arquivo) {
        printf("Erro ao abrir arquivo: %s\n", arquivo_dados);
        return -1;
    }
    
    Compra compra;
    int contador = 0;
    
    printf("\n=== COMPRAS ===\n");
        printf("%-15s %-15s %-8s %-15s %-30s\n",
           "ID Pedido", "ID Produto", "Qtd", "ID Usuario", "Data/Hora");
    printf("--------------------------------------------------------------------\n");
    
    while (fread(&compra, sizeof(Compra), 1, arquivo) == 1) {
        printf("%-15lld %-15lld %-8d %-15lld %-30s\n",
                compra.id_pedido, compra.id_produto, compra.quantidade,
                compra.id_usuario, compra.data_hora);
        contador++;
        if (contador >= 20) {
            printf("... (mostrando apenas os primeiros 20 registros)\n");
            break;
        }
    }
    
    fclose(arquivo);
    return contador;
}

int buscar_compra_binaria(const char* arquivo_dados, long long id_pedido, Compra* compra) {
    FILE* arquivo = fopen(arquivo_dados, "rb");
    if (!arquivo) {
        printf("Erro ao abrir arquivo: %s\n", arquivo_dados);
        return -1;
    }
    
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    int total_registros = (int)(tamanho / sizeof(Compra));
    
    int esquerda = 0;
    int direita = total_registros - 1;
    
    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;
        fseek(arquivo, meio * sizeof(Compra), SEEK_SET);
        
        if (fread(compra, sizeof(Compra), 1, arquivo) != 1) {
            fclose(arquivo);
            return -1;
        }
        
        if (compra->id_pedido == id_pedido) {
            fclose(arquivo);
            return meio;
        } else if (compra->id_pedido < id_pedido) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    
    fclose(arquivo);
    return -1; // Nao encontrado
}

int consultar_compra(const char* arquivo_dados, long long id_pedido) {
    Compra compra;
    int posicao = buscar_compra_binaria(arquivo_dados, id_pedido, &compra);
    
    if (posicao >= 0) {
        printf("\n=== COMPRA ENCONTRADA ===\n");
        printf("ID Pedido: %lld\n", compra.id_pedido);
        printf("ID Produto: %lld\n", compra.id_produto);
        printf("Quantidade: %d\n", compra.quantidade);
        printf("ID Usuario: %lld\n", compra.id_usuario);
        printf("Data/Hora: %s\n", compra.data_hora);
        printf("Posicao no arquivo: %d\n", posicao);
        return 1;
    } else {
        printf("Compra com ID Pedido %lld nao encontrada.\n", id_pedido);
        return 0;
    }
}

int criar_indice_compras(const char* arquivo_dados, const char* arquivo_indice) {
    FILE* dados = fopen(arquivo_dados, "rb");
    if (!dados) {
        printf("Erro ao abrir arquivo de dados: %s\n", arquivo_dados);
        return -1;
    }
    
    FILE* indice = fopen(arquivo_indice, "wb");
    if (!indice) {
        printf("Erro ao criar arquivo de indice: %s\n", arquivo_indice);
        fclose(dados);
        return -1;
    }
    
    Compra compra;
    IndiceParcial entrada_indice;
    int posicao = 0;
    
    while (fread(&compra, sizeof(Compra), 1, dados) == 1) {
        entrada_indice.chave = compra.id_pedido;
        entrada_indice.posicao = posicao;
        fwrite(&entrada_indice, sizeof(IndiceParcial), 1, indice);
        posicao++;
    }
    
    fclose(dados);
    fclose(indice);
    
    // Ordenar o indice por chave para busca binaria
    FILE* indice_read = fopen(arquivo_indice, "rb");
    if (!indice_read) {
        printf("Erro ao abrir arquivo de indice para ordenacao: %s\n", arquivo_indice);
        return -1;
    }
    
    IndiceParcial* indices = (IndiceParcial*)malloc(posicao * sizeof(IndiceParcial));
    if (!indices) {
        printf("Erro de memoria\n");
        fclose(indice_read);
        return -1;
    }
    
    fread(indices, sizeof(IndiceParcial), posicao, indice_read);
    fclose(indice_read);
    
    // Ordenar por chave
    qsort(indices, posicao, sizeof(IndiceParcial), comparar_indices);
    
    // Reescrever o indice ordenado
    FILE* indice_write = fopen(arquivo_indice, "wb");
    if (!indice_write) {
        printf("Erro ao reescrever arquivo de indice ordenado: %s\n", arquivo_indice);
        free(indices);
        return -1;
    }
    
    fwrite(indices, sizeof(IndiceParcial), posicao, indice_write);
    fclose(indice_write);
    free(indices);
    
    printf("Indice criado e ordenado com %d entradas para %s\n", posicao, arquivo_dados);
    return posicao;
}

// funcoes para visualizar indices
void mostrar_indice_joias(const char* arquivo_indice) {
    FILE* indice = fopen(arquivo_indice, "rb");
    if (!indice) {
        printf("Erro ao abrir arquivo de indice: %s\n", arquivo_indice);
        return;
    }

    IndiceParcial entrada;
    int contador = 0;
    
    printf("\n=== INDICE DE JOIAS ===\n");
    printf("%-15s %-15s\n", "ID Produto", "Posicao");
    printf("------------------------------\n");
    
    while (fread(&entrada, sizeof(IndiceParcial), 1, indice) == 1) {
        printf("%-15lld %-15ld\n", entrada.chave, entrada.posicao);
        contador++;
        if (contador >= 50) { // Limitar exibicao
            printf("... (mostrando apenas os primeiros 50 registros)\n");
            break;
        }
    }
    
    fclose(indice);
    printf("\nTotal de entradas no indice: %d\n", contador);
}

void mostrar_indice_compras(const char* arquivo_indice) {
    FILE* indice = fopen(arquivo_indice, "rb");
    if (!indice) {
        printf("Erro ao abrir arquivo de indice: %s\n", arquivo_indice);
        return;
    }

    IndiceParcial entrada;
    int contador = 0;
    
    printf("\n=== INDICE DE COMPRAS ===\n");
    printf("%-15s %-15s\n", "ID Pedido", "Posicao");
    printf("------------------------------\n");
    
    while (fread(&entrada, sizeof(IndiceParcial), 1, indice) == 1) {
        printf("%-15lld %-15ld\n", entrada.chave, entrada.posicao);
        contador++;
        if (contador >= 50) { // Limitar exibicao
            printf("... (mostrando apenas os primeiros 50 registros)\n");
            break;
        }
    }
    
    fclose(indice);
    printf("\nTotal de entradas no indice: %d\n", contador);
}
