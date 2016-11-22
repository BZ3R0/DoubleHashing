#include <stdio.h>
#include <string.h>
#define TAMANHO_ARQUIVO 11

typedef struct Noarchive {
	int key, age;
	char name[21];
}Noarchive;

int media=0;

int hash1 (int chave){
	int h1 = (chave % TAMANHO_ARQUIVO);
	return h1;
}

int hash2 (int chave){
	int h2 = (chave / TAMANHO_ARQUIVO);
	if (h2 == 0)
		h2 = 1;
	return h2;
}

int hash3 (int chave, int i){
	int h3 = ((hash1(chave) + i * hash2(chave)) % TAMANHO_ARQUIVO);
	return h3;
}

void inicialize (Noarchive f){
	int i;
	FILE *fh;

	f.key = -1;
	
	if ((fh = fopen("arquivo.bin", "ab"))){
		for (i=0; i < TAMANHO_ARQUIVO; i++){
			fseek(fh, (i*(sizeof(struct Noarchive))), SEEK_SET);
			fwrite(&f, sizeof(struct Noarchive), 1, fh);
		}
	}
	else
		printf("Unable to open file\n");
	fclose(fh);
}

void insert(Noarchive f){
	int i, chave, idade, j;
	char nome[21];
	FILE *fh;
	int cont=0;

	scanf("%d",&chave);
	scanf ("\n\r%[^\n]",nome);
	scanf ("%d",&idade);

	i = 0;
	int h3 = hash3(chave, i);

	fh = fopen ("arquivo.bin", "rb+");
	rewind(fh);
	if (fh == NULL){
		return;
	}

	if (chave == -1)
		return;

	for (j=0; j<TAMANHO_ARQUIVO; j++){
		fseek(fh, (j * sizeof(struct Noarchive)), SEEK_SET);
		fread(&f, sizeof(struct Noarchive), 1, fh);

		if (f.key == chave){
			printf("chave ja existente: %d\n",chave);
			return;
		}
	}

	fseek(fh, (h3 * sizeof(struct Noarchive)), SEEK_SET);
	fread(&f, sizeof(struct Noarchive), 1, fh);
	fseek(fh, (h3 * (sizeof(Noarchive))), SEEK_SET);
	if (f.key == -1){
		f.key = chave;	
		strcpy(f.name,nome);
		f.age = idade;
		fwrite(&f, sizeof(struct Noarchive), 1, fh);
		fclose(fh);
		return;
	}
	
	else{
		while (1){
			cont++;
			if (cont == TAMANHO_ARQUIVO)
				break;
			i++;
			h3 = hash3(chave, i);
			fseek(fh, (h3 * sizeof(struct Noarchive)), SEEK_SET);
			fread(&f, sizeof(struct Noarchive), 1, fh);
			fseek(fh, (h3 * (sizeof(Noarchive))), SEEK_SET);
			if (f.key == -1){
				f.key = chave;	
				strcpy(f.name,nome);
				f.age = idade;
				fwrite(&f, sizeof(struct Noarchive), 1, fh);
				break;
			}
		}
	}
	fclose(fh);
}

void search(Noarchive f, int chave){
	FILE *fh;
	int j;

	fh = fopen ("arquivo.bin", "rb");
	rewind(fh);
	if (fh == NULL)
		return;

	if (chave == -1)
		return;
	
	for (j=0; j<TAMANHO_ARQUIVO; j++){
		fseek(fh, (j * sizeof(struct Noarchive)), SEEK_SET);
		fread(&f, sizeof(struct Noarchive), 1, fh);

		if (f.key == chave){
			printf("chave: %d\n",chave);
			printf("%s\n", f.name);
			printf("%d\n", f.age);
			return;
		}
	}
	printf("chave nao encontrada: %d\n",chave);
	fclose(fh);
}

void remove_key(Noarchive f, int chave){
	FILE *fh;
	int j;

	fh = fopen ("arquivo.bin", "rb+");
	rewind(fh);
	if (fh == NULL)
		return;
	
	if (chave == -1)
		return;

	for (j = 0; j < TAMANHO_ARQUIVO; j++){
		fseek(fh, (j * sizeof(struct Noarchive)), SEEK_SET);
		fread(&f, sizeof(struct Noarchive), 1, fh);
		fseek(fh, (j * sizeof(struct Noarchive)), SEEK_SET);

		if (f.key == chave){
			f.key = -1;
			fseek(fh, (j * sizeof(struct Noarchive)), SEEK_SET);
			fwrite(&f, sizeof(struct Noarchive), 1, fh);
			fclose(fh);
			return;
		}
	}
	printf("chave nao encontrada: %d\n",chave);
	fclose(fh);
}

void print_file(Noarchive f){
	FILE *fh;
	int i;

	fh = fopen("arquivo.bin", "rb");
	if (fh == NULL)
		return;

	rewind(fh);
	for (i = 0; i < TAMANHO_ARQUIVO; i++){
		fseek(fh, (i * (sizeof(Noarchive))), SEEK_SET);
		fread(&f, sizeof(struct Noarchive), 1, fh);
		fseek(fh, (i * (sizeof(Noarchive))), SEEK_SET);
		if (f.key == -1)
			printf("%d: vazio\n",i);
		else 
			printf("%d: %d %s %d\n", i, f.key, f.name, f.age);
	}
	fclose(fh);
}

void individual_averange(Noarchive f, int chave){
	FILE *fh;
	int i=0;
	media = 0;

	int h3 = hash3(chave,i);
	fh = fopen("arquivo.bin","rb");

	if (fh == NULL)
		return;

	rewind(fh);
	fseek(fh, (h3 * sizeof(struct Noarchive)), SEEK_SET);
	fread(&f, sizeof(struct Noarchive), 1, fh);	
	fseek(fh, (h3 * sizeof(struct Noarchive)), SEEK_SET);

	if (f.key == chave){
		media++;
		fclose(fh);
	}
	else {
		while(f.key != -1){
		i++;
		media++;
		h3 = hash3(chave,i);
		
		rewind(fh);
		fseek(fh, (h3 * sizeof(struct Noarchive)), SEEK_SET);
		fread(&f, sizeof(struct Noarchive), 1, fh);	
		fseek(fh, (h3 * sizeof(struct Noarchive)), SEEK_SET);
			
		if(f.key == chave){
			media++;
			fclose(fh);
			return;
		}
	}}
}

float print_media(Noarchive f){
	FILE *fh;
	int i, j, soma=0;
	float cont;

	fh = fopen("arquivo.bin","rb");

	if (fh == NULL)
		return 1;

	for (i = 0; i < TAMANHO_ARQUIVO; i++){
		fseek(fh, (i * sizeof(struct Noarchive)), SEEK_SET);
		fread(&f, sizeof(struct Noarchive), 1, fh);
		fseek(fh, (i * sizeof(struct Noarchive)), SEEK_SET);

		if (f.key != -1){
			individual_averange(f, f.key);
			cont++;
			soma += media;
		}
	}
	fclose(fh);
	if (cont == 0)
		return 0.0;
	return soma / cont;
}

int exit_program(){
	return 0;
}

int main (){
	char option[1];
	int chave;	
	Noarchive f;

	inicialize(f);

	scanf ("\n\r%[^\n]",option);
	do{
		switch (option[0]){
			case 'i':
				insert(f);
			break;

			case 'c':
				scanf("%d",&chave);
				search(f, chave);
			break;

			case 'r':
				scanf("%d",&chave);
				remove_key(f, chave);
			break;

			case 'p':
				print_file(f);
			break;

			case 'm':
				printf("%.1f\n",print_media(f));
			break;

			case 'e':
				exit_program();
			break;
		}
		scanf("\n\r%[^\n]",option);
	}while (option[0] != 'e');

	return 0;
}
