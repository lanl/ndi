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

2.0.4: context_2.0.4.ps rcf_2.0.4.ps

fy06: context_fy06.ps rcf_fy06.ps
fy06.pdf: context_fy06.pdf rcf_fy06.pdf

2.0.2: context_2.0.2.ps rcf_2.0.2.ps
2.0.1: context_2.0.1.ps rcf_2.0.1.ps

rcf_2.0.4.ps: rcf_2.0.4.tex rcf17.tex rcf20.tex rcf21.tex \
		rcf24.tex rcf25.tex rcf26.tex rcf27.tex

rcf_2.0.2.ps: rcf_2.0.2.tex rcf7.tex rcf12.tex rcf14.tex \
		rcf16.tex rcf17.tex rcf19.tex

rcf_fy06.ps: rcf_fy06.tex rcf15.tex rcf18.tex rcf20.tex \
		rcf21.tex rcf22.tex rcf23.tex rcf24.tex \
		rcf25.tex rcf26.tex rcf27.tex

rcf_fy06.pdf: rcf_fy06.tex rcf15.tex rcf18.tex rcf20.tex \
		rcf21.tex rcf22.tex rcf23.tex rcf24.tex \
		rcf25.tex rcf26.tex rcf27.tex

clean:
	rm *.ps *.pdf *.aux *.log *~
