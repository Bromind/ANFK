#compile twice to have the table of contents
all:
	pdflatex doc_fr.tex && pdflatex doc_fr.tex && mv doc_fr.pdf ..

clean: 
	-rm *.log *.aux *.toc
