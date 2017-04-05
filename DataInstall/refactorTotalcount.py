PATH = "C:/Users/Felis/Desktop/programming/SuperComputing/WordData/data/"
Infile = "googlebooks-eng-all-totalcounts-20120701.txt"
Outfile = "googlebooks-eng-all-totalcounts-20120701.dat"
def init():
	line = ""
	with open(PATH + Infile, "r") as infile:
		for line in infile:
			line = line
	# Should only be one line...
	lines = line.split("\t")
	with open(PATH + Outfile, "w+") as outfile:
		for l in lines:
			if l == " " or l == "":
				continue
			outfile.write(l.replace(",", "\t") + "\n")
init()