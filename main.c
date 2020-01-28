#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include "frontend.h"
#include "ll.h"
#include "ast.h"
#include "backend.h"

FILE* fout = NULL;

int main (int argc, char **argv) {
  int opt;
  char *in_filename = NULL, *tree_filename = NULL, *out_filename = NULL;

  while ((opt = getopt(argc, argv, "hi:a:o:")) > 0) {
    switch (opt) {
			case 'h':
				printf("Comandos:\n-h\tAjuda\n-i\tArquivo de entrada\n-a\tArquivo de saída AST\n-d\tDebug mode\n");
				break ;
			case 'i':
				in_filename = optarg;
				break;
      case 'o':
        out_filename = optarg;
        break;
			// case 'a':
			// 	tree_filename = optarg;
			// 	break;
			// case 'd':
			// 	DEBUG = 1;
				break;
			default:
				fprintf(stderr, "Opcao invalida ou faltando argumento: `%c'\n", opt) ;
				return -1 ;
		}
  }

  if (!in_filename) {
    printf("Passe o arquivo de entrada: -i <arquivo>");
    exit(1);
  }

  if (!out_filename) {
    printf("Passe o arquivo de saída: -o <arquivo>");
    exit(1);
  }

  llnode list = frontend(in_filename);
  llnode p = list;

  FILE *out_file = fopen(out_filename, "w");
  if (!out_file){
    printf("ERRO ao abrir o arquivo %s\n", out_filename);
    exit(1);
  }

  gen(out_file, p);

  // if (tree_filename) {
  //   FILE *out_file = fopen(out_filename, "w");
  //   if (!out_file){
  //     printf("ERRO ao abrir o arquivo %s\n", out_filename);
  //     exit(1);
  // }
  // }

  // fout = stdout;

  // while(p != NULL){
  //   // printtree(p->tree, 0, fout);
  //   p = p->next;
  // }

  return 0;
}