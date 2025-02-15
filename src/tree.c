#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

// Create a new terminal (leaf) node.
TreeNode* createTerminalNode(Token* token) {
   TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for terminal node.\n");
        exit(EXIT_FAILURE);
    }
    node->nodeType = NODE_TYPE_TERMINAL;
    node->parent = NULL;
    node->content.terminal.token = token;
    return node;
}

// Create a new nonterminal (internal) node.
TreeNode* createNonTerminalNode(int symbolID, int numChildren) {
    TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for nonterminal node.\n");
        exit(EXIT_FAILURE);
    }
    node->nodeType = NODE_TYPE_NONTERMINAL;
    node->parent = NULL;
    node->content.nonterminal.symbolID = symbolID;
    node->content.nonterminal.numChildren = numChildren;
    if (numChildren > 0) {
        node->content.nonterminal.children = (TreeNode**) calloc(numChildren, sizeof(TreeNode*));
        if (!node->content.nonterminal.children) {
            fprintf(stderr, "Error: Memory allocation failed for children array.\n");
            free(node);
            exit(EXIT_FAILURE);
        }
    } else {
        node->content.nonterminal.children = NULL;
    }
    return node;
}

// Set a child node at a given index for a nonterminal parent.
void setChild(TreeNode* parent, int index, TreeNode* child) {
    if (!parent || parent->nodeType != NODE_TYPE_NONTERMINAL) {
        fprintf(stderr, "setChild Error: Provided parent is NULL or not a nonterminal.\n");
        return;
    }
    if (index < 0 || index >= parent->content.nonterminal.numChildren) {
        fprintf(stderr, "setChild Error: Index %d out of bounds (max %d).\n", index, parent->content.nonterminal.numChildren);
        return;
    }
    parent->content.nonterminal.children[index] = child;
    if (child) {
        child->parent = parent;
    }
}

// Helper function to print indentation.
static void printIndent(int indentLevel) {
    for (int i = 0; i < indentLevel; i++) {
        printf("  ");
    }
}

// Recursively prints the syntax tree.
void printSyntaxTree(TreeNode* root, int indentLevel) {
    if (!root) return;
    
    printIndent(indentLevel);
    if (root->nodeType == NODE_TYPE_TERMINAL) {
        // Print token information. Adjust according to the actual fields in your Token struct.
        if (root->content.terminal.token && root->content.terminal.token->lexeme) {
            printf("Terminal: %s\n", root->content.terminal.token->lexeme);
        } else {
            printf("Terminal: <NULL>\n");
        }
    } else if (root->nodeType == NODE_TYPE_NONTERMINAL) {
        printf("NonTerminal (ID: %d) with %d child(ren):\n", 
               root->content.nonterminal.symbolID, 
               root->content.nonterminal.numChildren);
        for (int i = 0; i < root->content.nonterminal.numChildren; i++) {
            if (root->content.nonterminal.children[i]) {
                printSyntaxTree(root->content.nonterminal.children[i], indentLevel + 1);
            } else {
                printIndent(indentLevel + 1);
                printf("Child %d: <NULL>\n", i);
            }
        }
    }
}

// Recursively free the syntax tree.
void freeSyntaxTree(TreeNode* root) {
    if (!root) return;
    
    if (root->nodeType == NODE_TYPE_NONTERMINAL) {
        // Free all child nodes.
        for (int i = 0; i < root->content.nonterminal.numChildren; i++) {
            freeSyntaxTree(root->content.nonterminal.children[i]);
        }
        free(root->content.nonterminal.children);
    }
    // Note: If the Token memory is managed elsewhere, do not free it here.
    free(root);
}


// Example usage of the syntax tree functions.
// This example creates a syntax tree for the expression "1 + 2".

// int main(void) {
//     // --- Create tokens for the expression "1 + 2" ---
//     // Assume createToken creates a token with the lexeme provided.
//     Token* token1     = createToken("1");
//     Token* tokenPlus  = createToken("+");
//     Token* token2     = createToken("2");

//     // --- Create terminal (leaf) nodes for each token ---
//     SyntaxTreeNode* leaf1    = createTerminalNode(token1);
//     SyntaxTreeNode* leafPlus = createTerminalNode(tokenPlus);
//     SyntaxTreeNode* leaf2    = createTerminalNode(token2);

//     // --- Create a nonterminal node for the expression ---
//     // Let's use symbolID = 100 to represent an "expression" nonterminal.
//     // This nonterminal node will have 3 children: left operand, operator, right operand.
//     SyntaxTreeNode* exprNode = createNonTerminalNode(100, 3);
    
//     // Attach the terminal nodes to the nonterminal node.
//     setChild(exprNode, 0, leaf1);      // Left operand: "1"
//     setChild(exprNode, 1, leafPlus);     // Operator: "+"
//     setChild(exprNode, 2, leaf2);      // Right operand: "2"

//     // --- Print the tree structure ---
//     printf("Printing syntax tree for expression '1 + 2':\n");
//     printSyntaxTree(exprNode, 0);

//     // --- Clean up memory ---
//     freeSyntaxTree(exprNode);
    
//     return 0;
// }
