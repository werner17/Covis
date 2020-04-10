include token.inc

basemap=europe
mapkey=ISO_A3

basemap=world-2019
# country code
mapkey=DW_NAME
# isocodes
mapkey=DW_STATE_CODE

#
# The map number, for multimap handling support
#
M=0

date=$(shell date "+%e %b %Y")

CountryCodeColumn=Country
ValueColumn.0=doubling cases (days)
ValueColumn.1=doubling deaths (days)
ValueColumn.2=prct pop 2% mort
ValueColumn.3=prct rep cases 2% mort
ValueColumn=$(ValueColumn.$M)

maptitle.0=Covis19 1/4 - Spreading: Confirmed Case Doubling Rate
maptitle.1=Covis19 2/4 - Spreading: Death Doubling Rate
maptitle.2=Covis19 3/4 - Towards Herd Immunity: Infected Portion
maptitle.3=Covis19 4/4 - Testing Coverage: Estimated Underreporting
maptitle=$(maptitle.$M)


note=Data for $(date) - Analysis & Simulation based on publicly available COVID data - NO WARRANTY! Interpret with caution! All data come with high statistical variations! Estimates are based on 2% mortality hypothesis and assuming 14 days average delay from infection report to fatality.
notes.0=$(note)
notes.1=$(note)
notes.2=$(note)
notes.3=$(note)


sourcename=VP Data Analysis
sourceurl=https://github.com/valeriupredoi/COVID-19_LINEAR/
intro.0=R=1 (one infectious person can transmit the disease to one other person) is achieved when the doubling time is equal to the infectious period, currently assumed to be 14 days. The longer the doubling time per country, the smaller R.
intro.1=Reported diseases are expected to have higher reliability than test results since more likely all deaths are recorded than all infections.
intro.2=Epidemics modelling assumes herd immunity is achieved at 60% infection rate. Countries that suffer most may achieve this status earlier. Note that this estimated portion is based on the current death rate. If that one shrinks over time, then the estimated portion will be smaller. A lower mortality will result in higher infection portion, i.e. herd infection is achieved quicker if mortality is smaller.
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



{{ country_name }} reported {{ cases }} known cases so far. However, at a mortality of 1% the number of reported deaths would imply {{ X1__mort_sim_cases }} actual cases, considering an average delay of 14 days from infection reporting to death.



If that is true, then {{ prct_pop_2__mort }}% of the population in {{ country_name }} already had Covid-19 and only
{{ prct_rep_cases_2__mort }}% cases have been reported yet.

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
	$(MAKE) descriptions notes -j
	$(MAKE) pngs

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

all-data datas data-all:
	$(MAKE) -j $(addprefix data.,$(MAPS))

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
data: data.$M

data.%: | id.inc token.inc
	curl --request PUT \
	--url https://api.datawrapper.de/v3/charts/$(ID$(suffix $@))/data \
	--header 'Authorization: Bearer $(token)' \
	--header 'content-type: text/csv' \
	--data-binary @result.csv
	$(info Data have been successfully uploaded, do "makemap" now.)

result.csv:
	wget $(url)
	mv $(notdir $(url)) $@


maptype="basemap": "$(basemap)", "map-key-attr": "$(mapkey)"

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


define colors.0
"gradient":{
"stops":[{"p":0,"v":2.0},{"p":0.2,"v":3.0},{"p":0.4,"v":4.0},{"p":0.6,"v":7.0},{"p":0.8,"v":14.0},{"p":1,"v":24.0}],
"colors":[{"c":"#201010","p":0},
          {"c":"#a01010","p":0.2},
          {"c":"#ff3000","p":0.4},
          {"c":"#e0d050","p":0.6},
          {"c":"#f8f8e0","p":0.8},
          {"c":"#41c600","p":1}],
"domain":[0,0.1,0.3,0.8,1]
},
"map-key-title": "Days of case doubling",
"map-key-auto": false,
"map-key-format": ".1f",
"map-key-position": "bl",
"map-key-stops": "3,3 days, R>>>1\n
4,4 days, R>>1\n
7,1 week, R>1\n
14,2 weeks, R=1\n
27,3 weeks, R<1
"
endef

colors.%:  | id.inc token.inc
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID$(suffix $@)) \
	--header 'Authorization: Bearer  $(token)' \
	--header 'content-type: application/json' \
 --data-binary $(subst ${\n},,'{"metadata": { "visualize": { $(maptype), $(colors$(suffix $@)) } } } ')


 # --data-binary '{"metadata": { "axes": { "visualize": { $(maptype) } } } } '



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
