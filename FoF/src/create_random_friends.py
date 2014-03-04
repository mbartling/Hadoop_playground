#!/usr/bin/env python

import sys
import random

MaxNumFriends = 4000

class person:
    def __init__(self, name):
        self._name = name
        #print self._name
        self._friends = []

    def __repr__(self):
        return '{!r}'.format(self._name)
#    def mystr(self):
#        temp = []
#        for thing in self._friends:
#            temp.append('{!r}'.format(thing))
#        self._friends = temp
#        #return temp
    def __str__(self):
        return '{!r}'.format(self._name)
    def add_friend(self, friend):
        if friend not in self._friends:
            self._friends.append(friend)
    #def __iter__(self):
    #    return iter(self._friends)
    def depth_first(self):
        yield self
        for c in self:
            for cc in c.depth_first():
                yield cc
            #yield from c.depth_first()
    def printMe(self):
        #if len(self._friends) > 1:
        #if self._friends:
        #print self._friends
        #fList = ''
        if self._friends:
            #fList = ' '.join("{!s}".format(self._friends))
            fList = ' '.join(self._friends)
            #x = self._friends
            #print type(x[1])
            #fList = ' '.join(x)
            #fList = str(self._friends)
            #fList = fList.replace('[', ' ')
            #fList = fList.replace(']', ' ')
            #fList = fList.replace('\'', ' ')
            #fList = fList.replace(' , ', ' ')
            print self._name + '\t' + fList
        #if not self._friends:
        #    self._friends.append(' ')
        #fList = ' '
        #fList.join(self._friends)
        #print self._name + '\t' + fList
        #else:
        #    print self._name

def make_friends(personA, personB):
    personA.add_friend(personB._name)
    personB.add_friend(personA._name)

namelistO = []
namelist = []

for name in sys.stdin:
    name = name.strip()
    namelist.append(person(name))


for i in range(0, MaxNumFriends):
    #personA = random.choice(namelist)
    make_friends(random.choice(namelist), random.choice(namelist))
    #A = random.choice(namelist)
    #B = random.choice(namelist)
    #make_friends(A._name, B._name)

print namelist
for people in namelist:
    #people.mystr()
    people.printMe()
    #print people._name
    #print people._friends
## Note that I can treat these as a monoid structure of name and op
#for name in namelist:
#    #print name
#    numFriends
#    
#
## Begin Psuedocode
#There are two types of people, ones who select their friends and those who will add anyone to their circle.
#The people who like to select their friends limit the number of friends in their circle
#
#Choose n unique random names where n < number of names
#
#for each of n selectiveFolk
#    assign random number of total friends (numF)
#
#for name in selectiveFolk
#    #create list of numF unique random people
#    while numF != 0:
#        number = randi(0, num_names)
#        result = add_friend(number, not self) # Not self or in closed_friends
#        numF-= result #whether or not we actually add a friend
#
#add_friend(number, me)
#    if name[number] == me:
#        return 0
#    if name[number] in closed_friends:
#        return 0
#    if name[number] in friendly_people
#        add me as friend to name[number]
#        return 1
#    if name[number] in selective_folk:
#        try to add me as friend to name[number]
#        return 1
#
#maxFriends = 30
#class person:
#    def __init__(self, type):
#        self._type = type
#        if type = 'selective':
#            self.numF = randint(0, maxFriends)
#        else
#            self.numF = 0
#        self.friends = []
#    def append(
