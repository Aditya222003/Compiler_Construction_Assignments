class IntermediateCodeOptimizer:
    def __init__(self, code):
        # Intermediate code represented as a list of tuples (operation, operand1, operand2, result)
        self.code = code

    # Constant Folding Optimization
    def constant_folding(self):
        for i, (op, arg1, arg2, result) in enumerate(self.code):
            if op in ['+', '-', '*', '/']:
                try:
                    # Try to fold constants
                    arg1 = int(arg1) if arg1.isdigit() else arg1
                    arg2 = int(arg2) if arg2.isdigit() else arg2
                    if isinstance(arg1, int) and isinstance(arg2, int):
                        # Perform the operation if both arguments are constants
                        if op == '+':
                            value = arg1 + arg2
                        elif op == '-':
                            value = arg1 - arg2
                        elif op == '*':
                            value = arg1 * arg2
                        elif op == '/':
                            value = arg1 // arg2  # Assuming integer division for simplicity
                        # Update code with folded result
                        self.code[i] = ('=', str(value), '', result)
                except ValueError:
                    pass  # Skip folding if operands are not constants

    # Constant Propagation Optimization
    def constant_propagation(self):
        value_map = {}
        for i, (op, arg1, arg2, result) in enumerate(self.code):
            # Track the constants
            if op == '=' and arg2 == '':
                value_map[result] = arg1
            # Propagate constants
            elif arg1 in value_map:
                self.code[i] = (op, value_map[arg1], arg2, result)
            elif arg2 in value_map:
                self.code[i] = (op, arg1, value_map[arg2], result)

    # Print the intermediate code
    def print_code(self):
        for code in self.code:
            print(" ".join(code))


# Example Intermediate Code
intermediate_code = [
    ('=', '3', '', 't1'),
    ('=', '5', '', 't2'),
    ('+', 't1', 't2', 't3'),
    ('*', 't3', '10', 't4'),
    ('-', 't4', '3', 't5'),
]

# Create an optimizer object
optimizer = IntermediateCodeOptimizer(intermediate_code)

# Before optimization
print("Before Optimization:")
optimizer.print_code()

# Apply optimizations
optimizer.constant_folding()
optimizer.constant_propagation()

# After optimization
print("\nAfter Optimization:")
optimizer.print_code()
