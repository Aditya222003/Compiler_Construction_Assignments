import ply.yacc as yacc
import math
from lexer import tokens  

precedence = (
    ('left', 'PLUS', 'MINUS'),
    ('left', 'MULTIPLY', 'DIVIDE'),
    ('right', 'POWER'),  
    ('left', 'SIN', 'COS', 'TAN', 'LOG', 'SQRT')
)

names = {}
def p_expression_binop(p):
    '''expression : expression PLUS expression
                  | expression MINUS expression
                  | expression MULTIPLY expression
                  | expression DIVIDE expression
                  | expression POWER expression'''
    if p[2] == '+':
        p[0] = p[1] + p[3]
    elif p[2] == '-':
        p[0] = p[1] - p[3]
    elif p[2] == '*':
        p[0] = p[1] * p[3]
    elif p[2] == '/':
        if p[3] != 0:
            p[0] = p[1] / p[3]
        else:
            print("Error: Division by zero!")
            p[0] = 0
    elif p[2] == '^':
        p[0] = p[1] ** p[3]

def p_expression_paren(p):
    '''expression : LPAREN expression RPAREN'''
    p[0] = p[2]

def p_expression_number(p):
    '''expression : NUMBER'''
    p[0] = p[1]

def p_expression_function(p):
    '''expression : SIN LPAREN expression RPAREN
                  | COS LPAREN expression RPAREN
                  | TAN LPAREN expression RPAREN
                  | LOG LPAREN expression RPAREN
                  | SQRT LPAREN expression RPAREN'''
    if p[1] == 'sin':
        p[0] = math.sin(math.radians(p[3]))  
    elif p[1] == 'cos':
        p[0] = math.cos(math.radians(p[3]))  
    elif p[1] == 'tan':
        p[0] = math.tan(math.radians(p[3]))  
    elif p[1] == 'log':
        if p[3] > 0:
            p[0] = math.log10(p[3])  
        else:
            print("Error: Logarithm of non-positive number!")
            p[0] = 0
    elif p[1] == 'sqrt':
        if p[3] >= 0:
            p[0] = math.sqrt(p[3])
        else:
            print("Error: Square root of negative number!")
            p[0] = 0

def p_error(p):
    print("Syntax error!")

parser = yacc.yacc()

def run_calculator():
    print("Scientific Calculator (type 'quit' to exit)")
    while True:
        try:
            s = input('calc > ')
            if s.lower() == 'quit':
                break
        except EOFError:
            break
        if not s:
            continue
        result = parser.parse(s)
        if result is not None:
            print("Result:", result)

if __name__ == "__main__":
    run_calculator()
