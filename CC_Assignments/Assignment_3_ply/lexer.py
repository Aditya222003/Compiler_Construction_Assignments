import ply.lex as lex

tokens = [
    'NUMBER',
    'PLUS', 'MINUS', 'MULTIPLY', 'DIVIDE', 'POWER',
    'LPAREN', 'RPAREN',
    'SIN', 'COS', 'TAN', 'LOG', 'SQRT'
]

t_PLUS    = r'\+'
t_MINUS   = r'-'
t_MULTIPLY= r'\*'
t_DIVIDE  = r'/'
t_POWER   = r'\^'
t_LPAREN  = r'\('
t_RPAREN  = r'\)'

t_ignore = ' \t'

def t_NUMBER(t):
    r'\d+(\.\d+)?'
    t.value = float(t.value)  
    return t

def t_SIN(t):
    r'sin'
    return t

def t_COS(t):
    r'cos'
    return t

def t_TAN(t):
    r'tan'
    return t

def t_LOG(t):
    r'log'
    return t

def t_SQRT(t):
    r'sqrt'
    return t

def t_error(t):
    print(f"Illegal character '{t.value[0]}'")
    t.lexer.skip(1)

lexer = lex.lex()
