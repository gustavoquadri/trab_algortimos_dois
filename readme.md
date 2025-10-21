## 📋 Descrição do Trabalho

Este trabalho implementa um sistema de gerenciamento de dados para uma loja online de joias, utilizando a organização **sequencial-indexada**. O sistema processa dados de compras de dezembro de 2018 a dezembro de 2021, contendo informações sobre produtos, categorias, preços, usuários e transações.

## 🎯 Objetivos

- Implementar arquivos de dados com registros de tamanho fixo
- Criar índices parciais para acesso eficiente aos dados
- Utilizar algoritmos de ordenação (Quicksort) e busca (Busca Binária)
- Gerenciar operações CRUD (Create, Read, Update, Delete) nos arquivos
- Demonstrar conceitos de organização de arquivos e estruturas de dados

## 📁 Estrutura do Projeto

```
trab_algoritmos_dois/
├── main.c              # Programa principal com menu interativo
├── joias.h             # Definições de estruturas e protótipos
├── joias.c             # Implementação das funções
├── jewelry.csv         # Dataset original (dados de entrada)
├── joias.dat           # Arquivo de dados das joias (gerado)
├── compras.dat         # Arquivo de dados das compras (gerado)
├── joias.idx           # Índice parcial das joias (gerado)
├── compras.idx         # Índice parcial das compras (gerado)
├── compila.bat         # Arquivo para rodar o sistema
└── README.md           # Este arquivo
```

## 🗂️ Arquivos de Dados

### joias.dat
**Estrutura:** Arquivo binário com registros de tamanho fixo (173 bytes)
**Organização:** Sequencial, ordenado por `id_produto`
**Campos:**
- `id_produto` (long long): Identificador único do produto
- `id_categoria` (long long): ID da categoria
- `alias_categoria` (char[50]): Nome da categoria
- `id_marca` (long long): ID da marca
- `preco` (double): Preço em USD
- `genero` (char[10]): Gênero do produto (f/m)
- `cor` (char[20]): Cor do produto
- `material` (char[20]): Material utilizado
- `pedra_preciosa` (char[20]): Tipo de pedra preciosa
- `quebra_linha` (char): Caractere de controle

### compras.dat
**Estrutura:** Arquivo binário com registros de tamanho fixo (58 bytes)
**Organização:** Sequencial, ordenado por `id_pedido`
**Campos:**
- `id_pedido` (long long): Identificador único do pedido
- `id_produto` (long long): ID do produto comprado
- `quantidade` (int): Quantidade de itens
- `id_usuario` (long long): ID do usuário
- `data_hora` (char[30]): Timestamp da compra
- `quebra_linha` (char): Caractere de controle

## 🔍 Arquivos de Índices

### joias.idx
**Estrutura:** Índice parcial ordenado por chave
**Campos:**
- `chave` (long long): ID do produto (chave de busca)
- `posicao` (long): Posição do registro no arquivo de dados

### compras.idx
**Estrutura:** Índice parcial ordenado por chave
**Campos:**
- `chave` (long long): ID do pedido (chave de busca)
- `posicao` (long): Posição do registro no arquivo de dados

## ⚙️ Algoritmos Utilizados

### 1. Quicksort
- **Função:** Ordenação dos dados antes de salvar nos arquivos
- **Complexidade:** O(n log n) no caso médio
- **Implementação:** `qsort()` da biblioteca padrão C
- **Uso:** Ordena joias por `id_produto` e compras por `id_pedido`

### 2. Busca Binária
- **Função:** Localização eficiente de registros
- **Complexidade:** O(log n)
- **Implementação:** Algoritmo iterativo com `fseek()`
- **Uso:** Busca em arquivos de dados e índices

### 3. Organização Sequencial-Indexada
- **Dados:** Armazenados sequencialmente em arquivos binários
- **Índices:** Criados para acesso direto aos registros
- **Vantagem:** Combina eficiência de acesso com economia de espaço

## 📋 Menu do Sistema

1. **Processar arquivo CSV e criar arquivos de dados**
2. **Mostrar joias** (limitado a 20 registros)
3. **Mostrar compras** (limitado a 20 registros)
4. **Buscar joia por ID** (busca binária direta)
5. **Buscar compra por ID** (busca binária direta)
6. **Criar índices** (gera arquivos .idx)
7. **Buscar joia usando índice** (busca binária no índice)
8. **Buscar compra usando índice** (busca binária no índice)
9. **Inserir nova joia** (com reconstrução do índice)
10. **Remover joia** (com reconstrução do índice)
11. **Inserir nova compra** (com reconstrução do índice)
12. **Remover compra** (com reconstrução do índice)
13. **Mostrar índice de joias**
14. **Mostrar índice de compras**

![alt text](<foto1.jpg>)

- **Como será gerenciada a inserção de um novo registro no arquivo de dados?**
Ao inserir uma nova jóia, o arquivo de dados (joias.dat) receberá essa nova linha no seu final, após isso, será ordenado. Logo, o arquivo de índices (joias.idx) que é organizado a partir do campo posição (que serve para calcular o offset) será totalmente apagado e refeito, agora, com a linha nova adicionada. Dessa forma, as inserções são bem custosas porque o arquivo é grande. A mesma funcionalidade serve para o arquivo de compras, porém, com o tamanho em bits das linhas diferentes.

- **Como será gerenciada a remoção de um registro no arquivo de dados?**
Ao remover, a linha não será apagada, mas terá seu valor substituído por valores ''placeholders", isso foi feito para não ser custoso ao remover (pois não será preciso reorganizar o arquivo, foto 1). Já no arquivo de índices, a linha é apagada, porém os outros índices não são atualizados para manter a conformidade do cálculo de offset a partir do campo posição (índice).


**Disciplina:** Algoritmos e Estruturas de Dados II  
