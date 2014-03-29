#! /usr/bin/env python
import sys
import os

literals = set()
clauseList = []

WINDOWSMODE = 1
verbosity = 2
MSVERBOSITY = " -verb={0}".format(verbosity)
def runMiniSat(filename, filenameout):
    if WINDOWSMODE == 1:
        minisat = "./minisat_static.exe " 
        minisat = minisat + filename + " " + filenameout + MSVERBOSITY + " -no-luby -rinc=1.5 -phase-saving=0 -rnd-freq=0.02"
        os.system(minisat)
    else:
        minisat = "./minisat_static " 
        minisat = minisat + filename + " " + filenameout + MSVERBOSITY +  " -no-luby -rinc=1.5 -phase-saving=0 -rnd-freq=0.02"
        os.system(minisat)
    

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
    	#numLits = len(literals)/2 # Should be even number
	# MiniSAT determines the number of literals and compares it to our problem statement. Note miniSat is lazy and looks for the max(abs) of the literals.
	numLits = max(literals)
    	filenameTxt = prevKey.replace(' ', '-')
    	filename = '{0}.cnf'.format(filenameTxt)
	filenameout = '{0}-MiniSat.out'.format(filenameTxt)    	
    	# Open the file to work on
    	f = open(filename, 'w')

    	# write out problem statement
    	# p cnf numliterals numClauses\n
    	#problem  = ('p cnf', numLits, len(clauseList), '\n')
    	problem = 'p cnf {0} {1}\n'.format(numLits, len(clauseList))
    	# MiniSAT doesnt need this, see www.msoos.org/minisat-faq/
	f.write(problem)

    	# Write the clauses
    	#f.writelines(clauseList)
    	for entry in clauseList:
    	    f.write('{0}\n'.format(entry))
    	
    	f.close()
    	
    	# Run the command line minisat here
    	print 'Running Minisat Dummy'
    	#os.system("cat " + filename+ ">" + '{0}.out'.format(filenameTxt))
    	runMiniSat(filename, filenameout)
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

#numLits = len(literals)/2 # Should be even number
numLits = max(literals)
filenameTxt = key.replace(' ', '-')
filename = '{0}.cnf'.format(filenameTxt)
filenameout = '{0}-MiniSat.out'.format(filenameTxt)    	
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
    f.write('{0}\n'.format(entry))
    
f.close()

# Run the command line minisat here
print 'Running Minisat Dummy'
#os.system("cat " + filename+ ">" + '{0}.out'.format(filenameTxt))
runMiniSat(filename, filenameout)
literals.clear()
clauseList = []
