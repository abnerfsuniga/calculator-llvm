#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include "frontend.h"
#include "ll.h"
#include "ast.h"
#include "backend.h"

int main (int argc, char **argv) {
  int opt;
  char *in_filename = NULL, *tree_filename = NULL, *out_filename = NULL;

  while ((opt = getopt(argc, argv, "hi:a:o:")) > 0) {
    switch (opt) {
			case 'h':
				printf("Comandos:\n-h\tAjuda\n-i\tArquivo de entrada\n-a\tArquivo de saída AST\n-o\tArquivo excutável\n");
				break ;
			case 'i':
				in_filename = optarg;
				break;
      case 'o':
        out_filename = optarg;
        break;
			case 'a':
				tree_filename = optarg;
				break;
			default:
				fprintf(stderr, "Opcao invalida ou faltando argumento: '%c'\n", opt) ;
				return -1 ;
		}
  }

  if (!in_filename) {
    printf("Passe o arquivo de entrada: -i <arquivo>\n");
    exit(1);
  }

  if (!out_filename) {
    printf("Passe o arquivo de saída: -o <arquivo>\n");
    exit(1);
  }

  llnode list = frontend(in_filename);
  llnode p = list;

  gen(out_filename, p);

  if (tree_filename) {
    FILE *tree_file = fopen(tree_filename, "w");
    if (!tree_file){
      printf("ERRO ao abrir o arquivo %s\n", tree_filename);
      exit(1);
    }

    p = list;
    while (p != NULL) {
      printtree(p->tree, 0, tree_file);
      p = p->next;
    }
  }


  return 0;
}