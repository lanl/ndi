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


2.0.2: context_2.0.2.ps rcf_2.0.2.ps
2.0.1: context_2.0.1.ps rcf_2.0.1.ps

rcf_2.0.2.ps: rcf_2.0.2.tex rcf6.tex rcf7.tex rcf8.tex rcf12.tex rcf14.tex \
		rcf15.tex rcf16.tex

clean:
	rm *.ps *.pdf *.aux *.log *~
