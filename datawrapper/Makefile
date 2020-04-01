include token.inc

basemap=europe
mapkey=ISO_A3

basemap=world-2019
mapkey=DW_NAME


CountryCodeColumn=Country
ValueColumn=doubling cases (days)

maptitle=Experimental Map Visualization
sourcename=Valeriu Predoi Data Analysis
sourceurl=https://github.com/valeriupredoi/COVID-19_LINEAR/
intro=Data Analysis of public COVID data


#
# "body": "{{ literacy_rate }}% of adults in this country can read.",
# "title": "{{ country }}",
# "fields": {
# "code": "code",
# "country": "country",
# "literacy_rate": "literacy-rate"
#

tooltiptitle=Key data for {{ Country }}

define	tooltipbody
Reported cases are doubling in {{ doubling_cases__days_ }} days, deaths doubling in {{ doubling_deaths__days_ }} days (the more the better).

Current case rate {{ case_rate__day_1_ }}, death rate {{ death_rate__day_1_ }}.

Predicted daily cases for simulated mortality based on reported deaths:
{{ X0_5__mort_sim_cases }} at 0.5%,
{{ X1__mort_sim_cases }}   at 1.0%,
{{ X2__mort_sim_cases }}   at 2.0%

endef



#
# From
#  head -1 result.csv
#
define	tooltipfieldsx
  "Country": "country"
, "case rate (day-1)": "case_rate__day_1_"
, "death rate (day-1)": "death_rate__day_1_"
, "doubling cases (days)": "doubling_cases__days_"
, "doubling deaths (days)": "doubling_deaths__days_"
, "0.5% mort sim cases": "X0_5__mort_sim_cases"
, "1% mort sim cases": "X1__mort_sim_cases"
, "2% mort sim cases": "X2__mort_sim_cases"
endef

valeriu=https://github.com/valeriupredoi/COVID-19_LINEAR/blob/master/ALL_COUNTRIES_DATA_31-03-2020.csv
valeriu=https://raw.githubusercontent.com/valeriupredoi/COVID-19_LINEAR/master/ALL_COUNTRIES_DATA_31-03-2020.csv
valeriu=https://raw.githubusercontent.com/valeriupredoi/COVID-19_LINEAR/master/REDUCED_DATA.csv

#
# ID is provided by the create-map target
#
include id.inc

define	help
1. $(MAKE) create-map
  -> check the ID as output from this command, and place it in the file id.inc .

2. $(MAKE) data
  -> uploads the provided .csv file

3. $(MAKE) makemap
   -> places the uploaded data on the created map with current ID $(ID)

4. $(MAKE) description
   -> adds description metadata to the map

endef

help:
	$(info $(help))

ifndef	ID
create-map-because-no-id: create-map
endif

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
	wget $(valeriu)
	mv $(notdir $(valeriu)) $@


makemap: | id.inc token.inc
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID) \
	--header 'Authorization: Bearer $(token)' \
	--header 'content-type: application/json' \
	--data-binary \
  '{ "metadata": { "axes": { "keys": "$(CountryCodeColumn)", "values": "$(ValueColumn)" }, "visualize": { "basemap": "$(basemap)", "map-key-attr": "$(mapkey)" } }  }'

description: | id.inc token.inc
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID) \
	--header 'Authorization: Bearer $(token)' \
	--header 'content-type: application/json' \
  --data-binary '{ "metadata": { "describe": { "source-name": "$(sourcename)", "source-url": "$(sourceurl)", "intro": "$(intro)" } } }'


define \n


endef

curl.tooltipbody=$(subst ${\n},\n,$(tooltipbody))
curl.tooltipfields=$(subst ${\n},,$(tooltipfields))

tooltip:  | id.inc token.inc
	curl  --request PATCH \
	--url https://api.datawrapper.de/v3/charts/$(ID) \
	--header 'Authorization: Bearer  $(token)' \
	--header 'content-type: application/json' \
  --data-binary '{"metadata": { "visualize": { "tooltip": {"body": "$(curl.tooltipbody)", "title": "$(tooltiptitle)", "fields": { $(curl.tooltipfields) } } } } }'


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
