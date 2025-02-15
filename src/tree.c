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



// // Example usage of the syntax tree functions.
// if(a<b) then a=a+1 else b=b+1

// int main(void) {
//     // -------------------------------
//     // Create Tokens for the if-statement components.
//     // -------------------------------
//     // Condition: a < b
//     Token* tok_a_cond = createToken("a");
//     Token* tok_lt     = createToken("<");
//     Token* tok_b_cond = createToken("b");

//     // Then branch (assignment): a = a + 1
//     Token* tok_a_then1 = createToken("a");  // left-hand side of assignment
//     Token* tok_eq_then = createToken("=");  // assignment operator
//     Token* tok_a_then2 = createToken("a");  // left-hand side of addition
//     Token* tok_plus_then = createToken("+");// addition operator
//     Token* tok_1_then   = createToken("1");  // right-hand side of addition

//     // Else branch (assignment): b = b + 1
//     Token* tok_b_else1 = createToken("b");  // left-hand side of assignment
//     Token* tok_eq_else = createToken("=");  // assignment operator
//     Token* tok_b_else2 = createToken("b");  // left-hand side of addition
//     Token* tok_plus_else = createToken("+");// addition operator
//     Token* tok_1_else   = createToken("1");  // right-hand side of addition

//     // -------------------------------
//     // Build the AST for each part.
//     // -------------------------------

//     // 1. Condition Node for "a < b"
//     // Create a nonterminal node for the condition with 3 children: [a] [<] [b]
//     SyntaxTreeNode* condNode = createNonTerminalNode(101, 3);
//     setChild(condNode, 0, createTerminalNode(tok_a_cond));
//     setChild(condNode, 1, createTerminalNode(tok_lt));
//     setChild(condNode, 2, createTerminalNode(tok_b_cond));

//     // 2. Then Branch: Assignment "a = a + 1"
//     // Create a nonterminal node for the assignment (symbolID 102) with 3 children.
//     SyntaxTreeNode* thenAssign = createNonTerminalNode(102, 3);
//     // Left-hand side (variable "a")
//     setChild(thenAssign, 0, createTerminalNode(tok_a_then1));
//     // Assignment operator "="
//     setChild(thenAssign, 1, createTerminalNode(tok_eq_then));

//     // Right-hand side: an addition expression "a + 1"
//     // Create a nonterminal node for addition (symbolID 103) with 3 children.
//     SyntaxTreeNode* addExprThen = createNonTerminalNode(103, 3);
//     setChild(addExprThen, 0, createTerminalNode(tok_a_then2));
//     setChild(addExprThen, 1, createTerminalNode(tok_plus_then));
//     setChild(addExprThen, 2, createTerminalNode(tok_1_then));
//     // Link the addition expression as the right-hand side of the assignment.
//     setChild(thenAssign, 2, addExprThen);

//     // 3. Else Branch: Assignment "b = b + 1"
//     // Create a nonterminal node for the assignment (symbolID 104) with 3 children.
//     SyntaxTreeNode* elseAssign = createNonTerminalNode(104, 3);
//     // Left-hand side (variable "b")
//     setChild(elseAssign, 0, createTerminalNode(tok_b_else1));
//     // Assignment operator "="
//     setChild(elseAssign, 1, createTerminalNode(tok_eq_else));

//     // Right-hand side: an addition expression "b + 1"
//     // Create a nonterminal node for addition (symbolID 105) with 3 children.
//     SyntaxTreeNode* addExprElse = createNonTerminalNode(105, 3);
//     setChild(addExprElse, 0, createTerminalNode(tok_b_else2));
//     setChild(addExprElse, 1, createTerminalNode(tok_plus_else));
//     setChild(addExprElse, 2, createTerminalNode(tok_1_else));
//     // Link the addition expression as the right-hand side of the assignment.
//     setChild(elseAssign, 2, addExprElse);

//     // 4. Build the If-Statement Node
//     // Create a nonterminal node for the if-statement (symbolID 100) with 3 children:
//     // [condition] [then branch] [else branch]
//     SyntaxTreeNode* ifStmt = createNonTerminalNode(100, 3);
//     setChild(ifStmt, 0, condNode);
//     setChild(ifStmt, 1, thenAssign);
//     setChild(ifStmt, 2, elseAssign);

//     // -------------------------------
//     // Print and free the AST.
//     // -------------------------------
//     printf("Printing syntax tree for the if-statement:\n");
//     printSyntaxTree(ifStmt, 0);

//     // Clean up all allocated memory in the tree.
//     freeSyntaxTree(ifStmt);

//     return 0;
// }

