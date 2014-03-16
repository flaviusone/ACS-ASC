
# ex. 1 - operatii cu numere si siruri

# a ) definiti sirul "abc-def-efghi-jkl" si creati o lista cu subsirurile 
# separate de "-", afisati lista

sir = "abc-def-efghi-jkl"
lista = sir.split('-');
print lista;

# b) Modificati lista de la punctul a), astfel incat fiecare subsir sa fie
# concatenat cu pozitia sa in lista, afisati lista

for i in range(len(lista)):
	lista[i] = lista[i] + str(i)
print lista;	

# c) Modificati sirurile de la punctul a), astfel incat fiecare subsir sa fie
# devina de forma <poz_lista><subsir><subsir>, afisati subsirurile
#   * folositi functia format si operatorul *

lista = sir.split('-');
for i in range(len(lista)):
	lista[i] = "%d%s%s" % (i,lista[i],lista[i])
print lista;	

# d) definiti sirul "Trust me, I'm the Doctor"
#  - calculati si afisati numarul de aparitii ale literei "t"
#  - calculati si afisati numarul de aparitii ale literei "t", case-insensitive,
#    folosind functia findall din modulul re.
#  - modificati sirul sa fie in intregime lowercase
#  - pe sirul modificat, luati ultimele 10 caractere si concatenati-le cu 
#    "is cool", iar rezultatul sa aiba toate cuvintele incepand cu litera mare
#      * impartiti sirul intr-o lista de cuvinte
#      * folositi list comprehensions pentru a modifica cuvintele
#      * recreati sirul cu metoda join (tot din metodele pentru siruri)
import re
sir2 = "Trust me, I'm the Doctor";
# Task d.1
print "Numarul de aparitii ale literei t " + str(len(re.findall('t',sir2)))
# Task d.2
print "Numarul de aparitii ale literei t case-insensitive " + str(len(re.findall('(?i)t',sir2)))
# Task d.3
sir2 = sir2.lower()
print sir2
# Task d.4
tmp = sir2[-10:] + " is cool"
lista2 = tmp.split()
lista2 = [x.capitalize() for x in lista2]
print lista2
rez=" "
print rez.join(lista2)

