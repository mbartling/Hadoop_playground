#! /usr/bin/env python

import sys

f = open('mapKeyList.txt', 'r')

keyList = []
for line in f:
	line.strip()
	keyList.append(line)

# Assume that there are no comment blocks or problem statements in this file
for line in sys.stdin:
	line = line.strip()
	lineS = line.split()
	if lineS[0] != 'c' and lineS[0] != 'p':
		for key in keyList:
			(key1, key2) = key.split()
			#print key1, key2
			if (key1 in lineS) or (key2 in lineS):
				keyS= ''.join(key)
				keyS = keyS.strip('\n')
				keyS = '{0}\t'.format(keyS)
				print keyS + line
			if ('-{0}'.format(key1) in line) or ('-{0}'.format(key2) in line):
				keyS = ''.join(key)
				keyS = keyS.strip('\n')
				keyS = '{0}\t'.format(keyS)
				print keyS + line
