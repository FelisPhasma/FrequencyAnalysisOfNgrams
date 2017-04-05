#!/bin/sh
#http://storage.googleapis.com/books/ngrams/books/googlebooks-eng-all-1gram-20120701-0.gz
#http://storage.googleapis.com/books/ngrams/books/googlebooks-eng-all-totalcounts-20120701.txt
LETTERS=(0 1 2 3 4 5 6 7 8 9 a b c d e f g h i j k l m n o p q r s t u v w x y z other pos punctuation)
CORPUS=eng-all
VERSION=20120701
cd ../data/
for i in "${LETTERS[@]}"
do 
	URL="http://storage.googleapis.com/books/ngrams/books/googlebooks-$CORPUS-1gram-$VERSION-${i}.gz"
	echo "Downloading $URL"
	curl -O $URL
done
echo "Unziping..."
gunzip *.gz

URL="http://storage.googleapis.com/books/ngrams/books/googlebooks-$CORPUS-totalcounts-$VERSION.txt"
echo "Downloading $URL"
curl -O $URL
echo "Done"
