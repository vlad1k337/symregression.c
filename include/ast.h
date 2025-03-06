#ifndef AST_H__
#define AST_H__

#include <stdlib.h>
#include <stdint.h>

#define NODE_UNREACHABLE(message) do { fprintf(stderr, "%s:%d: NODE UNREACHABLE: %s\n", __func__, __LINE__, message); abort(); } while(0)

typedef enum {
	CONST,
	VAR,

	ADD,
	SUB,
	MUL,

	SIN,
	COS,
	ABS
} node_type;


typedef enum {
	VALUE,
	BIN_OP,
	UNARY_OP
} op_type;


typedef struct Node Node;

struct Node {
	node_type type;
	op_type operation;

	union {
		struct { double value; } ;		
		struct { double* variable; } ;
		struct { Node* left; Node* right; } ;		
		struct { Node* child; } ;
	};
};


Node* node_const_create(double value);
Node* node_var_create(double* variable);
Node* node_unop_create(node_type type);
Node* node_binop_create(node_type type);


/* Returns a random unary/binary expression with given variable. */
Node* node_base_create_random(double* variable);


/* Returns a random subtree of a given tree. Pass count as 0, please. */
void node_get_random(Node** root, Node*** result, uint32_t* count);


void node_free(Node* root);


void node_crossover(Node** a, Node** b);


/* Expands every tree leaf with random base node */
void node_expand(Node** root, double* variable);


double node_parse(Node* root);


void node_print(Node* root);


op_type get_op_from_type(node_type type);

node_type get_unop_random();
node_type get_binop_random();

#endif /* AST_H__ */
