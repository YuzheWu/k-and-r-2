/*
 * K&R2 Exercise 7-5 by Yuzhe Wu
 * Rewrite the postfix calculator of Chapter 4 to use scanf and/or
 * sscanf to do the input and number conversion.
 *
 * Assumptions on user input: operators/operands are separated by
 * spaces; last operator/operand in a line is immediately followed by
 * newline character.
 */

#include <stdio.h>

#define MAXOP 100	/* max size of operand or operator */

/* scan input and perform postfix arithmetic operations */
int main() {
	void push(double);
	double pop(void);

	char op[MAXOP], c;
	double operand, op2;

	/* get oprator/operand and immediately following space */
	while (scanf("%s%c", op, &c) == 2) {
		if (sscanf(op, "%lf", &operand) == 1) 	/* op is number */
			push(operand);
		else {									/* op is operator */
			switch (op[0]) {
				case '+':
					push(pop() + pop());
					break;
				case '*':
					push(pop() * pop());
					break;
				case '-':
					op2 = pop();
					push(pop() - op2);
					break;
				case '/':
					op2 = pop();
					if (op2)
						push(pop() / op2);
					else
						printf("error: division by zero\n");
					break;
				default:
					printf("error: unknown command %c\n", op[0]);
					break;
			}
		}
		if (c == '\n')		/* output result at newline */
			printf("\t%.8g\n", pop());
	}
}

/*******************************************************************
 *                All code below here is from K&R2.                *
 *******************************************************************/

/* push and pop are from K&R2, page 77 */
#define MAXVAL 100		/* maximum depth of value stack */

int sp = 0;				/* next free stack position */
double val[MAXVAL];		/* value stack */

/* push: push f onto value stack */
void push(double f) {
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value from stack */
double pop(void) {
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}
