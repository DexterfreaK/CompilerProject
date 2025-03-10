#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "tree.h"
// Define the TreeNode structure

// Stack implementation for TreeNode pointers
// Initialize a new stack with given capacity
Token* createDummyToken(const char* lexeme) {
    Token* token = malloc(sizeof(Token));
    if (token == NULL) {
        fprintf(stderr, "Memory allocation failed in createDummyToken\n");
        exit(EXIT_FAILURE);
    }
    // Copy the lexeme into the fixed-size array safely.
    strncpy(token->lexeme, lexeme, BUFFER_SIZE);
    token->lexeme[BUFFER_SIZE - 1] = '\0';  // Ensure null termination
    // You can set default values for type, cat, and lineNo as needed.
    token->type = 0;    // Default or dummy value
    token->cat = 0;     // Default or dummy value
    token->lineNo = 0;  // Default or dummy value
    return token;
}
void createStack(Stack *stack, int capacity) {
    stack->array = (TreeNode **)malloc(capacity * sizeof(TreeNode *));
    if (stack->array == NULL) {
        fprintf(stderr, "Memory allocation failed in createStack\n");
        exit(EXIT_FAILURE);
    }
    stack->capacity = capacity;
    stack->top = -1;  // Stack starts empty
}

// Free the memory used by the stack
void deleteStack(Stack *stack) {
    free(stack->array);
    stack->array = NULL;
    stack->capacity = 0;
    stack->top = -1;
}

// Check if stack is full
int isStackFull(const Stack *stack) {
    return stack->top == stack->capacity - 1;
}

// Check if stack is empty
int isStackEmpty(const Stack *stack) {
    return stack->top == -1;
}

// Resize the stack to double its capacity
void resizeStack(Stack *stack) {
    int oldCapacity = stack->capacity;
    int newCapacity = oldCapacity * 2;
    TreeNode **newArray = (TreeNode **)realloc(stack->array, newCapacity * sizeof(TreeNode *));
    if (newArray == NULL) {
        fprintf(stderr, "Memory allocation failed in resizeStack\n");
        exit(EXIT_FAILURE);
    }
    stack->array = newArray;
    stack->capacity = newCapacity;
    printf("Stack resized from %d to %d elements\n", oldCapacity, newCapacity);
}

// Push a TreeNode pointer onto the stack
int pushStack(Stack *stack, TreeNode *node) {
    if (isStackFull(stack)) {
        resizeStack(stack);
    }
    stack->array[++stack->top] = node;
    return 1;  // Success
}

// Pop a TreeNode pointer from the stack
int popStack(Stack *stack, TreeNode **node) {
    if (isStackEmpty(stack)) {
        return 0;  // Failure, stack is empty
    }
    *node = stack->array[stack->top--];
    return 1;  // Success
}

// Get the top TreeNode pointer without removing it
int topStack(Stack *stack, TreeNode **node) {
    if (isStackEmpty(stack)) {
        return 0;  // Failure, stack is empty
    }
    *node = stack->array[stack->top];
    return 1;  // Success
}

// Print the stack (for debugging)
// For nonterminal nodes, prints the symbolID; for terminal nodes, prints the token's lexeme.
void printStack(const Stack *stack) {
    printf("Stack contents (top to bottom):\n");
    for (int i = stack->top; i >= 0; i--) {
        TreeNode *node = stack->array[i];
        if (node->nodeType == NODE_TYPE_TERMINAL) {
            char* tokenStr = (node->content.terminal.token && node->content.terminal.token->lexeme)
                                ? node->content.terminal.token->lexeme : "NULL";
            printf("[%d]: %p (Type: TERMINAL, Token: %s)\n", i, (void*)node, tokenStr);
        } else {
            printf("[%d]: %p (Type: NONTERMINAL, SymbolID: %d)\n", i, (void*)node, node->content.nonterminal.symbolID);
        }
    }
    printf("Stack capacity: %d, Used: %d\n", stack->capacity, stack->top + 1);
}


// ----- Main function to test the stack implementation -----

// int stack_main() {
//     Stack nodeStack;
//     TreeNode *poppedNode;
//     int initialCapacity = 3;  // Small initial capacity to test resizing

//     printf("=== TreeNode Stack Test Program ===\n\n");
    
//     // Create the stack
//     createStack(&nodeStack, initialCapacity);
//     printf("Created stack with initial capacity: %d\n\n", initialCapacity);
    
//     // Create some TreeNodes according to the new tree definition.
//     // For nonterminal nodes we use createNonTerminalNode(symbolID, numChildren)
//     // For terminal nodes we use createTerminalNode(Token*) with a dummy token.
//     TreeNode* root = createNonTerminalNode(1, 2);   // symbolID 1, assume 2 children
//     TreeNode* child1 = createNonTerminalNode(2, 2);   // symbolID 2, assume 2 children
//     TreeNode* child2 = createNonTerminalNode(3, 0);   // symbolID 3, leaf nonterminal (no children)
//     TreeNode* leaf1 = createTerminalNode(createDummyToken("Leaf1"));
//     TreeNode* leaf2 = createTerminalNode(createDummyToken("Leaf2"));
    
//     // Optionally set parent pointers (if needed for your tree)
//     child1->parent = root;
//     child2->parent = root;
//     leaf1->parent = child1;
//     leaf2->parent = child1;
    
//     // Push nodes onto the stack
//     printf("Pushing nodes onto stack...\n");
//     pushStack(&nodeStack, root);
//     printf("Pushed root node: %p (SymbolID: %d)\n", (void*)root, root->content.nonterminal.symbolID);
    
//     pushStack(&nodeStack, child1);
//     printf("Pushed child1 node: %p (SymbolID: %d)\n", (void*)child1, child1->content.nonterminal.symbolID);
    
//     pushStack(&nodeStack, child2);
//     printf("Pushed child2 node: %p (SymbolID: %d)\n", (void*)child2, child2->content.nonterminal.symbolID);
    
//     // This should trigger resizing
//     pushStack(&nodeStack, leaf1);
//     printf("Pushed leaf1 node: %p (Token: %s)\n", (void*)leaf1, leaf1->content.terminal.token->lexeme);
    
//     pushStack(&nodeStack, leaf2);
//     printf("Pushed leaf2 node: %p (Token: %s)\n\n", (void*)leaf2, leaf2->content.terminal.token->lexeme);
    
//     // Print stack contents
//     printf("Current stack state:\n");
//     printStack(&nodeStack);
//     printf("\n");
    
//     // Test peek (top) operation
//     if (topStack(&nodeStack, &poppedNode)) {
//         if (poppedNode->nodeType == NODE_TYPE_TERMINAL) {
//             char* tokenStr = (poppedNode->content.terminal.token && poppedNode->content.terminal.token->lexeme)
//                                 ? poppedNode->content.terminal.token->lexeme : "NULL";
//             printf("Top node without popping: %p (Type: TERMINAL, Token: %s)\n\n", (void*)poppedNode, tokenStr);
//         } else {
//             printf("Top node without popping: %p (Type: NONTERMINAL, SymbolID: %d)\n\n",
//                    (void*)poppedNode, poppedNode->content.nonterminal.symbolID);
//         }
//     }
    
//     // Pop and process nodes
//     printf("Popping and processing nodes...\n");
//     while (popStack(&nodeStack, &poppedNode)) {
//         if (poppedNode->nodeType == NODE_TYPE_TERMINAL) {
//             char* tokenStr = (poppedNode->content.terminal.token && poppedNode->content.terminal.token->lexeme)
//                                 ? poppedNode->content.terminal.token->lexeme : "NULL";
//             printf("Popped node: %p (Type: TERMINAL, Token: %s)\n", (void*)poppedNode, tokenStr);
//         } else {
//             printf("Popped node: %p (Type: NONTERMINAL, SymbolID: %d)\n", (void*)poppedNode, poppedNode->content.nonterminal.symbolID);
//         }
//     }
//     printf("\n");
    
//     // Check empty stack
//     printf("Stack after popping all elements:\n");
//     printStack(&nodeStack);
//     printf("\n");
    
//     // Test empty stack operations
//     if (!popStack(&nodeStack, &poppedNode)) {
//         printf("Pop on empty stack correctly returned failure\n");
//     }
    
//     if (!topStack(&nodeStack, &poppedNode)) {
//         printf("Top on empty stack correctly returned failure\n");
//     }
    
//     // Free memory: first delete the stack, then free each node.
//     // Note: In an actual syntax tree the nodes would be freed via a recursive tree-freeing function.
//     printf("\nCleaning up resources...\n");
//     // deleteStack(&nodeStack);
//     // freeTreeNode(root);
//     // freeTreeNode(child1);
//     // freeTreeNode(child2);
//     // freeTreeNode(leaf1);
//     // freeTreeNode(leaf2);
    
//     printf("\n=== Test completed successfully ===\n");
    
//     return 0;
// }