🔥 Free Fire — Sistema de Inventário em C

Este projeto implementa um sistema completo de mochila e gerenciamento de equipamentos para um jogo de sobrevivência, desenvolvido em linguagem C.

O sistema integra os três níveis do desafio (Novato, Aventureiro e Mestre) em um único código, aplicando estruturas de dados, algoritmos de ordenação e técnicas de busca para simular a evolução estratégica do jogador dentro do jogo.

🧠 Estruturas de Dados Utilizadas

Vetores (listas sequenciais)

Lista encadeada (estrutura dinâmica)

Structs (Item, No, Componente)

Ponteiros e alocação dinâmica

Algoritmos de ordenação

Busca sequencial e binária

Contadores de comparação

Medição de tempo com clock()

---

🎮 Funcionalidades por Nível
✅ Nível Novato – Inventário Básico

Cadastro de até 10 itens na mochila

Remoção de itens pelo nome

Listagem completa do inventário

Busca sequencial por nome

Uso de struct e vetor

---

✅ Nível Aventureiro – Estrutura Dupla

O inventário passa a funcionar com:

Vetor (lista sequencial)

Lista encadeada (estrutura dinâmica)

Funções implementadas nas duas versões:

Inserção de itens

Remoção

Listagem

Busca sequencial

Ordenação do vetor por nome

Busca binária (após ordenação)

Contador de comparações

---

✅ Nível Mestre – Sistema Avançado de Priorização

Nesta fase o jogador organiza os componentes da torre de fuga.

Funcionalidades:

Cadastro de até 20 componentes

Ordenação por:

Nome (Bubble Sort)

Tipo (Insertion Sort)

Prioridade (Selection Sort)

Busca binária otimizada

Medição de tempo de execução

Contagem real de comparações

Visualização estratégica da montagem

---

🎨 Sistema de Cores ANSI

O sistema utiliza cores no terminal para facilitar a leitura do jogo:

| Cor | Significado |
|-----|-------------|
| 🟢 Verde |	Sucesso e confirmações |
| 🔴 Vermelho |	Erros |
| 🟡 Amarelo |	Avisos e informações |
| 🔵 Azul | Menus e títulos |

---

👨‍💻 Autor: Rodrigo Gomes

---

Disciplina: Estruturas de Dados

---

Professor: Daisy Albuquerque

---

Faculdade: Estácio de Sá

---

⚙️ Compilação e Execução
Compilar:

gcc Free-Fire.c -o FreeFire

Executar:
./FreeFire
