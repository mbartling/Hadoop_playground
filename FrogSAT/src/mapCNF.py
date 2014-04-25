#! /usr/bin/env python

import sys
sys.path.append('.')
f = open('mapKeyList.txt', 'r')

keyList = []
# make a list of all of the relevant keys
for line in f:
	line.strip()
	keyList.append(line)

f.close()

# Assume that there are no comment blocks or problem statements in this file
for line in sys.stdin:
	line = line.strip()
	lineS = line.split()
	# For each line we receive, ignore comments and problem statements
	if lineS[0] != 'c' and lineS[0] != 'p':
		for key in keyList:
			(key1, key2) = key.split()
			#print key1, key2
			# if either A or ~A or B or ~B in the clause then print out
			# key1 key2 \t clause, where key1 < key2
			neg1 = '-%s' % (key1)
			neg2 = '-%s' % (key2)
			if (key1 in lineS) or (key2 in lineS):
				keyS= ''.join(key)
				keyS = keyS.strip('\n')
				#keyS = '{0}\t'.format(keyS)
				keyS = '%s\t' %(keyS)
				print keyS + line
				print >> sys.stderr, keyS + line
			#if ('-{0}'.format(key1) in line) or ('-{0}'.format(key2) in line):
			if (neg1 in line) or (neg2 in line):
				keyS = ''.join(key)
				keyS = keyS.strip('\n')
				#keyS = '{0}\t'.format(keyS)
				keyS = '%s\t' %(keyS)
				print keyS + line
				print >> sys.stderr, keyS + line
