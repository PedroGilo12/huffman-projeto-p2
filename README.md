<p align="center">
  <img src="imagens/logo-ic.png" alt="Descrição da imagem">
</p>


# Compactador de arquivos GZIP

Este projeto tem como objetivo implementar um compactador de arquivos utilizando o algoritmo de Huffman. O algoritmo de Huffman é um método de compressão sem perdas que utiliza a frequência de ocorrência dos caracteres em um arquivo para criar códigos de tamanho variável. Isso permite que caracteres mais frequentes sejam representados por códigos mais curtos, resultando em uma redução no tamanho do arquivo.

---
## Índice

- [Visão Geral](#visão-geral)
- [Requisitos](#requisitos)
- [Instalação](#instalação)
- [Uso](#uso)
- [Contribuição](#contribuição)
---
## Visão Geral

O algoritmo de Huffman é um método de compressão de dados que visa reduzir o tamanho de um arquivo ou mensagem, tornando-o mais eficiente em termos de armazenamento ou transmissão. A ideia básica por trás do algoritmo de Huffman é atribuir códigos binários mais curtos às palavras, letras ou símbolos mais frequentes em um conjunto de dados, e códigos binários mais longos às palavras menos frequentes. Isso aproveita a redundância nos dados, economizando espaço para representar informações comuns e usando mais bits para informações menos comuns.
## Requisitos

- GCC
- MakeFile
- [CUnit](#instalação-rápida-cunit) (Somente para os testes).
- Também é recomendado estudos dos métodos de compressão, descompressão e o conhecimento para a melhor entendimento da árvore de Huffman.
## Instalação

### Instalação rápida CUnit
1. No Windows, abra o terminal do MSYS2 e digite o seguinte comando:
    ```bash
    $ pacman -S mingw-w64-x86_64-cunit
    ```
### Compilação e execução
#### GZIP Huffman:

```bash
$ make
$ gzip_v1
```
#### CUnit testes:
```bash
$ cd CUnit-tests
$ make
$ tests-huffman
```
#### Contagem e plotagem:
```bash
$ cd contagem-e-plotagem
$ make
$ plot
```
## Uso GZIP Huffman
Ao iniciar o uso da aplicação, um menu vai exibir 4 opções, sendo elas:

1 - Compactar arquivo: Ao selecionar, o programa vai requisitar o caminho do arquivo e, com base no formato escolhido ele vai gerar o arquivo compactado.

2 - Descompactar arquivo: Ao selecionar, o programa vai requisitar a extensão final do arquivo para realizar o processo de descompactação.

3 - Alternar extensão (x).gip ( ).huff: Comando utilizado para escolher qual a extensão de arquivo você deseja utilizar.

9 - Sair: Comando para encerrar o programa.
## Contribuição

* Caio Oliveira França dos Anjos [cofa@ic.ufal.br](cofa@ic.ufal.br)
* Pedro Henrique Balbino Rocha [phbr@ic.ufal.br](phbr@ic.ufal.br)
* Pedro Henrique Vieira Giló [phvg@ic.ufal.br](phvg@ic.ufal.br)
* Raniel Ferreira Athayde [rfa@ic.ufal.br](rfa@ic.ufal.br)

Para qualquer contribuição, correção ou sugestão basta entrar em contato pelo GitHub ou pelo e-mail dos responsáveis pelo projeto.

---

Sociedade™. 