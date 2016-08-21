
include $(OMEGA_HOME)/R/Config/GNUmakefile.Web

DOCS=RCSS.pdf ../inst/doc/guide.html


%.html:	 ../inst/doc/%.xml
	@echo "Making $(@F)"
	$(MAKE) -C $(@D) $(@F)

