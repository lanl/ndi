%.ps: %.dot
	dot -Tps -o $@ $<

%.pdf: %.tex
	pdflatex $<
	pdflatex $<

%.dvi: %.tex
	latex $<
	latex $<

%.ps: %.dvi
	dvips -o $@ $<


2.0.1: context_2.0.1.ps rcf_2.0.1.ps
2.0.2: context_2.0.2.ps rcf_2.0.2.ps

clean:
	rm *.ps *.pdf *.aux *.log *~
