#include "tree.h"
#include "utils.h"

ErrorCode PrintTree(Node* node, FILE* outFile);

ErrorCode printTreeGraph(Node* node, FILE* outFile);

ErrorCode CreateTree(Tree* tree)
{
    AssertSoft(tree, NULL_PTR);

    SafeCalloc(tempNode, 1, Node, NO_MEMORY);

    tree->root = tempNode;
    tree->size = 0;

    tree->root->left   = NULL;
    tree->root->right  = NULL;
    tree->root->parent = NULL;

    tree->error = 0;

    return OK;
}

ErrorCode deleteNode(Node* node)
{
    if (node->left)
        deleteNode(node->left);
    
    if (node->right)
        deleteNode(node->right);

    node->right  = NULL;
    node->left   = NULL;
    node->parent = NULL;

    free(node); 

    return OK;
}

ErrorCode DestroyTree(Tree* tree)
{
    AssertSoft(tree, NULL);

    deleteNode(tree->root);

    tree->root = NULL;

    tree->error = 0;

    tree->size = 0;

    return OK;
}

Node* InsertLeft(Tree* tree, Node* node, /* char* */ int data)
{
    AssertSoft(tree, NULL);
    AssertSoft(node, NULL);

    if (tree->size == 0)
    {
        tree->root->data = data;

        tree->size++;

        return tree->root;
    }

    if (node->left)
    {
        tree->error = REPEAT_INSERT_LEFT;

        return NULL;
    }

    SafeCalloc(newNode, 1, Node, NULL);

    newNode->left = NULL;
    newNode->right = NULL;

    newNode->data = data;

    node->left = newNode;
    newNode->parent = node;

    tree->size++;

    return newNode;
}

Node* InsertRight(Tree* tree, Node* node, int data)
{
    AssertSoft(tree, NULL);
    AssertSoft(node, NULL);

    if (tree->size == 0)
    {
        tree->root->data = data;

        tree->size++;

        return tree->root;
    }

    if (node->right)
    {
        tree->error = REPEAT_INSERT_RIGHT;

        return NULL;
    }

    SafeCalloc(newNode, 1, Node, NULL);

    newNode->left = NULL;
    newNode->right = NULL;

    newNode->data = data;

    node->right = newNode;
    newNode->parent = node;

    tree->size++;

    return newNode;
}

ErrorCode checkTreeLinks(Tree* tree, Node* node, size_t* counter)
{
    AssertSoft(tree, NULL_PTR);
    
    AssertSoft(node, NULL_PTR); 

    (*counter)++;

    if (node->left != NULL && (node->left->parent != node || node->right->parent != node))
    {
        tree->error = TREE_LINKING_ERROR;
        return TREE_LINKING_ERROR;
    }

    if (node->left != NULL)
        checkTreeLinks(tree, node->left, counter);
    
    if (node->right != NULL)
        checkTreeLinks(tree, node->right, counter);

    return OK;
}

ErrorCode PrintTree(Node* node, FILE* outFile)
{
    if (node == NULL)
    {
        fprintf(outFile, "nil");
        return OK;
    }

    fprintf(outFile, "(");

    PrintTree(node->left, outFile);

    fprintf(outFile, " "SPECIFIER" ", node->data); 

    PrintTree(node->right, outFile);

    fprintf(outFile, ")");

    return OK;
}

ErrorCode DumpTreeGraph(Node* node, const char* filename)
{
    FILE* outFile = fopen(filename, "w+");

    fprintf(outFile, "  digraph tree"
                     "  {\n"
                     "  node [shape = \"circle\", style = \"filled\", fillcolor = \"blue\", fontcolor = \"#FFFFFF\", margin = \"0.01\"];\n"
                     "  rankdir = \"TB\";\n\n"
                     "  label = \"Tree Dump\";\n");

    printTreeGraph(node, outFile);

    fprintf(outFile, "  }");

    fclose(outFile);
}

ErrorCode printTreeGraph(Node* node, FILE* outFile)
{
    if (node->left == NULL && node->right == NULL)
        fprintf(outFile, "  \"%d\" [shape = \"record\", fillcolor = \"red\", label = \"{%d | parent\\n%p | <f0> pos\\n%p| left\\n%p | right\\n%p\\n}\"];\n", node->data, node->data, node->parent, node, node->left, node->right);
    else
        fprintf(outFile, "  \"%d\" [shape = \"record\", label = \"{%d | parent\\n%p | <f0> pos\\n%p| left\\n%p | right\\n%p\\n}\"];\n", node->data, node->data, node->parent, node, node->left, node->right);

    if (node->left != NULL)
    {
        if (node->left->parent == node)
        {
            fprintf(outFile, "  \"%d\"->\"%d\";\n", node->data, node->left->data);
            fprintf(outFile, "  \"%d\"->\"%d\";\n", node->left->data, node->data);
        }
        else
        {
            fprintf(outFile, "  edge [color=\"#FE6200\"];\n");
            fprintf(outFile, "  \"%d\"->\"%d\";\n", node->data, node->left->data);
            fprintf(outFile, "  \"%d\"->\"%d\";\n", node->left->data, node->left->parent->data);
            fprintf(outFile, "  edge [color=\"#000000\"];\n");
        }

        printTreeGraph(node->left, outFile);
    }

    if (node->right != NULL)
    {
        if (node->right->parent == node)
        {
            fprintf(outFile, "  \"%d\"->\"%d\";\n", node->data, node->right->data);
            fprintf(outFile, "  \"%d\"->\"%d\";\n", node->right->data, node->data);
        }
        else
        {
            fprintf(outFile, "  edge [color=\"#FE6200\"];\n");
            fprintf(outFile, "  \"%d\"->\"%d\";\n", node->data, node->right->data);
            fprintf(outFile, "  \"%d\"->\"%d\";\n", node->right->data, node->right->parent->data);
            fprintf(outFile, "  edge [color=\"#000000\"];\n");
        }

        printTreeGraph(node->right, outFile);
    }
    
    return OK;
}


