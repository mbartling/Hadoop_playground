#! /usr/bin/env python

import sys

for line in sys.stdin:
    line = line.strip()
    #we get main person TAB list of friends
    person, friendlist = line.split('\t')

    # Split the friends list into a list of names
    namelist = friendlist.split()
    
    # pair the person with someone in their list of friends. Output the first name as the one with the lower lexigraphical order. 
    for name in namelist:
        if person < name:
            print '%s %s\t%s' % (person, name, friendlist)
        else:
            print '%s %s\t%s' %  (name, person, friendlist)
