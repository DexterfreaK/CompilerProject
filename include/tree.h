#ifndef COMPILER_TREE_H
#define COMPILER_TREE_H

#include <stdbool.h>
#include "lexer.h"  // Assumes Token type is defined here

// Enumeration for node types.
typedef enum {
    NODE_TYPE_NONTERMINAL,
    NODE_TYPE_TERMINAL
} NodeType;

// Forward declaration for the syntax tree node.
typedef struct TreeNode TreeNode;

// Structure for terminal (leaf) nodes.
typedef struct {
    Token* token;    // Pointer to the token (e.g., identifier, literal, etc.)
} TerminalNode;

// Structure for nonterminal (internal) nodes.
typedef struct {
    int symbolID;              // Identifier for the nonterminal symbol.
    int numChildren;           // Number of child nodes.
    TreeNode** children; // Dynamically allocated array of child node pointers.
} NonTerminalNode;

// Union to store either a terminal or a nonterminal node.
typedef union {
    TerminalNode terminal;
    NonTerminalNode nonterminal;
} NodeContent;

// The syntax tree node structure.
struct TreeNode {
    NodeType nodeType;  // Distinguishes terminal from nonterminal.
    TreeNode* parent; // Pointer to the parent node (NULL for root).
    NodeContent content;    // Node-specific content.
};

// Function Declarations:

/**
 * @brief Creates a new terminal node.
 * 
 * @param token Pointer to the token for this terminal.
 * @return Pointer to the newly created SyntaxTreeNode.
 */
TreeNode* createTerminalNode(Token* token);

/**
 * @brief Creates a new nonterminal node.
 * 
 * @param symbolID Identifier for the nonterminal.
 * @param numChildren The number of child nodes to allocate space for.
 * @return Pointer to the newly created SyntaxTreeNode.
 */
TreeNode* createNonTerminalNode(int symbolID, int numChildren);

/**
 * @brief Sets the child node at the specified index for a nonterminal node.
 * 
 * @param parent The nonterminal parent node.
 * @param index The index in the children array (0-based).
 * @param child The child node to attach.
 */
void setChild(TreeNode* parent, int index,TreeNode* child);

/**
 * @brief Recursively prints the syntax tree.
 * 
 * @param root The root node of the tree/subtree.
 * @param indentLevel The current indentation level (use 0 for root).
 */
void printSyntaxTree(TreeNode* root, int indentLevel);

/**
 * @brief Frees the memory allocated for the syntax tree.
 * 
 * @param root The root node of the tree/subtree.
 */
void freeSyntaxTree(TreeNode* root);

#endif /* COMPILER_TREE_H */
