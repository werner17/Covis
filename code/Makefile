run: run.cpp
	g++ -std=c++2a -o $@ $<
	/bin/time -p ./run

edc.europa.eu.csv:
	wget https://opendata.ecdc.europa.eu/covid19/casedistribution/csv
	mv -f csv $@

update: edc.europa.eu.csv

.PHONY: edc.europa.eu.csv
