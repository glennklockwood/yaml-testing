/*
 *  Largely stolen from https://stackoverflow.com/questions/36410122/c-libyaml-document-based-parsing
 */
#include <stdio.h>
#include <yaml.h>

void print_yaml_node(yaml_document_t *document_p, yaml_node_t *node)
{
    yaml_node_t *next_node_p;

    switch (node->type)
    {
        case YAML_NO_NODE:
            printf("Empty node:\n");
            break;

        case YAML_SCALAR_NODE:
            printf("%s(type=%s, length=%lu)", node->data.scalar.value, node->tag, node->data.scalar.length);
            break;

        case YAML_SEQUENCE_NODE:
            printf("Sequence node:\n");
            yaml_node_item_t *i_node;
            for (i_node = node->data.sequence.items.start; i_node < node->data.sequence.items.top; i_node++) {
                next_node_p = yaml_document_get_node(document_p, *i_node);
                if (next_node_p)
                {
                    print_yaml_node(document_p, next_node_p);
                    printf("\n");
                }
            }
            break;

        case YAML_MAPPING_NODE:
            printf("Mapping node:\n");

            yaml_node_pair_t *i_node_p;
            for (i_node_p = node->data.mapping.pairs.start; i_node_p < node->data.mapping.pairs.top; i_node_p++)
            {
                next_node_p = yaml_document_get_node(document_p, i_node_p->key);
                if (next_node_p)
                {
                    printf("Key: ");
                    print_yaml_node(document_p, next_node_p);
                    printf(", ");
                }
                else
                {
                    fprintf(stderr, "Couldn't find next node\n");
                    exit(1);
                }

                next_node_p = yaml_document_get_node(document_p, i_node_p->value);
                if (next_node_p)
                {
                    printf("Value: ");
                    print_yaml_node(document_p, next_node_p);
                    printf("\n");
                }
                else
                {
                    fprintf(stderr, "Couldn't find next node\n");
                    exit(1);
                }
            }
            break;

        default:
            fprintf(stderr, "Unknown node type\n");
            exit(1);
            break;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Syntax: %s <yaml file>\n", argv[0]);
        return 1;
    }

    FILE *fh = fopen(argv[1], "r");
    yaml_parser_t parser;
    yaml_document_t document;

    /* Initialize parser */
    if (!yaml_parser_initialize(&parser))
    {
        fprintf(stderr, "Failed to initialize parser!\n");
        return 1;
    }

    if (fh == NULL)
    {
        fprintf(stderr, "Failed to open file!\n");
        return 1;
    }

    /* Set input file */
    yaml_parser_set_input_file(&parser, fh);

    int done = 0;
    int error = 0;
    while (!done)
    {
        if (!yaml_parser_load(&parser, &document))
        {
            fprintf(stderr, "Failed to load document\n");
            error = 1;
            break;
        }

        done = (!yaml_document_get_root_node(&document));

        if (!done)
        {
            printf("---\n");
            print_yaml_node(&document, yaml_document_get_root_node(&document));
        }

        yaml_document_delete(&document);
    }

    yaml_parser_delete(&parser);

    fclose(fh);

    return !error;
}
