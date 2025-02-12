# Shell Simples

Este é um interpretador de comandos (shell) simples escrito em C.

## Pré-requisitos

- Sistema operacional POSIX (Linux, macOS ou similar)
- Compilador C com suporte ao padrão C99 (por exemplo, GCC)
- Make (GNU Make)

## Como Compilar e Executar

1. Faça o clone do repositório:

   ```bash
   git clone https://github.com/IlMeloIl/shell-c
   cd shell-c

2. Compile o projeto usando o Makafile

   ```bash
   make

3. Execute o shell

   ```bash
   ./shell

## Estrutura do Projeto e Makefile

### Estrutura do Projeto
- **Diretórios e Arquivos:**  
  - `include/`: cabeçalhos.
  - `src/`: código fonte.
  - `Makefile`: script de compilação.

## Dependências do Sistema

O programa utiliza funções que fazem parte da API POSIX:
- **wordexp:** Para expansão de tokens e reconhecimento de quoting.
- **termios, unistd, sys/wait, etc.:** Para manipulação do terminal e criação de processos.

Portanto, para que o programa compile e funcione corretamente, o usuário deve ter:
- Um sistema operacional Unix-like (Linux, macOS, etc.).
- Um compilador C como o GCC (ou Clang) instalado.
- As ferramentas de desenvolvimento (Make, bibliotecas padrão POSIX).

Em sistemas Linux isso pode ser garantido instalando o pacote `build-essential` (Debian/Ubuntu) ou equivalente.
