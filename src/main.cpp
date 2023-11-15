#include "tree.h"

int main(void)
{
    FILE* fp = fopen("tree.txt", "w+");

    Tree tree = {};

    CreateTree(&tree);

    Node* node = InsertRight(&tree, tree.root, 10);

    node = InsertRight(&tree, node, 20);

    InsertLeft(&tree, node, 30);

    InsertRight(&tree, node, 50);

    InsertLeft(&tree, tree.root, 40);

    PrintTree(tree.root, fp);

    DumpTreeGraph(tree.root, "treegraph.dot");

    DestroyTree(&tree);

    fclose(fp);

}