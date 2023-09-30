/**
* @file input_process.c
*
* @brief Responsável por receber e tratar o arquivo de entrada.
*
* @authors Caio Oliveira França dos Anjos (cofa@ic.ufal.br)
*          Pedro Henrique Balbino Rocha (phbr@ic.ufal.br)
*          Pedro Henrique Vieira Giló (phvg@ic.ufal.br)
*          Raniel Ferreira Athayde (rfa@ic.ufal.br)
*/

#ifndef GZIP_V1_INTERFACE_H
#define GZIP_V1_INTERFACE_H

#include <common.h>
#include <time.h>
#include <input_process.h>
#include <output_process.h>
#include <stdio.h>

#define N_ATTEMPTS_TO_OPEN_FILE 9

void main_interface(void);

#endif  // GZIP_V1_INTERFACE_H
