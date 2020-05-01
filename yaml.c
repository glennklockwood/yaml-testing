#include <stdio.h>
#include <yaml.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Syntax: %s <yaml file>\n", argv[0]);
        return 1;
    }
    FILE *fh = fopen(argv[1], "r");
    yaml_parser_t parser;
    yaml_token_t  token;   /* new variable */
    yaml_token_type_t last_token = YAML_NO_TOKEN;

    /* Initialize parser */
    if (!yaml_parser_initialize(&parser))
        fprintf(stderr, "Failed to initialize parser!\n");
    if (fh == NULL)
        fprintf(stderr, "Failed to open file!\n");

    /* Set input file */
    yaml_parser_set_input_file(&parser, fh);

    /* BEGIN new code */
    int level = 0;
    do
    {
        for (int i = 0; i < level; i++)
            printf(".");


        yaml_parser_scan(&parser, &token);
        switch(token.type)
        {
            /* Stream start/end */
            case YAML_STREAM_START_TOKEN:
                printf("STREAM START\n");
                break;
            case YAML_STREAM_END_TOKEN:
                printf("STREAM END\n");
                break;

            /* Token types (read before actual token) */
            case YAML_KEY_TOKEN:
                printf("(Key token)   ");
                break;
            case YAML_VALUE_TOKEN:
                printf("(Value token) ");
                break;

            /* Block delimiters */
            case YAML_BLOCK_SEQUENCE_START_TOKEN:
                printf("Start Block (Sequence)\n");
                level++;
                break;
            case YAML_BLOCK_ENTRY_TOKEN:
                printf("Start Block (Entry)\n");
                level++;
                break;
            case YAML_BLOCK_END_TOKEN:
                printf("End block\n");
                level--;
                break;

            /* Data */
            case YAML_BLOCK_MAPPING_START_TOKEN:
                printf("[Block mapping]\n");
                break;
            case YAML_SCALAR_TOKEN:
                printf("scalar: %s\n", token.data.scalar.value);
                if (last_token == YAML_BLOCK_ENTRY_TOKEN)
                    level--;
                break;

            /* Others */
            default:
                printf("Got token of type %d\n", token.type);
        }
        last_token = token.type;
        if (token.type != YAML_STREAM_END_TOKEN)
            yaml_token_delete(&token);
    } while (token.type != YAML_STREAM_END_TOKEN);
    yaml_token_delete(&token);
    /* END new code */

    /* Cleanup */
    yaml_parser_delete(&parser);
    fclose(fh);
    return 0;
}
