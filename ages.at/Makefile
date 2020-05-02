here: Bundeslander.csv

R_eff_bundesland.csv:
	wget https://www.ages.at/fileadmin/AGES2015/Wissen-Aktuell/COVID19/R_eff_bundesland.csv

Bundeslander=Wien.csv Steiermark.csv Nieder.csv Salzburg.csv Tirol.csv Vorarlberg.csv rnten.csv Ober.csv Burgenland.csv

Header=Datum, Wien, Steiermark, Niederösterreich, Salzburg, Tirol, Vorarlberg, Kärnten, Oberösterreich, Burgenland



Bundesländer: $(Bundeslander)

$(Bundeslander): R_eff_bundesland.csv
	grep -a $(basename $@) $< > $@

dates.inc: Wien.csv Makefile
	printf DATES= > $@
	@awk -v FS=\; -v ORS= '{  print $$2 " " }' < Wien.csv >> $@

include dates.inc

dates: $(addsuffix .dat,$(DATES))

%.dat: Makefile Bundesländer
	printf $(basename $@) > $@
	for f in $(Bundeslander); do awk -v FS=\; -v ORS= '/$(basename $@)/{  print ";" $$3 }' < $$f; done >> $@
	echo >> $@

Bundeslander.csv: $(addsuffix .dat,$(DATES))
	echo $(Header) > $@
	sed '-es|,|.|g' '-es|;|,|g' *.dat >> $@


ID=nZUxz

include ../datawrapper/token.inc

publish:
	curl --request POST \
	--url https://api.datawrapper.de/charts/$(ID)/publish \
	--header 'Authorization: Bearer $(token)'

data: 	Bundeslander.csv Makefile
	curl --request PUT \
	--url https://api.datawrapper.de/v3/charts/$(ID$)/data \
	--header 'Authorization: Bearer $(token)' \
	--header 'content-type: text/csv' \
	--data-binary @Bundeslander.csv

png:	Bundeslander.csv Makefile
	curl --request GET \
	--url 'https://api.datawrapper.de/v3/charts/$(ID)/export/png?unit=px&mode=rgb&plain=false&scale=2&borderWidth=20' \
	--header "Authorization: Bearer $(token)" \
	--header 'accept: */*' \
	--output 'Bundeslaender.png'
