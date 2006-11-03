
# DEFINE VERSION and DEPENDS list

VERSION ?= $(shell ls -t rcf_*.tex | head -1 | sed 's/rcf_\(.*\)\.tex/\1/')

DEPENDS	:= $(shell grep input rcf_$(VERSION).tex | \
			sed 's/^[ \t]*\\input{\(.*\)}[ \t]*$$/\1.tex/')

RERUN = "(There were undefined references|Rerun to get (cross-references|the bars) right)"
RERUNBIB = "No file.*\.bbl|Citation.*undefined"



# RULES

%.ps: %.dot
	dot -Tps -o $@ $<

%.pdf: %.tex
	pdflatex $<
	egrep -c $(RERUNBIB) $*.log && (bibtex $*;pdflatex $<); true
	egrep $(RERUN) $*.log && (pdflatex $<) ; true
	egrep $(RERUN) $*.log && (pdflatex $<) ; true

# TARGETS

$(VERSION): context_$(VERSION).ps rcf_$(VERSION).pdf

context_$(VERSION).ps: context_$(VERSION).dot

rcf_$(VERSION).pdf: rcf_$(VERSION).tex $(DEPENDS)

.PHONY: print clean

print: context_$(VERSION).ps rcf_$(VERSION).pdf
	lpr context_$(VERSION).ps rcf_$(VERSION).pdf

clean:
	rm -f *.ps *.pdf *.aux *.log *.dvi *~
