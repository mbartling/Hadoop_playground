#! /usr/bin/env python
import sys
import os

literals = set()
clauseList = []

#prevKey = ''
line  = sys.stdin.readline()
line = line.strip()
(key,clause) = line.split('\t')
lits = clause.split()
prevKey = key

for literal in lits:
    literal = int(literal)
    literals.add(literal)
    literals.add(-literal)
clauseList.append(clause)

for line in sys.stdin:
    line = line.strip()
    (key,clause) = line.split('\t')

    if key != prevKey:
        numLits = len(literals)/2 # Should be even number
        filenameTxt = prevKey.replace(' ', '-')
        filename = '{0}.cnf'.format(filenameTxt)
        
        # Open the file to work on
        f = open(filename, 'w')

        # write out problem statement
        # p cnf numliterals numClauses\n
        #problem  = ('p cnf', numLits, len(clauseList), '\n')
        problem = 'p cnf {0} {1}\n'.format(numLits, len(clauseList))
        f.write(problem)

        # Write the clauses
        #f.writelines(clauseList)
        for entry in clauseList:
            f.write('{0} 0\n'.format(entry))
        
	f.close()
        
        # Run the command line minisat here
        print 'Running Minisat Dummy'
	os.system("cat " + filename+ ">" + '{0}.out'.format(filenameTxt))
        literals.clear()
        clauseList = []
    
    # Working on same key
    lits = clause.split()
    prevKey = key
    for literal in lits:
        literal = int(literal)
        literals.add(literal)
        literals.add(-literal)
    clauseList.append(clause)

# Process the last key

numLits = len(literals)/2 # Should be even number
filenameTxt = key.replace(' ', '-')
filename = '{0}.cnf'.format(filenameTxt)

# Open the file to work on
f = open(filename, "w")

# write out problem statement
# p cnf numliterals numClauses\n
#problem  = ('p cnf', numLits, len(clauseList), '\n')
problem = 'p cnf {0} {1}\n'.format(numLits, len(clauseList))
f.write(problem)

# Write the clauses
#f.writelines(clauseList)
for entry in clauseList:
    f.write('{0} 0\n'.format(entry))
        
f.close()

# Run the command line minisat here
print 'Running Minisat Dummy'
os.system("cat " + filename+ ">" + '{0}.out'.format(filenameTxt))
literals.clear()
clauseList = []
