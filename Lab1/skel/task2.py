# Arhitectura sistemelor de calcul - Laboratorul 1
# ex. 2 - lucrul cu fisierele

# a) cititi fisierul de configurare openttd.cfg astfel incat sa mentineti toate
# configurarile necesare sub forma unui dictionar
#  - folositi metodele din modulul io
#  - ignorati liniile goale sau cele care contin doar un sir de forma [<ceva>]
#  - acolo unde proprietatea nu are setata o valoare, folositi None
#  - afisati dictionarul

dictionar_global={}

def task_a():
	import io
	dict={}
	with open('openttd.cfg', 'r') as f:
		for line in f:   #citire linie cu linie din fisier
			lista_linie = line.split()
			if line.strip()!='' and len(lista_linie)>1: #elimin liniile goale si cele cu [ceva]
				# print lista_linie
				if len(lista_linie) == 2:
					# proprietatea nu are valoare
					dict[lista_linie[0]] = "None"
				else:
					dict[lista_linie[0]] = lista_linie[2]
	print dict
	f.close()

# Aici rulez task a
print "\nTask a)"
task_a()


# b) completati codul anterior astfel incat:
#  - sa eliminati caracterele de tip spatiu de la inceputul si sfarsitul 
#    sirurilor (metoda strip)
#  - daca proprietatile au valori numerice, pastrati-le in dictionar ca tipuri
#    numerice
#  - daca proprietatile au ca valori o lista (separate prin ','), pastrati-le
#    ca liste

def task_b():
	import io
	dict={}
	with open('openttd.cfg', 'r') as f:
		for line in f:   #citire linie cu linie din fisier
			lista_linie = line.split()
			if line.strip()!='' and len(lista_linie)>1: #elimin liniile goale si cele cu [ceva]
				if len(lista_linie) == 2:
					# proprietatea nu are valoare
					dict[lista_linie[0]] = "None"
				else:
					temp = lista_linie[2]
					temp = temp.split(',') #split dupa ,
					if len(temp) > 1: 	# atunci e o lista
						dict[lista_linie[0]] = temp
					else:				# nu e lista e numar sau string(true/false)
						if lista_linie[2].isdigit():
							dict[lista_linie[0]] = int(lista_linie[2])	
	print dict
	global dictionar_global
	dictionar_global = dict
	f.close()

#Aici rulez task b
print "\nTask b)"
task_b()

# c) faceti verificare de erori de format fisier (e.g. o linie nu e de forma
#    "ceva = altceva", linie goala sau linie incepand cu [)
#  - afisati proprietatile doar daca nu s-a intalnit o eroare
#  - folositi clauza 'else' pentru bucla de citire fisier


def task_c():
	s=''
	with open('openttd.cfg', 'r') as f:
		for line in f:   #citire linie cu linie din fisier
			if line.strip()=='' or line[0]=='[' or (len(line.split()) == 3 and line.split()[1] ==  '='):
				s+=line
			else:
				print "Eroare" # pt ca "screenshot_format =" de ex
				break
		else:
			print s

print "\nTask c)"
task_c()

# d) folositi modulul pickle pentru a serializa dictionarul de la punctele anterioare
#  - salvati dictionarul intr-un alt fisier
#  - incarcati dictionarul din fisierul salvat si afisati-l

#Aici trebuie rulat unul din taskurile anterioare pentru a obtine dictionarul global

def task_d():
	import pickle
	import io
	pickle.dump(dictionar_global,open( "fisier_dictionar", "wb" ) )

	dictionar_incarcat = pickle.load( open( "fisier_dictionar", "rb" ) )
	print dictionar_incarcat

print "\nTask d)"
task_d()

# e) modificati codul astfel incat sa aveti in vedere aparitia unor exceptii
#  - exemplu folosire try/except/finally http://docs.python.org/2/tutorial/errors.html
#  - simulati aparitia unei exceptii (e.g. fisierul nu exista), afisati  mesajul
#    exceptiei
#  - folositi clauza else pentru blocul try, afisati proprietatile doar daca nu 
#    au aparut exceptii

def task_e():
	import pickle
	import io
	pickle.dump(dictionar_global,open( "fisier_dictionar", "wb" ) )

	try:
		dictionar_incarcat = pickle.load( open( "fisier_gresit", "rb" ) )
	except(IOError):
		print IOError
	else:
		print dictionar_incarcat

print "\nTask e)"
task_e()

# f) organizati codul de mai sus in metode pentru citire fisier si pentru serializare.
#  - def read_file(filepath) - intoarce un dictionar cu configurari
#  - def store_data(filepath, data)
#  - def load_data(filepath) - intoarce un dictionar

def red_file(filepath):
	import io
	dict={}
	with open('openttd.cfg', 'r') as f:
		for line in f:   #citire linie cu linie din fisier
			lista_linie = line.split()
			if line.strip()!='' and len(lista_linie)>1: #elimin liniile goale si cele cu [ceva]
				if len(lista_linie) == 2:
					# proprietatea nu are valoare
					dict[lista_linie[0]] = "None"
				else:
					temp = lista_linie[2]
					temp = temp.split(',') #split dupa ,
					if len(temp) > 1: 	# atunci e o lista
						dict[lista_linie[0]] = temp
					else:				# nu e lista e numar sau string(true/false)
						if lista_linie[2].isdigit():
							dict[lista_linie[0]] = int(lista_linie[2])	
	return dict
	f.close()

def load_data(filepath):
	import pickle
	import io
	try:
		dictionar_incarcat = pickle.load( open( filepath, "rb" ) )
	except(IOError):
		return IOError
	else:
		return dictionar_incarcat

def store_data(filepath, data):
	import pickle
	import io
	pickle.dump(data,open( filepath, "wb" ) )