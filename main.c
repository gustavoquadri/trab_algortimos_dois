#include "joias.h"


//gcc -Wall -Wextra -g3 main.c joias.c -o main.exe
// /"


void mostrar_menu() {
    printf("\n=== MENU DO SISTEMA ===\n");
    printf("1. Processar arquivo CSV e criar arquivos de dados\n");
    printf("2. Mostrar joias\n");
    printf("3. Mostrar compras\n");
    printf("4. Buscar joia por ID\n");
    printf("5. Buscar compra por ID\n");
    printf("6. Criar indices\n");
    printf("7. Buscar joia usando indice\n");
    printf("8. Buscar compra usando indice\n");
    printf("9. Consultas especiais\n");
    printf("10. Inserir nova joia\n");
    printf("11. Remover joia\n");
    printf("12. Inserir nova compra\n");
    printf("13. Remover compra\n");
    printf("14. Mostrar indice de joias\n");
    printf("15. Mostrar indice de compras\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}


void processar_csv() {
    printf("\n=== PROCESSANDO ARQUIVO CSV ===\n");

    // processa joias
    printf("Processando joias...\n");
    int joias_inseridas = inserir_joias_do_csv("jewelry.csv", "joias.dat");
    if (joias_inseridas > 0) {
        printf("%d joias processadas com sucesso!\n", joias_inseridas);
    }

    // processa compras
    printf("\nProcessando compras...\n");
    int compras_inseridas = inserir_compras_do_csv("jewelry.csv", "compras.dat");
    if (compras_inseridas > 0) {
        printf("%d compras processadas com sucesso!\n", compras_inseridas);
    }

    printf("\n=== ARQUIVOS CRIADOS ===\n");
    printf("- joias.dat (arquivo de dados das joias)\n");
    printf("- compras.dat (arquivo de dados das compras)\n");
}


void criar_indices() {
    printf("\n=== CRIANDO INDICES ===\n");

    printf("Criando indice para joias...\n");
    int indice_joias = criar_indice_joias("joias.dat", "joias.idx");
    if (indice_joias > 0) {
        printf("Indice de joias criado com %d entradas!\n", indice_joias);
    }

    printf("Criando indice para compras...\n");
    int indice_compras = criar_indice_compras("compras.dat", "compras.idx");
    if (indice_compras > 0) {
        printf("Indice de compras criado com %d entradas!\n", indice_compras);
    }

    printf("\n=== INDICES CRIADOS ===\n");
    printf("- joias.idx (indice do arquivo de joias)\n");
    printf("- compras.idx (indice do arquivo de compras)\n");
}


int main() {
    int opcao;
    long long id;
    long long chave;
    long idx;
    printf("=== SISTEMA INICIANDO ===\n");

    do {
        mostrar_menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                processar_csv();
                break;

            case 2:
                mostrar_joias("joias.dat");
                break;

            case 3:
                mostrar_compras("compras.dat");
                break;

            case 4:
                printf("Digite o ID do Produto: ");
                scanf("%lld", &id);
                consultar_joia("joias.dat", id);
                break;

            case 5:
                printf("Digite o ID do Pedido: ");
                scanf("%lld", &id);
                consultar_compra("compras.dat", id);
                break;

            case 6:
                criar_indices();
                break;

            case 7:
                printf("Digite o numero do indice: ");
                scanf("%ld", &idx);
               chave = pesquisa_por_idx_arqInd("joias.idx", idx);
                printf("%lld", chave);
                break;

            case 8:
                printf("Digite o numero do indice: ");
                scanf("%ld", &idx);
                chave = pesquisa_por_idx_arqInd("compras.idx", idx);
                printf("%lld", chave);
                break;

            case 9:
                /*implementar*/
                break;

            case 10:
                inserir_joia_ordenada("joias.dat", "joias.idx");
                break;

            case 11:
                remove_joia("joias.dat", "joias.idx");

                break; 

            case 12:
                inserir_compra_ordenada("compras.dat", "compras.idx");
                break;

            case 13:
                remove_compra("compras.dat", "compras.idx");
                break;

            case 14:
                mostrar_indice_joias("joias.idx");
                break;

            case 15:
                mostrar_indice_compras("compras.idx");
                break;

            case 0:
                printf("Saindo do sistema...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }

        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
            getchar();
        }

    } while (opcao != 0);

    return 0;
}
