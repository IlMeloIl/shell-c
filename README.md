# Shell Simples

Um interpretador de comandos (shell) simples escrito em C, demonstrando conceitos de manipulação de processos, redirecionamento e autocompletar utilizando uma Trie. Este projeto serve como referência para aprender sobre programação de sistemas e desenvolvimento de shells.

## Funcionalidades

- Execução de comandos internos e externos
- Autocomplete de comandos utilizando estrutura Trie
- Suporte a redirecionamento de saída e erro
- Configuração interativa do terminal (modo não canônico)

## Pré-requisitos

- **Sistema operacional:** POSIX (Linux, macOS ou similar)
- **Compilador:** C com suporte ao padrão C99 (por exemplo, GCC ou Clang)
- **Ferramenta de compilação:** GNU Make
- **Ferramentas de desenvolvimento:** Em sistemas Linux, por exemplo, instale o pacote `build-essential` (Debian/Ubuntu) ou equivalente

## Como Compilar e Executar

1. **Clone o repositório:**
   ```bash
   git clone https://github.com/IlMeloIl/shell-c.git
   cd shell-c
   ```

2. **Compile o projeto utilizando o Makefile:**
   ```bash
   make
   ```

3. **Execute o shell:**
   ```bash
   ./shell
   ```

## Estrutura do Projeto

- **include/**: Arquivos de cabeçalho (headers) contendo as definições de funções e estruturas
- **src/**: Código-fonte com as implementações das funcionalidades do shell
- **Makefile**: Script de compilação que gera o executável `shell`

## Dependências do Sistema

O programa utiliza funções da API POSIX, tais como:

- **wordexp:** Responsável pela expansão de tokens e reconhecimento de quoting
- **termios, unistd, sys/wait, etc.:** Utilizadas para manipulação do terminal e criação de processos

Portanto, para compilar e executar o shell corretamente, é necessário ter um sistema operacional Unix-like (Linux, macOS, etc.), um compilador C (como GCC ou Clang) e as ferramentas de desenvolvimento apropriadas.

## Exemplos de Uso

Após executar `./shell`, experimente os seguintes comandos:

- Digite `echo "Olá, mundo!"` e pressione Enter para visualizar a saída
- Utilize o autocomplete pressionando a tecla TAB após digitar parte de um comando
- Teste redirecionamentos com comandos como `ls > arquivo.txt` ou `grep "termo" arquivo 2> erros.txt`
