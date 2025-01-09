import copy

# perform grammar augmentation
def grammarAugmentation(rules, nonterm_userdef, start_symbol):
    # newRules stores processed output rules
    newRules = []

    # create unique 'symbol' to represent new start symbol
    newChar = start_symbol + "'"
    while newChar in nonterm_userdef:
        newChar += "'"

    # adding rule to bring start symbol to RHS
    newRules.append([newChar, ['.', start_symbol]])

    # new format => [LHS,[.RHS]], can't use dictionary since duplicate keys can be there
    for rule in rules:
        # split LHS from RHS
        k = rule.split("->")
        lhs = k[0].strip()
        rhs = k[1].strip()

        # split all rule at '|'
        # keep single derivation in one rule
        multirhs = rhs.split('|')
        for rhs1 in multirhs:
            rhs1 = rhs1.strip().split()
            # ADD dot pointer at start of RHS
            rhs1.insert(0, '.')
            newRules.append([lhs, rhs1])
    return newRules

# find closure
def findClosure(input_state, dotSymbol):
    global start_symbol, separatedRulesList, statesDict

    # closureSet stores processed output
    closureSet = []

    # if findClosure is called for I0, add rules for start symbol
    if dotSymbol == start_symbol:
        for rule in separatedRulesList:
            if rule[0] == dotSymbol:
                closureSet.append(rule)
    else:
        # for any higher state than I0, set initial state as received input_state
        closureSet = input_state

    # iterate till new states are getting added in closureSet
    prevLen = -1
    while prevLen != len(closureSet):
        prevLen = len(closureSet)

        tempClosureSet = []  # temporary closure to avoid concurrent modification error

        # add corresponding rules to tempClosureSet for the dot symbol
        for rule in closureSet:
            indexOfDot = rule[1].index('.')
            if rule[1][-1] != '.':
                dotPointsHere = rule[1][indexOfDot + 1]
                for in_rule in separatedRulesList:
                    if dotPointsHere == in_rule[0] and in_rule not in tempClosureSet:
                        tempClosureSet.append(in_rule)

        # add new closure rules to closureSet
        for rule in tempClosureSet:
            if rule not in closureSet:
                closureSet.append(rule)
    return closureSet

def compute_GOTO(state):
    global statesDict, stateCount
    generateStatesFor = []

    # find all symbols on which GOTO function is needed
    for rule in statesDict[state]:
        if rule[1][-1] != '.':  # if rule is not "Handle"
            indexOfDot = rule[1].index('.')
            dotPointsHere = rule[1][indexOfDot + 1]
            if dotPointsHere not in generateStatesFor:
                generateStatesFor.append(dotPointsHere)

    # call GOTO iteratively on all symbols
    for symbol in generateStatesFor:
        GOTO(state, symbol)
    return

def GOTO(state, charNextToDot):
    global statesDict, stateCount, stateMap

    # newState stores processed new state
    newState = []
    for rule in statesDict[state]:
        indexOfDot = rule[1].index('.')
        if rule[1][-1] != '.':
            if rule[1][indexOfDot + 1] == charNextToDot:
                # perform shift operation
                shiftedRule = copy.deepcopy(rule)
                shiftedRule[1][indexOfDot] = shiftedRule[1][indexOfDot + 1]
                shiftedRule[1][indexOfDot + 1] = '.'
                newState.append(shiftedRule)

    # add closure rules to newState
    addClosureRules = []
    for rule in newState:
        indexDot = rule[1].index('.')
        if rule[1][-1] != '.':
            closureRes = findClosure(newState, rule[1][indexDot + 1])
            for rule in closureRes:
                if rule not in addClosureRules and rule not in newState:
                    addClosureRules.append(rule)

    # add closure result to newState
    for rule in addClosureRules:
        newState.append(rule)

    # check if newState already exists
    stateExists = -1
    for state_num in statesDict:
        if statesDict[state_num] == newState:
            stateExists = state_num
            break

    # stateMap is a mapping of GOTO with its output states
    if stateExists == -1:
        stateCount += 1
        statesDict[stateCount] = newState
        stateMap[(state, charNextToDot)] = stateCount
    else:
        stateMap[(state, charNextToDot)] = stateExists
    return

def generateStates(statesDict):
    prev_len = -1
    called_GOTO_on = []

    # run loop until new states are added
    while len(statesDict) != prev_len:
        prev_len = len(statesDict)
        keys = list(statesDict.keys())

        for key in keys:
            if key not in called_GOTO_on:
                called_GOTO_on.append(key)
                compute_GOTO(key)
    return

# Handle FIRST
# Handle FIRST
def first(rule, visited=None):
    global term_userdef, diction

    # Initialize visited set to prevent recursion loops
    if visited is None:
        visited = set()

    # If the rule starts with a terminal, return that terminal
    if len(rule) != 0 and rule[0] in term_userdef:
        return rule[0]

    # If the rule is epsilon (empty string), return epsilon
    if rule[0] == '#':
        return '#'

    # If we've already visited this non-terminal, avoid recursion
    if rule[0] in visited:
        return []

    # Mark this non-terminal as visited
    visited.add(rule[0])

    if rule[0] in diction:
        fres = []
        rhs_rules = diction[rule[0]]
        
        for rhs in rhs_rules:
            # Recursively find FIRST for the RHS of the rule
            res = first(rhs, visited.copy())  # Use a copy of visited set to avoid interference between recursive calls

            if isinstance(res, list):
                fres.extend(res)
            else:
                fres.append(res)

        if '#' in fres:
            fres.remove('#')
            if len(rule) > 1:
                next_first = first(rule[1:], visited.copy())
                if isinstance(next_first, list):
                    fres.extend(next_first)
                else:
                    fres.append(next_first)
            fres.append('#')  # Add epsilon back if necessary
        return fres
    return []

# Handle FOLLOW
def follow(nt):
    global start_symbol, rules, nonterm_userdef, term_userdef, diction, firsts, follows

    solset = set()
    if nt == start_symbol:
        solset.add('$')

    for curNT in diction:
        rhs = diction[curNT]
        for subrule in rhs:
            if nt in subrule:
                while nt in subrule:
                    index_nt = subrule.index(nt)
                    subrule = subrule[index_nt + 1:]

                    if len(subrule) != 0:
                        res = first(subrule)
                        if '#' in res:
                            res.remove('#')
                            ansNew = follow(curNT)
                            if ansNew is not None:
                                res += ansNew
                    else:
                        if nt != curNT:
                            res = follow(curNT)
                    if res is not None:
                        solset.update(res)
    return list(solset)

# Create Parse Table (handling conflicts)
def createParseTable(statesDict, stateMap, T, NT):
    global separatedRulesList, diction

    rows = list(statesDict.keys())
    cols = T + ['$'] + NT

    Table = [['' for _ in range(len(cols))] for _ in range(len(rows))]

    # add shift and GOTO entries
    for entry in stateMap:
        state = entry[0]
        symbol = entry[1]
        a = rows.index(state)
        b = cols.index(symbol)
        if symbol in NT:
            Table[a][b] = f"{stateMap[entry]}"
        elif symbol in T:
            Table[a][b] = f"S{stateMap[entry]}"

    # add REDUCE entries
    numbered = {}
    key_count = 0
    for rule in separatedRulesList:
        tempRule = copy.deepcopy(rule)
        tempRule[1].remove('.')
        numbered[key_count] = tempRule
        key_count += 1

    # resolve shift-reduce conflicts by prioritizing shifts
    for stateno in statesDict:
        for rule in statesDict[stateno]:
            if rule[1][-1] == '.':
                for terminal in T + ['$']:
                    if terminal in follow(rule[0]):
                        if Table[stateno][cols.index(terminal)] != '':
                            print(f"Conflict detected in state {stateno} on symbol {terminal}: ", end="")
                            if 'S' in Table[stateno][cols.index(terminal)]:
                                print("Shift-Reduce conflict. Resolving by prioritizing Shift.")
                            else:
                                print("Reduce-Reduce conflict. Manual resolution needed.")
                        else:
                            Table[stateno][cols.index(terminal)] = f"R{list(numbered.keys())[list(numbered.values()).index(rule)]}"
    return Table

# Example usage
rules = [
    "E -> E + T | T",
    "T -> T * F | F",
    "F -> ( E ) | id"
]
nonterm_userdef = ['E', 'T', 'F']
term_userdef = ['+', '*', '(', ')', 'id']
start_symbol = 'E'

diction = {
    'E': [['E', '+', 'T'], ['T']],
    'T': [['T', '*', 'F'], ['F']],
    'F': [['(', 'E', ')'], ['id']]
}

separatedRulesList = grammarAugmentation(rules, nonterm_userdef, start_symbol)
statesDict = {}
stateMap = {}
stateCount = 0

# Create initial state
statesDict[0] = findClosure([], start_symbol)
generateStates(statesDict)

# Compute FIRST and FOLLOW sets
firsts = {nt: first([nt]) for nt in nonterm_userdef}
follows = {nt: follow(nt) for nt in nonterm_userdef}

# Create the parse table
parse_table = createParseTable(statesDict, stateMap, term_userdef, nonterm_userdef)

# Print parse table
for row in parse_table:
    print(row)
