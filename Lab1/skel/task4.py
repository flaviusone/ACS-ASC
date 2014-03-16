# Arhitectura sistemelor de calcul - Laboratorul 1
# ex.4 Generator

# a) creati o lista care contine toate elementele pare de la 0 la N 
#    multiplicate cu 3.
#    - folositi range si list comprehensions
#    - afisati valorile separate prin spatiu

print "\n-------------------------------------\n"

# b) creati un generator pentru elementele pare de la 0 la N multiplicate cu 3.
#    - folositi range si tuple comprehensions ((...) in loc de [...]creeaza un
#      generator) 
#    - afisati folosind functia type tipul rezultatului expresiei, ce observati
#    - afisati valorile separate prin spatiu
#    - care este avantajul generatorului?

print "\n-------------------------------------\n"

# c) afisati primele N numere pare separate prin spatiu, folositi xrange in loc
#    de range
#    - xrange e un generator, genereaza pe rand valori, range creeaza o lista 
#      cu toate valorile 

print "\n-------------------------------------\n"

# d) the magic of 'yield' - testati si urmariti exemplul urmator

# Citirea "lazy" a liniilor dintr-un fisier

def read_non_empty_file_lines(file):
    for line in file:
        if line.strip()!='':        
            yield line      # return line
    
def readfile(filename):    
    try:
        f = open(filename)
        for line in read_non_empty_file_lines(f):
            print line,
            #TODO - copy stuff from task2
            
    except IOError as e:
        print str(e)
    else:       
        f.close()
 
readfile("openttd.cfg")

"""
- De ce nu folosim io.readline() din modulul io?
Daca am folosi readline, prelucrarea liniei (fie ea un singur apel de functie)
devine strans cuplata de citirea ei, fiind facuta in bucla de parcurgere linie
cu linie a fisierului. 

- De ce nu folosim readlines() din modulul io?
Daca dorim sa decuplam citirea fisierului de prelucrarea lui trebuie sa 
intoarcem cumva continutul acestuia, deci sa il mentinem in memorie, ceea ce nu
e eficient cand fisierele au dimensiuni mari.

- De ce sa folosim yield?
Vrem sa iteram pe datele din fisier (fie ele linii sau chunkuri etc) fara sa le
incarcam pe toate deodata in memorie. Folosind yield, metoda 
'read_non_empty_file_lines' va deveni un obiect generator. 

- Cum functioneaza?
Generatorii sunt de fapt iteratori si au metoda '__next__'. La iteratia din bucla
'for' se apeleaza next pe 'read_non_empty_file_lines', si se va intoarce o linie. 
Din cauza folosirii 'yield', metoda nu se executa dintr-o data, ci 'yield' opreste
executia salvand starea metodei
"""
    
