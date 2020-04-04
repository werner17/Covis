include token.inc

basemap=europe
mapkey=ISO_A3

basemap=world-2019
mapkey=DW_NAME

#
# The map number, for multimap handling support
#
M=0

date=$(shell date "+%e %b %Y")

CountryCodeColumn=Country
ValueColumn.0=doubling cases (days)
ValueColumn.1=doubling deaths (days)
ValueColumn.2=prct pop 1% mort
ValueColumn.3=prct rep cases 1% mort
ValueColumn=$(ValueColumn.$M)

maptitle.0=Covis19 1/4 - Spreading: Confirmation Doubling Rate
maptitle.1=Covis19 2/4 - Spreading: Death Doubling Rate
maptitle.2=Covis19 3/4 - Towards Herd Immunity: Infected Portion
maptitle.3=Covis19 4/4 - Testing Coverage: Estimated Underreporting
maptitle=$(maptitle.$M)


note=Data for $(date) - Analysis & Simulation based on publicly available COVID data - NO WARRANTY! Interpret with caution! All data come with high statistical varations! Estimates are based on 1% mortality hypothesis and assuming 20 days average delay from infection report to fatality.
notes.0=$(note)
notes.1=$(note)
notes.2=$(note)
notes.3=$(note)


sourcename=VP Data Analysis
sourceurl=https://github.com/valeriupredoi/COVID-19_LINEAR/
intro.0=Infinite doubling time means R=1, i.e. one infectious person can transmit the disease to one other person. The longer the better.
intro.1=Reported diseases are expected to have higher reliability than test results since more likely all deaths are recorded than all infections.
intro.2=Epidemics modelling assumes herd immunity is achieved at 60% infection rate. Countries that suffer most may achieve this status earlier.
intro.3=Low values probably mean only severe cases have been tested. High values indicate a broader sampling of testing also including many healthy candidates (always subject to statistical deviations due to low sampling in particular).


#
# "body": "{{ literacy_rate }}% of adults in this country can read.",
# "title": "{{ country }}",
# "fields": {
# "code": "code",
# "country": "country",
# "literacy_rate": "literacy-rate"
#

tooltiptitle=Covid-19 data for {{ country_name }}

define	tooltipbody
Reported cases are doubling in {{ doubling_cases__days_ }} days, with current daily increase of {{ case_rate }}%.



Deaths are doubling in {{ doubling_deaths__days_ }} days (the more the better) with daily increase of {{ death_rate }}%.



{{ country_name }} reported {{ cases }} known cases so far. However, at a mortality of 1% the number of reported deaths would imply {{ X1__mort_sim_cases }} actual cases, considering an average delay of 20 days from infection reporting to death.



If that is true, then {{ prct_pop_1__mort }}% of the population in {{ country_name }} already had Covid-19 and only
{{ prct_rep_cases_1__mort }}% cases have been reported yet.

endef



#
# From
#  head -1 result.csv
#
define	tooltipfields
"case_rate": "case rate",
"cases": "cases",
"Country": "Country",
"country_name": "country-name",
"death_rate": "death rate",
"deaths": "deaths",
"doubling_cases__days_": "doubling cases (days)",
"doubling_deaths__days_": "doubling deaths (days)",
"pct_pop_0_5__mort": "pct pop 0.5% mort",
"prct_pop_1__mort": "prct pop 1% mort",
"prct_pop_2__mort": "prct pop 2% mort",
"prct_rep_cases_0_5__mort": "prct rep cases 0.5% mort",
"prct_rep_cases_1__mort": "prct rep cases 1% mort",
"prct_rep_cases_2__mort": "prct rep cases 2% mort",
"X0_5__mort_sim_cases": "0.5% mort sim cases",
"X1__mort_sim_cases": "1% mort sim cases",
"X2__mort_sim_cases": "2% mort sim cases"
endef

#valeriu=https://github.com/valeriupredoi/COVID-19_LINEAR/blob/master/ALL_COUNTRIES_DATA_31-03-2020.csv
#valeriu=https://raw.githubusercontent.com/valeriupredoi/COVID-19_LINEAR/master/ALL_COUNTRIES_DATA_31-03-2020.csv
#valeriu=https://raw.githubusercontent.com/valeriupredoi/COVID-19_LINEAR/master/REDUCED_DATA.csv
#valeriu=https://raw.githubusercontent.com/valeriupredoi/COVID-19_LINEAR/master/ALL_COUNTRIES_DATA_31-03-2020.csv

date=31 Mar 2020

#
# ID is provided by the create-map target
#
include id.inc
ID=$(ID.$M)

define	help

Operations PER map:

1. $(MAKE) create-map
  -> check the ID as output from this command, and place it in the file id.inc .
  -> to add support for another map, add the ID to the list there.

2. $(MAKE) data
  -> uploads the provided .csv file

3. $(MAKE) makemap
   -> places the uploaded data on the created map with current ID $(ID)

4. $(MAKE) description
   -> adds description metadata to the map

Data update via:
  rm result.csv;  $(MAKE) result.csv

endef

help:
	$(info $(help))


update:
	rm currentdata.inc
	$(MAKE) currentdata.inc
	$(MAKE) update-data

update-data:
	rm result.csv
	$(MAKE) result.csv
	$(MAKE) all-data


include currentdata.inc

currentdata.inc:
	-rm $@
	wget https://raw.githubusercontent.com/valeriupredoi/COVID-19_LINEAR/master/country_tables/currentdata.inc

ifndef	ID
create-map-because-no-id: create-map
endif

MAPS=0 1 2 3

publish-all:
	$(MAKE) publish M=0
	$(MAKE) publish M=1
	$(MAKE) publish M=2
	$(MAKE) publish M=3

all-data:
	$(MAKE) data M=0
	$(MAKE) data M=1
	$(MAKE) data M=2
	$(MAKE) data M=3

descriptions:
	$(MAKE) -j $(addprefix description.,$(MAPS))

titles:
	$(MAKE) -j $(addprefix title.,$(MAPS))

tooltips:
	$(MAKE) -j $(addprefix tooltip.,$(MAPS))

notes:
	$(MAKE) notes.0 notes.1 notes.2 notes.3 -j


pngs:
	$(MAKE) png.0 png.1 png.2 png.3 -j


png:	$(ID).png

$(ID).png: png.$M

png.%: result.csv id.inc Makefile
	curl --request GET \
	--url 'https://api.datawrapper.de/v3/charts/$(ID$(suffix $@))/export/png?unit=px&mode=rgb&plain=false&scale=2&borderWidth=20' \
	--header "Authorization: Bearer $(token)" \
	--header 'accept: */*' \
	--output '$(ID$(suffix $@)).png'

#
# https://developer.datawrapper.de/docs/creating-a-map
#
data: | id.inc token.inc
	curl --request PUT \
	--url https://api.datawrapper.de/v3/charts/$(ID)/data \
	--header 'Authorization: Bearer $(token)' \
	--header 'content-type: text/csv' \
	--data-binary @result.csv
	$(info Data have been successfully uploaded, do "makemap" now.)

result.csv:
	wget $(url)
	mv $(notdir $(url)) $@


makemap: | id.inc token.inc
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID) \
	--header 'Authorization: Bearer $(token)' \
	--header 'content-type: application/json' \
	--data-binary \
  '{ "metadata": { "axes": { "keys": "$(CountryCodeColumn)", "values": "$(ValueColumn)" }, "visualize": { "basemap": "$(basemap)", "map-key-attr": "$(mapkey)" } }  }'

description: description.$M

description.%: | id.inc token.inc
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID$(suffix $@)) \
	--header 'Authorization: Bearer $(token)' \
	--header 'content-type: application/json' \
  --data-binary '{ "metadata": { "describe": { "source-name": "$(sourcename)", "source-url": "$(sourceurl)", "intro": "$(intro$(suffix $@))" } } }'

title: title.$M

title.%: | id.inc token.inc
	$(info updating title for map$(suffix $@))
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID$(suffix $@)) \
	--header 'Authorization: Bearer $(token)' \
	--header 'content-type: application/json' \
	--data '{ "title": "$(maptitle$(suffix $@))" }'

note: notes.$M
notes.%: | id.inc token.inc
	$(info updating notes for map$(suffix $@))
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID$(suffix $@)) \
	--header 'Authorization: Bearer $(token)' \
	--header 'content-type: application/json' \
	--data '{ "metadata": { "annotate": { "notes": "$(notes$(suffix $@))"} } }'


chart-height=450
embed-height=600
embed-width=1024

size.%: | id.inc token.inc
	$(info updating title for map $(suffix $@))
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID$(suffix $@)) \
	--header 'Authorization: Bearer $(token)' \
	--header 'content-type: application/json' \
	--data '{ "metadata": {  "publish": { "chart-height": $(chart-height), "embed-height": $(embed-height), "embed-width": $(embed-width) } } }'


publish: | id.inc token.inc
	curl --request POST \
	--url https://api.datawrapper.de/charts/$(ID)/publish \
	--header 'Authorization: Bearer $(token)'

publish.%: | id.inc token.inc
	curl --request POST \
	--url https://api.datawrapper.de/charts/$(ID$(suffix $@))/publish \
	--header 'Authorization: Bearer $(token)'


define \n


endef

curl.tooltipbody=$(subst ${\n},\n,$(tooltipbody))
curl.tooltipfields=$(subst ${\n},,$(tooltipfields))

tooltip:  tooltip.$M

tooltip.%:  | id.inc token.inc
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID$(suffix $@)) \
	--header 'Authorization: Bearer  $(token)' \
	--header 'content-type: application/json' \
  --data-binary '{"metadata": { "visualize": { "tooltip": {"body": "$(curl.tooltipbody)", "title": "$(tooltiptitle)", "fields": { $(curl.tooltipfields) } } } } }'

maptype.%:  | id.inc token.inc
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID$(suffix $@)) \
	--header 'Authorization: Bearer  $(token)' \
	--header 'content-type: application/json' \
	--data-binary '{ "type": "d3-maps-choropleth" }'

external-url.%:  | id.inc token.inc
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID$(suffix $@)) \
	--header 'Authorization: Bearer  $(token)' \
	--header 'content-type: application/json' \
	--data-binary '{ "externalData": "$(url)" }'

viz:
	$(info https://app.datawrapper.de/map/$(ID)/visualize)

me:	 | token.inc
	curl --request GET \
	--url https://api.datawrapper.de/v3/me \
	--header "Authorization: Bearer $(token)"

#
# Show the list of available basemapes
#
show-basemaps:
	curl --request GET \
	--url https://api.datawrapper.de/plugin/basemaps

#
# Show properties of the selected basemap
#
map:
	curl --request GET \
	--url https://api.datawrapper.de/plugin/basemaps/$(basemap)/$(mapkey)

create-map: | token.inc
	curl --request POST \
	--url https://api.datawrapper.de/v3/charts \
	--header 'Authorization: Bearer $(token)' \
	--header 'content-type: application/json' \
	--data '{ "title": "$(maptitle)", "type":"d3-maps-choropleth"}'
	$(info Place the ID into the file $@ using the syntax ID=<ID> with <ID> being the id from the above response.)
