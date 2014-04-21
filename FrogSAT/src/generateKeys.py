#! /usr/bin/env python

import sys
import random
#from set import Set
# This script will generate a list of keys to test
# for large files just use head THING.cnf | ./generateKeys.py

numLits = ''
numClauses = ''
keyFile = 'mapKeyList.txt'

for line in sys.stdin:
	line = line.strip()
	(iden, stuff) = line.split(' ',1)
	#print stuff
	if iden == 'p':
		stuff.strip()
		(cnf, numLits, numClauses) = stuff.split()
		break;

#print numClauses
#print numLits
numClauses = int(numClauses)
numLits = int(numLits)

# Generate key pairs (literal pairs) by convention, will be positive.

literals = range(1, numLits+1)
keyLiterals = set()

for i in range(numLits):
	key1 = random.choice(literals)
	key2 = random.choice(literals)
	
	if key1 > key2:
		#Swap to make sure the smallest number comes first 
		# key1,key2 = key2,key1
		temp = key1
		key1 = key2
		key2 = key1
	key = '{0} {1}'.format(key1, key2)
	keyLiterals.add(key)

for Uniquekey in keyLiterals:
	print Uniquekey
