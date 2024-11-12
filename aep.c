#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

#define XOR_KEY 42 

char *criptografaXOR(const char *string) {
    static char result[256];
    int stringSize = strlen(string);
    
    for (int i = 0; i < stringSize; i++) {
        result[i] = string[i] ^ XOR_KEY; 
    }
    result[stringSize] = '\0'; 
    return result;
}

char *descriptografaXOR(const char *string) {
    return criptografaXOR(string);
}

void lista()
{
    FILE *arquivo = fopen("usuarios.txt", "r");
    char linha[250], usuario[100], senha[100];
    char usuario_descript[100],senha_descript[100];

    if (arquivo == NULL)
    {
        perror("Nao foi encontrado usuarios!");
        
        return;
    }
    
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        sscanf(linha, "%99[^,], %99[^\n]", usuario, senha);

        strcpy(usuario_descript, descriptografaXOR(usuario)); 
        strcpy(senha_descript, descriptografaXOR(senha)); 

        printf("usuario: %s\nsenha: %s\n-----------------------\n", usuario_descript, senha_descript);
    }
}

void cadastro()
{
    FILE *arquivo = fopen("usuarios.txt", "a+");
    
    char linha[250] = "", usuario[100], senha[100];
    printf("usuario: ");
    
    while (getchar() != '\n');
    fgets(usuario, 99, stdin);
    printf("senha: ");
    fgets(senha, 99, stdin);
    usuario[strcspn(usuario, "\n")] = '\0';
    senha[strcspn(senha, "\n")] = '\0';
    strcpy(linha, criptografaXOR(usuario));
    strcat(linha, ", ");
    strcat(linha, criptografaXOR(senha));
    fputs(linha, arquivo);
    fputs("\n", arquivo);
    fclose(arquivo);
}

void editar(const char *editarUsuario)
{
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL)
    {
        perror("Não foi possível abrir o arquivo user.txt");
        return;
    }

    FILE *temporario = fopen("temp.txt", "w");
    if (temporario == NULL)
    {
        perror("Erro ao criar o arquivo temp.txt");
        fclose(arquivo);
        return;
    }

    char linha[250], usuario[100], senha[100];
    int encontrarUsuario = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        sscanf(linha, "%99[^,], %99[^\n]", usuario, senha);

        if (strcmp(usuario, editarUsuario) == 0)
        {
            encontrarUsuario = 1;
            printf("Usuário encontrado: %s\n", usuario);
            printf("Digite o novo usuário: ");
            scanf(" %99s", usuario);
            printf("Digite a nova senha: ");
            scanf(" %99s", senha);

            // Escreve o novo usuário e senha criptografados no arquivo temporário
            fprintf(temporario, "%s, %s\n", criptografaXOR(usuario), criptografaXOR(senha));
        }
        else
        {
            // Caso o usuário não tenha sido encontrado, copia as linhas para o arquivo temporário
            fputs(linha, temporario);
        }
    }

    fclose(arquivo);
    fclose(temporario);

    if (encontrarUsuario)
    {
        // Substitui o arquivo original pelo arquivo temporário
        arquivo = fopen("usuarios.txt", "w");
        if (arquivo == NULL)
        {
            perror("Erro ao reabrir o arquivo user.txt");
            return;
        }

        temporario = fopen("temp.txt", "r");
        if (temporario == NULL)
        {
            perror("Erro ao abrir o arquivo temp.txt");
            fclose(arquivo);
            return;
        }

        while (fgets(linha, sizeof(linha), temporario) != NULL)
        {
            fputs(linha, arquivo);
        }

        fclose(arquivo);
        fclose(temporario);
        printf("Usuário editado com sucesso!\n");
    }
    else
    {
        printf("Usuário não encontrado\n");
    }
}

void excluir(const char *deletarUsuario)
{
    FILE *arquivo = fopen("user.txt", "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo user.txt");
        return;
    }

    FILE *temporario = fopen("temp.txt", "w");
    if (temporario == NULL)
    {
        perror("Erro ao criar o arquivo temp.txt");
        fclose(arquivo);
        return;
    }

    char linha[250], usuario[100], senha[100];
    int encontrarUsuario = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        sscanf(linha, "%99[^,], %99[^\n]", usuario, senha);

        if (strcmp(usuario, deletarUsuario) == 0)
        {
            encontrarUsuario = 1;
            printf("Usuário %s excluído.\n", descriptografaXOR(usuario));
            continue;  // Pula a escrita dessa linha
        }

        fputs(linha, temporario);  // Escreve as outras linhas
    }

    fclose(arquivo);
    fclose(temporario);

    if (encontrarUsuario)
    {
        // Substitui o arquivo original pelo arquivo temporário
        arquivo = fopen("user.txt", "w");
        if (arquivo == NULL)
        {
            printf("Erro ao reabrir o arquivo user.txt");
            return;
        }

        temporario = fopen("temp.txt", "r");
        if (temporario == NULL)
        {
            perror("Erro ao abrir o arquivo temp.txt");
            fclose(arquivo);
            return;
        }

        while (fgets(linha, sizeof(linha), temporario) != NULL)
        {
            fputs(linha, arquivo);
        }

        fclose(arquivo);
        fclose(temporario);
        printf("Arquivo atualizado com sucesso!\n");
    }
    else
    {
        printf("Usuário não encontrado.\n");
    }
}

int main() 
{
    char nome[100];
    int opcao;

    setlocale(LC_ALL,"");
    opcao = 1;
    while(opcao != 0)
    {
        printf("Escolha uma opcao: \n");
        printf("1 - Lista de Usuarios\n");
        printf("2 - Cadastrar novo Usuario\n");
        printf("3 - Editar Usuario\n");
        printf("4 - Excluir Usuario\n");
        printf("0 - Sair do Programa\n");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 0:
            printf("Encerrado.");
            break;
        case 1:
            lista();
            break;
        case 2:
            cadastro();
            break;
        case 3:
            printf("usuario que deseja editar: ");
            scanf(" %99s", nome);
            editar(criptografaXOR(nome));
            break;
        case 4:
            printf("usuario que deseja excluir: ");
            scanf(" %99s", nome);
            excluir(criptografaXOR(nome));
            break;
        
        default:
            printf("erro.");
            break;
        }
    }

    return 0;
}
